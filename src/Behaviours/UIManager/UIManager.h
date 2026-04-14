#pragma once

#include <Crow2D/Crow2D.h>
#include <vector>

namespace FOUL::Behaviours {


class UIManager : public Crow2D::Components::Behaviour {
  // #region Data
public:
  Crow2D::Components::UIRenderer *pauseRenderer = nullptr;
  Crow2D::Components::UIRenderer *mainRenderer = nullptr;

private:
  // #endregion

  // #region Crow2D
private:
  void Awake() override;
  // void Update() override;
  // #endregion

  // #region Methods
public:
  void UpdatePoints(const int &points);
  void Respawn(const int &respawn);
  void SetPause(const bool &pause, const bool &isTutorial = false);

private:
  static void OnQuit(const std::string &type, const std::string &payload);
  static void OnPlay(const std::string &type, const std::string &name);


  // #endregion
};
} // namespace FOUL::Behaviours
