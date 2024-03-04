#include "Scene.h"

void Scene::SetUp(Client *client, NetworkClientManager *ncm) noexcept {
    _timer.SetUp();
    _world.SetUp();
    SceneSetUp();
    _client = client;
    _networkClientManager = ncm;
}

void Scene::Update() noexcept
{
    SceneUpdate();
    _timer.Tick();
    _world.Update(_timer.DeltaTime);
    _mouseLeftReleased = false;
    _mouseRightReleased = false;
}

void Scene::TearDown() noexcept
{
    SceneTearDown();
    _bodyRefs.clear();
    AllGraphicsData.clear();
    _colRefs.clear();
    _world.TearDown();
}

void Scene::GetMousePos(const Math::Vec2F mousePos) noexcept
{
    _mousePos = mousePos;
}

void Scene::OnLeftClick() noexcept
{
    _mouseLeftReleased = true;
}

void Scene::OnRightClick() noexcept
{
    _mouseRightReleased = true;
}
