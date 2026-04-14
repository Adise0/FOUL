#pragma once
#include <Crow2D/Crow2D.h>

namespace FOUL {
struct Data {
public:
  static constexpr float XLimit = 15;
  static constexpr float PaddleY = -8;
  static constexpr float xPerPlayer = 0.7f;

  static inline Crow2D::Scenes::Scene *menuScene = nullptr;
  static inline Crow2D::Scenes::Scene *mainScene = nullptr;
};
} // namespace FOUL
