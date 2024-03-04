#include "SceneManager.h"

#include "Game.h"
#include "Menu.h"
void SceneManager::SetUp(Client* client, NetworkClientManager* ncm) noexcept {
    _Scenes.push_back(std::make_unique<Menu>());
    _Scenes.push_back(std::make_unique<Game>());

    _Scenes[_SceneIdx]->SetUp(client,ncm);
}

void SceneManager::ChangeScene(int idx, Client* client,
                               NetworkClientManager* ncm) noexcept {
    _Scenes[_SceneIdx]->TearDown();
    _SceneIdx = idx;
    _Scenes[_SceneIdx]->SetUp(client, ncm);
}

void SceneManager::UpdateScene() const noexcept
{
    _Scenes[_SceneIdx]->Update();
}

std::vector<GraphicsData> &SceneManager::GetSceneData() const noexcept
{
    return _Scenes[_SceneIdx]->AllGraphicsData;
}

void SceneManager::GiveMousePositionToScene(const Math::Vec2F mousePosition) const noexcept
{
    _Scenes[_SceneIdx]->GetMousePos(mousePosition);
}

void SceneManager::GiveLeftMouseClickToScene() const noexcept
{
    _Scenes[_SceneIdx]->OnLeftClick();
}

void SceneManager::GiveRightMouseClickToScene() const noexcept
{
    _Scenes[_SceneIdx]->OnRightClick();
}
