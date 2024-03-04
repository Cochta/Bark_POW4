#pragma once

#include "Random.h"
#include "Scene.h"

class Game : public Scene, public ContactListener {
 private:
  static constexpr float SPEED = 500;

 public:
  std::string GetName() noexcept override;
  std::string GetDescription() noexcept override;

  void OnTriggerEnter(ColliderRef col1, ColliderRef col2) noexcept override{};

  void OnTriggerExit(ColliderRef col1, ColliderRef col2) noexcept override{};

  void OnCollisionEnter(ColliderRef col1, ColliderRef col2) noexcept override;

  void OnCollisionExit(ColliderRef col1, ColliderRef col2) noexcept override;

  void CreateBall(Math::Vec2F position) noexcept;

 protected:
  void SceneSetUp() noexcept override;

  void SceneUpdate() noexcept override;

  void SceneTearDown() noexcept override;

 private:
};