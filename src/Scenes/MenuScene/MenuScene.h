#pragma once

#include <Crow2D/Crow2D.h>

namespace FOUL {

class MenuScene : public Crow2D::Scenes::Scene {
  // #region Crow2D
public:
  MenuScene();
  void Load() override;
  // #endregion

  // #region Loads
private:
  void LoadUI();
  // #endregion
};
} // namespace FOUL
