#include "SFMLApp.h"

#include "logger.h"

#ifdef TRACY_ENABLE
#include "Tracy.hpp"
#include "TracyC.h"
#endif

void SFMLApp::SetUp() {
  // auto screen = sf::VideoMode::getDesktopMode();
  // Metrics::Width = screen.width;
  // Metrics::Height = screen.height;
  _window.create(sf::VideoMode(Metrics::Width, Metrics::Height), Title,
                 sf::Style::Close);

  _sceneManager.SetUp(&_client, &_window);
}

void SFMLApp::TearDown() const noexcept {}

void SFMLApp::Run() noexcept {
  bool quit = false;

  bool adjustWindow = true;

  sf::Event e;

  while (!quit) {
    while (Packet* packet = _client.PopPacket()) {
      OnPacketReceive(*packet);

      auto packetTypeValue = static_cast<int>(packet->type);

      if (packetTypeValue >= 0 &&
          packetTypeValue <= static_cast<int>(PacketType::Invalid)) {
        delete packet;
      }
    }
    while (_window.pollEvent(e)) {
      switch (e.type) {
        case sf::Event::Closed:
          quit = true;
          break;
        case sf::Event::MouseButtonReleased:
          if (e.mouseButton.button == sf::Mouse::Left) {
            _sceneManager.GiveLeftMouseClickToScene();
            _sceneManager.StartConnection(_client.IsConnected);
          }
          break;
        case sf::Event::TextEntered:
          switch (e.text.unicode) {
            case BackspaceKeyCode_:
              if (!_username.empty()) {
                _username.pop_back();
              }
              break;
            case SpaceKeyCode_:
              _username += ' ';
              break;
            default:
              if (_username.size() >= 10) {
                break;
              }
              if (e.text.unicode > SpaceKeyCode_ && e.text.unicode < 122) {
                _username += static_cast<char>(e.text.unicode);
              }
              break;
          }
          break;
      }
    }

    _window.clear(sf::Color::Black);

    sf::Vector2i sfMousePos = sf::Mouse::getPosition(_window);
    MousePos = {sfMousePos.x, sfMousePos.y};
    _sceneManager.GiveMousePositionToScene(
        {(float)MousePos.X, (float)MousePos.Y});

    _sceneManager.UpdateScene();
    _sceneManager.DrawPlayerData(_username, 4, _p1, _p2);
    if (_sceneManager.UpdateQuitButton()) {
      _sceneManager.ChangeScene(0, &_client, &_window);
    }

    DrawAllGraphicsData();

    _window.display();

#ifdef TRACY_ENABLE
    FrameMark;
#endif
  }
}

void SFMLApp::DrawCircle(const Math::Vec2F center, const float radius,
                         const int segments, const sf::Color& col) noexcept {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif

  sf::CircleShape circle = sf::CircleShape(radius, segments);
  circle.setFillColor(col);
  circle.setOrigin(radius, radius);
  circle.setPosition(center.X, center.Y);

  _window.draw(circle);
}

void SFMLApp::DrawRectangle(const Math::Vec2F minBound,
                            const Math::Vec2F maxBound,
                            const sf::Color& col) noexcept {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif

  // Calculate the width and height of the rectangle
  float width = maxBound.X - minBound.X;
  float height = maxBound.Y - minBound.Y;

  // Create an SFML rectangle shape
  sf::RectangleShape rectangle(sf::Vector2f(width, height));

  // Set the position and color of the rectangle
  rectangle.setPosition(minBound.X, minBound.Y);
  rectangle.setFillColor(col);

  // Draw the rectangle to the window
  _window.draw(rectangle);
}

void SFMLApp::DrawRectangleBorder(const Math::Vec2F minBound,
                                  const Math::Vec2F maxBound,
                                  const sf::Color& col) noexcept {
  // Calculate the width and height of the rectangle
  float width = maxBound.X - minBound.X;
  float height = maxBound.Y - minBound.Y;

  // Create an SFML rectangle shape
  sf::RectangleShape rectangle(sf::Vector2f(width, height));

  // Set the position and color of the rectangle
  rectangle.setPosition(minBound.X, minBound.Y);
  rectangle.setOutlineColor(col);
  rectangle.setOutlineThickness(1);
  rectangle.setFillColor(sf::Color::Transparent);

  // Draw the rectangle to the window
  _window.draw(rectangle);
}

void SFMLApp::DrawPolygon(const std::vector<Math::Vec2F>& vertices,
                          const sf::Color& col) {
  if (vertices.size() < 3) {
    return;  // Don't draw if the polygon is invalid
  }

  sf::ConvexShape polygon(vertices.size());

  for (size_t i = 0; i < vertices.size(); ++i) {
    polygon.setPoint(i, sf::Vector2f(vertices[i].X, vertices[i].Y));
  }

  polygon.setFillColor(col);

  _window.draw(polygon);
}

void SFMLApp::DrawAllGraphicsData() noexcept {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  auto sceneData = _sceneManager.GetSceneData();

  for (auto it = sceneData.rbegin(); it != sceneData.rend(); ++it) {
    auto& bd = *it;

    if (bd.Shape.index() == static_cast<int>(Math::ShapeType::Circle)) {
      auto& circle = std::get<Math::CircleF>(bd.Shape);
      DrawCircle(circle.Center(), circle.Radius(), 30, bd.Color);
    } else if (bd.Shape.index() ==
               static_cast<int>(Math::ShapeType::Rectangle)) {
      auto& rect = std::get<Math::RectangleF>(bd.Shape);
      if (!bd.Filled) {
        DrawRectangleBorder(rect.MinBound(), rect.MaxBound(), bd.Color);
      } else {
        DrawRectangle(rect.MinBound(), rect.MaxBound(), bd.Color);
      }
    } else if (bd.Shape.index() == static_cast<int>(Math::ShapeType::Polygon)) {
      auto& polygon = std::get<Math::PolygonF>(bd.Shape);
      DrawPolygon(polygon.Vertices(), bd.Color);
    }
  }
}

void SFMLApp::OnPacketReceive(const Packet& packet) {
  if (packet.type == PacketType::StartGame) {
    _sceneManager.ChangeScene(1, &_client, &_window);
    auto& startPacket = dynamic_cast<const StartGamePacket&>(packet);
    _p1.name = startPacket.p1Name;
    _p1.elo = startPacket.p1Elo;
    _p2.name = startPacket.p2Name;
    _p2.elo = startPacket.p2Elo;
  }

  if (packet.type == PacketType::HasPlayed) {
    auto& hasplayedpacket = dynamic_cast<const HasPlayedPacket&>(packet);

    _sceneManager.OtherPlayerHasPlayed(hasplayedpacket.IsFirstTurn,
                                       hasplayedpacket.X, hasplayedpacket.Y,
                                       hasplayedpacket.index);
  }
  if (packet.type == PacketType::Surrender) {
    _sceneManager.OtherPlayerHasSurrendered();
  }
}
