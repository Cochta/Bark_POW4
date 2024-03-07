#pragma once

#include "Scene.h"

class SceneManager {
 private:
  std::vector<std::unique_ptr<Scene>> _Scenes;
  std::size_t _SceneIdx = 0;

 public:
  [[nodiscard]] int GetSceneNbr() noexcept { return _Scenes.size(); }
  [[nodiscard]] int GetCurrentIndex() noexcept { return _SceneIdx; }

  [[nodiscard]] std::string GetSceneName(int idx) {
    return _Scenes[idx]->GetName();
  }
  [[nodiscard]] std::string GetSceneDescription(int idx) {
    return _Scenes[idx]->GetDescription();
  }

  void OtherPlayerHasPlayed(bool isFirstTurn, float x, float y, int index) {
    _Scenes[_SceneIdx]->IsPlayerTurn = true;
    if (isFirstTurn) {
      _Scenes[_SceneIdx]->IsPlayer1 = true;
    } else {
      _Scenes[_SceneIdx]->IsPlayer1 = !_Scenes[_SceneIdx]->IsPlayer1;
      _Scenes[_SceneIdx]->CreateBall({x, y}, index);
      _Scenes[_SceneIdx]->IsPlayer1 = !_Scenes[_SceneIdx]->IsPlayer1;
    }
  }

  void SetUp(Client* client, sf::RenderWindow* window) noexcept;

  void UpdateScene() const noexcept;

  void ChangeScene(int idx, Client* client, sf::RenderWindow* window) noexcept;

  [[nodiscard]] std::vector<GraphicsData>& GetSceneData() const noexcept;

  void GiveMousePositionToScene(Math::Vec2F mousePosition) const noexcept;

  void GiveLeftMouseClickToScene() const noexcept;

  void GiveRightMouseClickToScene() const noexcept;

  void StartConnection() const noexcept {
    _Scenes[_SceneIdx]->StartConnection();
  };
};