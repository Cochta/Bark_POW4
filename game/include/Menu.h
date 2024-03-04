#pragma once

#include "Scene.h"

class Menu : public Scene {
 private:
  static constexpr float SPEED = 500;
  bool isPlayerOne = true;

 public:
  std::string GetName() noexcept override;
  std::string GetDescription() noexcept override;
  void CreateBall(Math::Vec2F position) noexcept override{};

 protected:
  void SceneSetUp() noexcept override;

  void SceneUpdate() noexcept override;

  void SceneTearDown() noexcept override;

 private:
};