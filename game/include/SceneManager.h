#pragma once

#include "Scene.h"

class SceneManager {
 private:
  std::vector<std::unique_ptr<Scene>> _Scenes;

 public:
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

  void SetPlayerTurn() { _Scenes[_SceneIdx]->IsPlayerTurn = true; }

  void SetUp(Client* client, NetworkClientManager* ncm) noexcept;

  void UpdateScene() const noexcept;

  void ChangeScene(int idx, Client* client, NetworkClientManager* ncm) noexcept;

  [[nodiscard]] std::vector<GraphicsData>& GetSceneData() const noexcept;

  void GiveMousePositionToScene(Math::Vec2F mousePosition) const noexcept;

  void GiveLeftMouseClickToScene() const noexcept;

  void GiveRightMouseClickToScene() const noexcept;
};