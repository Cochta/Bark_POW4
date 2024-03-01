#include "SFMLApp.h"

#ifdef TRACY_ENABLE
#include "Tracy.hpp"
#include "TracyC.h"
#endif

void SFMLApp::SetUp() {
  _window.create(sf::VideoMode(Width, Height), Title);
  ImGui::SFML::Init(_window);

  _sampleManager.SetUp();
}

void SFMLApp::TearDown() const noexcept { ImGui::SFML::Shutdown(); }

void SFMLApp::Run() noexcept {
  bool quit = false;

  bool adjustWindow = true;

  sf::Event e;

  while (!quit) {
    while (_window.pollEvent(e)) {
      ImGui::SFML::ProcessEvent(e);
      switch (e.type) {
        case sf::Event::Closed:
          quit = true;
          break;
        case sf::Event::KeyReleased:
          switch (e.key.code) {

            case sf::Keyboard::Space:
              _sampleManager.RegenerateSample();
              break;
          }
          break;
        case sf::Event::MouseButtonReleased:
          if (e.mouseButton.button == sf::Mouse::Left) {
            _sampleManager.GiveLeftMouseClickToSample();
          } else if (e.mouseButton.button == sf::Mouse::Right) {
            _sampleManager.GiveRightMouseClickToSample();
          }
          break;
      }
    }

    ImGui::SFML::Update(_window, _deltaClock.restart());

    if (adjustWindow) {
      ImGui::SetNextWindowSize(ImVec2(Metrics::Width / 3, Metrics::Height / 5));
      adjustWindow = false;
    }

    ImGui::Begin("Sample Manager");



    ImGui::Spacing();

    ImGui::TextWrapped(
        _sampleManager.GetSampleDescription(_sampleManager.GetCurrentIndex())
            .c_str());

    ImGui::Spacing();

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() -
                         (ImGui::GetFrameHeightWithSpacing()));


    ImGui::SameLine();

    if (ImGui::Button("Regenerate")) {
      _sampleManager.RegenerateSample();
    }


    ImGui::End();

    _window.clear(sf::Color::Black);

    sf::Vector2i sfMousePos = sf::Mouse::getPosition(_window);
    MousePos = {sfMousePos.x, sfMousePos.y};
    _sampleManager.GiveMousePositionToSample(
        {(float)MousePos.X, (float)MousePos.Y});
    _sampleManager.UpdateSample();

    DrawAllGraphicsData();

    ImGui::SFML::Render(_window);

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
  for (auto& bd : _sampleManager.GetSampleData()) {
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
