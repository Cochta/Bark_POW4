#pragma once

#include <vector>

#include "SceneManager.h"

class SFMLApp {
 private:
  sf::RenderWindow _window;
  sf::Clock _deltaClock;

  SceneManager _sceneManager;
  NetworkClientManager _client = NetworkClientManager(HOST_NAME, PORT);

  std::string _username;

  PlayerData _p1, _p2;

  static constexpr std::int8_t EnterKeyCode_ = 13;
  static constexpr std::int8_t SpaceKeyCode_ = 32;
  static constexpr std::int8_t BackspaceKeyCode_ = 8;

 public:
  std::string Title;
  int Width, Height;
  Math::Vec2I MousePos;

  SFMLApp(sf::String title, sf::Uint32 width, sf::Uint32 height) noexcept
      : _window(sf::VideoMode(width, height), title) {
    Width = width;
    Height = height;
    Title = title;
  }
  void SetUp();

  void TearDown() const noexcept;

  void Run() noexcept;

 private:
  void DrawCircle(Math::Vec2F center, float radius, int segments,
                  const sf::Color &col) noexcept;

  void DrawRectangle(Math::Vec2F minBound, Math::Vec2F maxBound,
                     const sf::Color &col) noexcept;

  void DrawRectangleBorder(Math::Vec2F minBound, Math::Vec2F maxBound,
                           const sf::Color &col) noexcept;

  void DrawPolygon(const std::vector<Math::Vec2F> &vertices,
                   const sf::Color &col);

  void DrawAllGraphicsData() noexcept;

  void OnPacketReceive(const Packet &packet);
};