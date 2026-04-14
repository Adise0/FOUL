#pragma once

#include <Crow2D/Crow2D.h>
#include <vector>

namespace FOUL::Behaviours {


class UIManager : public Crow2D::Components::Behaviour {
  // #region Data
private:
  Crow2D::Components::UIRenderer *renderer = nullptr;
  // #endregion

  // #region Crow2D
private:
  void Awake() override;
  // void Update() override;
  // #endregion

  // #region Methods
private:
  static void OnQuit(const std::string &type, const std::string &payload);
  static void OnPlay(const std::string &type, const std::string &name);
  // #endregion
};
} // namespace FOUL::Behaviours
