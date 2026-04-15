#include "UIManager.h"
#include "Data.h"
#include "MainScene.h"
#include <Crow2D/SceneManager.h>
#include <stdexcept>
#include <string>

namespace FOUL::Behaviours {
using namespace Crow2D;
using namespace Crow2D::Components;
using namespace Crow2D::Types;
using namespace Crow2D::Inputs;

void UIManager::Awake() {
  // #region Awake

  if (!mainRenderer) throw std::runtime_error("UIManager requires a UIRenderer component");

  mainRenderer->bridge->On("__quit", OnQuit);
  mainRenderer->bridge->On("Play", OnPlay);
  // #endregion
}


void UIManager::OnQuit(const std::string &type, const std::string &payload) { Application::Quit(); }
void UIManager::OnPlay(const std::string &type, const std::string &name) {
  // #region OnPlay
  Data::currentPlayer = name;
  MainScene *mainScene = new MainScene();
  Scenes::SceneManager::SetSceneAsActive(*mainScene);

  if (Data::mainScene) delete Data::mainScene;
  Data::mainScene = mainScene;
  // TODO: add scene object deletion on sceneManager (Crow2D)
  // #endregion
}

void UIManager::UpdatePoints(const int &points) {
  if (!mainRenderer) return;
  mainRenderer->bridge->Send("Points", std::to_string(points));
}
void UIManager::Respawn(const int &respawn) {
  if (!mainRenderer) return;
  mainRenderer->bridge->Send("Respawn", std::to_string(respawn));
}

void UIManager::SetPause(const bool &pause, const bool &isTutorial) {
  if (!pauseRenderer) return;
  if (pause) pauseRenderer->bridge->Send("Enable", std::to_string(isTutorial));
  else pauseRenderer->bridge->Send("Disable");
}

} // namespace FOUL::Behaviours
