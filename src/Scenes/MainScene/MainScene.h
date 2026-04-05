#pragma once

#include <Crow2D/Crow2D.h>

namespace FOUL {

class MainScene : public Crow2D::Scenes::Scene {

public:
  MainScene();
  void Load() override;
};
} // namespace FOUL
