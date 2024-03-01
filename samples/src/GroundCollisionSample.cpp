#include "GroundCollisionSample.h"

std::string GroundCollisionSample::GetName() noexcept {
  return "Bouncing Ground";
}

std::string GroundCollisionSample::GetDescription() noexcept {
  return "CONTROLS: Left click to create a circle, right click to create a "
         "rectangle.\n\nFloor has 1 x bounciness, Circles and rectangles have "
         "0 x bounciness";
}

void GroundCollisionSample::OnCollisionEnter(ColliderRef col1,
                                             ColliderRef col2) noexcept {}

void GroundCollisionSample::OnCollisionExit(ColliderRef col1,
                                            ColliderRef col2) noexcept {}

void GroundCollisionSample::SampleSetUp() noexcept {
  _world.SetContactListener(this);
  GraphicsData gd;
  gd.Color = {0, 0, 255};
  // Create static rectangle
  const auto groundRef = _world.CreateBody();
  _bodyRefs.push_back(groundRef);
  auto& groundBody = _world.GetBody(groundRef);
  groundBody.Type = BodyType::STATIC;
  groundBody.Mass = 1;

  groundBody.Position = {Metrics::Width / 2.f,
                         Metrics::Height - Metrics::Height / 5.f};

  const auto groundColRef = _world.CreateCollider(groundRef);
  _colRefs.push_back(groundColRef);
  auto& groundCol = _world.GetCollider(groundColRef);
  groundCol.Shape =
      Math::RectangleF({-Metrics::Width / 3.f, 0.f},
                       {Metrics::Width / 3.f, Metrics::MetersToPixels(0.2f)});
  groundCol.BodyPosition = groundBody.Position;
  groundCol.Restitution = 0.f;

  AllGraphicsData.emplace_back(gd);

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
    wallCol.Shape = Math::RectangleF(
        {-Metrics::MetersToPixels(0.1f), -Metrics::Width / 4.f},
        {Metrics::MetersToPixels(0.1f), Metrics::Width / 4.f});
    wallCol.BodyPosition = wallBody.Position;
    wallCol.Restitution = 0.f;

    AllGraphicsData.emplace_back(gd);

    // Create static rectangle
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
    roofCol.Shape =
        Math::CircleF(Math::Vec2F::Zero(), Metrics::MetersToPixels(0.1f));
    roofCol.BodyPosition = roofBody.Position;
    roofCol.Restitution = 0.f;

    AllGraphicsData.emplace_back(gd);
  }
}
void GroundCollisionSample::SampleUpdate() noexcept {
  if (_mouseLeftReleased) {
    CreateBall(_mousePos);
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
        if (i != 0) {
          _world.GetBody(col.BodyRef).ApplyForce({0, SPEED});
        }
        AllGraphicsData[i].Shape =
            std::get<Math::RectangleF>(shape) + col.BodyPosition;
        break;
      default:
        break;
    }
  }
}

void GroundCollisionSample::SampleTearDown() noexcept {}

void GroundCollisionSample::CreateBall(Math::Vec2F position) noexcept {
  isPlayerOne = !isPlayerOne;

  const auto circleBodyRef = _world.CreateBody();
  _bodyRefs.push_back(circleBodyRef);
  auto& circleBody = _world.GetBody(circleBodyRef);

  circleBody.Mass = 1;

  circleBody.Position = position;

  const auto circleColRef = _world.CreateCollider(circleBodyRef);
  _colRefs.push_back(circleColRef);
  auto& circleCol = _world.GetCollider(circleColRef);
  circleCol.Shape = Math::Circle(Math::Vec2F::Zero(), 40.f);
  circleCol.BodyPosition = circleBody.Position;
  circleCol.Restitution = 0.f;

  GraphicsData gd;
  if (isPlayerOne) {
    gd.Color = {255, 255, 0};
  } else {
    gd.Color = {255, 0, 0};
  }

  AllGraphicsData.emplace_back(gd);
}
