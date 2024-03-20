#pragma once

#include "Random.h"
#include "Scene.h"

enum class GameColor { None = -1, Red = 0, Yellow = 1 };

struct GameBoard {
  std::map<std::pair<int, int>, GameColor> board;
  GameBoard() {
    for (int x = 0; x < 8; ++x) {
      for (int y = 0; y < 7; ++y) {
        board[{x, y}] = GameColor::None;
      }
    }
  }
};

class Game : public Scene, public ContactListener {
 private:
  static constexpr float SPEED = 500;

  GameBoard _gb;

  bool _draw = false;
  bool _hasGameEnded = false;
  bool _isLobbyDestroyed = false;
  bool _hasP1Won = false;
  bool _hasP2Won = false;
  bool _isHoverButton = false;

  sf::RectangleShape _rectPutBall =
      sf::RectangleShape({Metrics::Width / 30.f, Metrics::Width / 30.f});

  sf::RectangleShape _rectGameState =
      sf::RectangleShape({Metrics::Width / 2.f, Metrics::Height / 10.f});
  sf::FloatRect _textBounds;
  sf::Text _textGameState;
  sf::Font _font;

 public:
  std::string GetName() noexcept override;
  std::string GetDescription() noexcept override;

  void OnTriggerEnter(ColliderRef col1, ColliderRef col2) noexcept override {}

  void OnTriggerExit(ColliderRef col1, ColliderRef col2) noexcept override {}

  void OnCollisionEnter(ColliderRef col1, ColliderRef col2) noexcept override;

  void OnCollisionExit(ColliderRef col1, ColliderRef col2) noexcept override;

  void CreateBall(Math::Vec2F position, int index) noexcept override;
  void StartConnection(bool isConnectedToServer) noexcept override {}
  bool UpdateQuitButton() override;
  void OtherPlayerHasSurrendered() noexcept override;

 protected:
  void SceneSetUp() noexcept override;

  void SceneUpdate() noexcept override;

  void SceneTearDown() noexcept override;

 private:
  bool CheckVictory(int x, int y) noexcept;
  void DrawGameState();
};