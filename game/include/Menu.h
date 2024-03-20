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
  sf::Text _textStart;
  sf::Font _font;

 public:
  std::string GetName() noexcept override;
  std::string GetDescription() noexcept override;
  void CreateBall(Math::Vec2F position, int index) noexcept override{};
  void StartConnection(bool isConnectedToServer) noexcept override;
  bool UpdateQuitButton() override;
  void OtherPlayerHasSurrendered() noexcept override{};

 protected:
  void SceneSetUp() noexcept override;

  void SceneUpdate() noexcept override;

  void SceneTearDown() noexcept override;

 private:
};