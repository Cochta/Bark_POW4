#pragma once

#include <string>

namespace Metrics  // Meter in physical world != meter irl
{
constexpr float MeterRatio = 100.f;

[[nodiscard]] constexpr float PixelsToMeters(float pixels) noexcept {
  return pixels / MeterRatio;
}

[[nodiscard]] constexpr float MetersToPixels(float meters) noexcept {
  return meters * MeterRatio;
}

constexpr int Width = MetersToPixels(12);
constexpr int Height = MetersToPixels(8);
}  // namespace Metrics

struct PlayerData {
  std::string name;
  int elo;
  void clear() {
    name = "";
    elo = 0;
  }
};