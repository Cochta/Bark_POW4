#pragma once

#include "Scene.h"

class Menu : public Scene {
 private:
  static constexpr float SPEED = 500;

  bool _isHoverButton = false;
  bool _isWaitingForConnection = false;
  bool _isConnectedToServer = false;

  sf::RectangleShape _rectStart =
      sf::RectangleShape({Metrics::Width / 3.f, Metrics::Height / 4.f});
  sf::RectangleShape _rectPlayerName =
      sf::RectangleShape({Metrics::Width / 3.f, Metrics::Height / 6.f});
  sf::Text _textStart;
  sf::Text _textPlayerName;
  sf::Font _font;

  std::string _playerName;

 public:
  std::string GetName() noexcept override;
  std::string GetDescription() noexcept override;
  void CreateBall(Math::Vec2F position, int index) noexcept override{};
  void StartConnection(bool isConnectedToServer) noexcept override;
  bool UpdateQuitButton() override;
  void OtherPlayerHasSurrendered() noexcept override{};
  void DrawPlayerData(std::string_view name, int elo, PlayerData p1,
                      PlayerData p2);

 protected:
  void SceneSetUp() noexcept override;

  void SceneUpdate() noexcept override;

  void SceneTearDown() noexcept override;

 private:
};