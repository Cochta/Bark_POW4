#include "Game.h"

std::string Game::GetName() noexcept { return "Bouncing Ground"; }

std::string Game::GetDescription() noexcept {
  return "CONTROLS: Left click to create a circle, right click to create a "
         "rectangle.\n\nFloor has 1 x bounciness, Circles and rectangles have "
         "0 x bounciness";
}

void Game::OnCollisionEnter(ColliderRef col1, ColliderRef col2) noexcept {}

void Game::OnCollisionExit(ColliderRef col1, ColliderRef col2) noexcept {}

void Game::SceneSetUp() noexcept {
  _world.SetContactListener(this);
  GraphicsData gd;
  gd.Color = {0, 0, 255};
  // Create static rectangle
  for (size_t i = 0; i < 7; i++) {
    const auto groundRef = _world.CreateBody();
    _bodyRefs.push_back(groundRef);
    auto& groundBody = _world.GetBody(groundRef);
    groundBody.Type = BodyType::STATIC;
    groundBody.Mass = 1;

    groundBody.Position = {
        Metrics::Width / 2.f,
        Metrics::Height - Metrics::Height / 5.f - i * Metrics::Width / 15.f};

    const auto groundColRef = _world.CreateCollider(groundRef);
    _colRefs.push_back(groundColRef);
    auto& groundCol = _world.GetCollider(groundColRef);
    if (i != 0) {
      groundCol.IsTrigger = true;
    }
    auto rect = Math::RectangleF({-Metrics::Width / 3.f, 0.f},
                                 {Metrics::Width / 3.f, Metrics::Width / 60.f});
    /*rect.*/
    groundCol.Shape = rect;

    groundCol.BodyPosition = groundBody.Position;
    groundCol.Restitution = 0.f;

    AllGraphicsData.emplace_back(gd);
  }

  for (int i = -4; i < 5; ++i) {
    // Create static rectangle
    const auto wallRef = _world.CreateBody();
    _bodyRefs.push_back(wallRef);
    auto& wallBody = _world.GetBody(wallRef);
    wallBody.Type = BodyType::STATIC;
    wallBody.Mass = 1;

    wallBody.Position = {Metrics::Width / 2.f + Metrics::Width / 12.f * i,
                         Metrics::Height / 2.f};

    const auto wallColRef = _world.CreateCollider(wallRef);
    _colRefs.push_back(wallColRef);
    auto& wallCol = _world.GetCollider(wallColRef);
    wallCol.Shape =
        Math::RectangleF({-Metrics::Width / 120.f, -Metrics::Width / 4.f},
                         {Metrics::Width / 120.f, Metrics::Width / 4.f});
    wallCol.BodyPosition = wallBody.Position;
    wallCol.Restitution = 0.f;

    AllGraphicsData.emplace_back(gd);

    // Create static circles
    const auto roofRef = _world.CreateBody();
    _bodyRefs.push_back(roofRef);
    auto& roofBody = _world.GetBody(roofRef);
    roofBody.Type = BodyType::STATIC;
    roofBody.Mass = 1;

    roofBody.Position = {Metrics::Width / 2.f + Metrics::Width / 12.f * i,
                         Metrics::Height / 8.f};

    const auto roofColRef = _world.CreateCollider(roofRef);
    _colRefs.push_back(roofColRef);
    auto& roofCol = _world.GetCollider(roofColRef);
    roofCol.Shape = Math::CircleF(Math::Vec2F::Zero(), Metrics::Width / 120.f);
    roofCol.BodyPosition = roofBody.Position;
    roofCol.Restitution = 0.f;

    AllGraphicsData.emplace_back(gd);
  }
}
void Game::SceneUpdate() noexcept {
  if (_mouseLeftReleased && IsPlayerTurn) {
    CreateBall(_mousePos);
    auto p = new HasPlayedPacket();
    p->IsFirstTurn = false;
    p->X = _mousePos.X;
    p->Y = _mousePos.Y;
    _client->SendPacket(p);
    IsPlayerTurn = false;
  }

  for (std::size_t i = 0; i < _colRefs.size(); ++i) {
    const auto& col = _world.GetCollider(_colRefs[i]);

    const auto& shape = _world.GetCollider(_colRefs[i]).Shape;

    switch (shape.index()) {
      case static_cast<int>(Math::ShapeType::Circle):
        _world.GetBody(col.BodyRef).ApplyForce({0, SPEED});
        AllGraphicsData[i].Shape =
            std::get<Math::CircleF>(shape) + col.BodyPosition;
        break;
      case static_cast<int>(Math::ShapeType::Rectangle):

        AllGraphicsData[i].Shape =
            std::get<Math::RectangleF>(shape) +
            Math::Vec2F(
                col.BodyPosition.X,
                col.BodyPosition.Y - (i < 7 ? Metrics::Width / 120.f : 0));
        break;
      default:
        break;
    }
  }
}

void Game::SceneTearDown() noexcept {}

void Game::CreateBall(Math::Vec2F position) noexcept {
  const auto circleBodyRef = _world.CreateBody();
  _bodyRefs.push_back(circleBodyRef);
  auto& circleBody = _world.GetBody(circleBodyRef);

  circleBody.Mass = 1;

  circleBody.Position = position;

  const auto circleColRef = _world.CreateCollider(circleBodyRef);
  _colRefs.push_back(circleColRef);
  auto& circleCol = _world.GetCollider(circleColRef);
  circleCol.Shape = Math::Circle(Math::Vec2F::Zero(), Metrics::Width / 30.f);
  circleCol.BodyPosition = circleBody.Position;
  circleCol.Restitution = 0.f;

  GraphicsData gd;
  if (IsPlayer1) {
    gd.Color = {255, 255, 0};
  } else {
    gd.Color = {255, 0, 0};
  }

  AllGraphicsData.emplace_back(gd);
}
