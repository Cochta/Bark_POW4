#pragma once

#include "World.h"
#include "Metrics.h"

#include "Timer.h"

struct Color
{
    int r = 255, g = 255, b = 255, a = 255;
};
// for the graphics renderer to draw
struct GraphicsData
{
    std::variant<Math::CircleF, Math::RectangleF, Math::PolygonF> Shape{
            Math::CircleF(Math::Vec2F::Zero(), 1)};
    bool Filled = true;
    Color Color;
};

class Sample
{
public:
    std::vector<GraphicsData> AllGraphicsData;

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

    void SetUp() noexcept;

    void TearDown() noexcept;

    void Update() noexcept;

    void GetMousePos(Math::Vec2F mousePos) noexcept;

    void OnLeftClick() noexcept;

    void OnRightClick() noexcept;

    virtual ~Sample() noexcept = default;

protected:
    virtual void SampleSetUp() noexcept = 0;

    virtual void SampleTearDown() noexcept = 0;

    virtual void SampleUpdate() noexcept = 0;
};

