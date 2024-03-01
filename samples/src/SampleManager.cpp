#include "SampleManager.h"

void SampleManager::SetUp() noexcept
{
    _samples.push_back(std::make_unique<GroundCollisionSample>());

    _samples[_sampleIdx]->SetUp();
}

void SampleManager::UpdateSample() const noexcept
{
    _samples[_sampleIdx]->Update();
}

void SampleManager::RegenerateSample() const noexcept
{
    _samples[_sampleIdx]->TearDown();
    _samples[_sampleIdx]->SetUp();
}

std::vector<GraphicsData> &SampleManager::GetSampleData() const noexcept
{
    return _samples[_sampleIdx]->AllGraphicsData;
}

void SampleManager::GiveMousePositionToSample(const Math::Vec2F mousePosition) const noexcept
{
    _samples[_sampleIdx]->GetMousePos(mousePosition);
}

void SampleManager::GiveLeftMouseClickToSample() const noexcept
{
    _samples[_sampleIdx]->OnLeftClick();
}

void SampleManager::GiveRightMouseClickToSample() const noexcept
{
    _samples[_sampleIdx]->OnRightClick();
}
