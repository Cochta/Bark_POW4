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

  sf::Socket::Status status = _client.socket->connect("localhost", 55555);

  if (status != sf::Socket::Done) {
    LOG_ERROR("Could not connect to server");
    _window.close();
  }

  _networkClientManager.SetOnMessageReceived([this](const Packet& packet) {
    if (packet.type == PacketType::StartGame) {
      _sceneManager.ChangeScene(1, &_client, &_window);
    }

    if (packet.type == PacketType::HasPlayed) {
      auto& hasplayedpacket = dynamic_cast<const HasPlayedPacket&>(packet);

      _sceneManager.OtherPlayerHasPlayed(hasplayedpacket.IsFirstTurn,
                                         hasplayedpacket.X, hasplayedpacket.Y);
    }
    return true;
  });
  _networkClientManager.StartThreads(_client);
}

void SFMLApp::TearDown() const noexcept {}

void SFMLApp::Run() noexcept {
  bool quit = false;

  bool adjustWindow = true;

  sf::Event e;

  while (!quit) {

    while (_window.pollEvent(e)) {
      switch (e.type) {
        case sf::Event::Closed:
          quit = true;
          break;
        case sf::Event::MouseButtonReleased:
          if (e.mouseButton.button == sf::Mouse::Left) {
            _sceneManager.GiveLeftMouseClickToScene();
            _sceneManager.StartConnection();
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

    if (bd.Shape.index() == (int)Math::ShapeType::Circle) {
      auto& circle = std::get<Math::CircleF>(bd.Shape);
      DrawCircle(circle.Center(), circle.Radius(), 30,
                 {static_cast<sf::Uint8>(bd.Color.r),
                  static_cast<sf::Uint8>(bd.Color.g),
                  static_cast<sf::Uint8>(bd.Color.b),
                  static_cast<sf::Uint8>(bd.Color.a)});
    } else if (bd.Shape.index() == (int)Math::ShapeType::Rectangle) {
      auto& rect = std::get<Math::RectangleF>(bd.Shape);
      if (!bd.Filled) {
        DrawRectangleBorder(rect.MinBound(), rect.MaxBound(),
                            {static_cast<sf::Uint8>(bd.Color.r),
                             static_cast<sf::Uint8>(bd.Color.g),
                             static_cast<sf::Uint8>(bd.Color.b),
                             static_cast<sf::Uint8>(bd.Color.a)});
      } else {
        DrawRectangle(rect.MinBound(), rect.MaxBound(),
                      {static_cast<sf::Uint8>(bd.Color.r),
                       static_cast<sf::Uint8>(bd.Color.g),
                       static_cast<sf::Uint8>(bd.Color.b),
                       static_cast<sf::Uint8>(bd.Color.a)});
      }
    } else if (bd.Shape.index() == (int)Math::ShapeType::Polygon) {
      auto& polygon = std::get<Math::PolygonF>(bd.Shape);
      DrawPolygon(polygon.Vertices(), {static_cast<sf::Uint8>(bd.Color.r),
                                       static_cast<sf::Uint8>(bd.Color.g),
                                       static_cast<sf::Uint8>(bd.Color.b),
                                       static_cast<sf::Uint8>(bd.Color.a)});
    }
  }
}
