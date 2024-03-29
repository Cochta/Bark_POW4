#pragma once

#include <SFML/Graphics.hpp>

#include "Metrics.h"
#include "Timer.h"
#include "World.h"
#include "network_client_manager.h"
#include "packet_manager.h"

// for the graphics renderer to draw
struct GraphicsData {
  std::variant<Math::CircleF, Math::RectangleF, Math::PolygonF> Shape{
      Math::CircleF(Math::Vec2F::Zero(), 1)};
  bool Filled = true;
  sf::Color Color;
};

class Scene {
 public:
  std::vector<GraphicsData> AllGraphicsData;

  bool IsPlayerTurn = false;
  bool IsPlayer1 = false;

  NetworkClientManager *_client;
  sf::RenderWindow *_window;

 protected:
  World _world;

  std::vector<BodyRef> _bodyRefs;
  std::vector<ColliderRef> _colRefs;

  Math::Vec2F _mousePos;

  bool _mouseLeftReleased = false;
  bool _mouseRightReleased = false;

 private:
  Timer _timer;

 public:
  virtual std::string GetName() noexcept = 0;
  virtual std::string GetDescription() noexcept = 0;

  virtual void CreateBall(Math::Vec2F position, int index) noexcept = 0;
  virtual void StartConnection(bool isConnectedToServer) noexcept = 0;
  virtual bool UpdateQuitButton() = 0;
  virtual void OtherPlayerHasSurrendered() noexcept = 0;
  virtual void DrawPlayerData(std::string_view name, int elo, PlayerData p1,
                              PlayerData p2) = 0;

  void SetUp(NetworkClientManager *client, sf::RenderWindow *window) noexcept;

  void TearDown() noexcept;

  void Update() noexcept;

  void GetMousePos(Math::Vec2F mousePos) noexcept;

  void OnLeftClick() noexcept;

  void OnRightClick() noexcept;

  virtual ~Scene() noexcept = default;

 protected:
  virtual void SceneSetUp() noexcept = 0;

  virtual void SceneTearDown() noexcept = 0;

  virtual void SceneUpdate() noexcept = 0;
};
