#pragma once

#include <Crow2D/Crow2D.h>

namespace FOUL {

class MainScene : public Crow2D::Scenes::Scene {
  // #region Crow2D
public:
  MainScene();
  void Load() override;
  // #endregion

  // #region Loads
private:
  void LoadBackground();
  void LoadPlayer();
  // #endregion
};
} // namespace FOUL
