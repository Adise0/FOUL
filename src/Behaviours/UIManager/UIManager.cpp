#include "UIManager.h"
#include "Data.h"
#include "MainScene.h"
#include <Crow2D/SceneManager.h>
#include <stdexcept>

namespace FOUL::Behaviours {
using namespace Crow2D;
using namespace Crow2D::Components;
using namespace Crow2D::Types;
using namespace Crow2D::Inputs;

void UIManager::Awake() {
  // #region Awake
  renderer = gameObject->GetComponent<UIRenderer>();
  if (!renderer) throw std::runtime_error("UIManager requires a UIRenderer component");

  renderer->bridge->On("__quit", OnQuit);
  renderer->bridge->On("Play", OnPlay);
  // #endregion
}


void UIManager::OnQuit(const std::string &type, const std::string &payload) { Application::Quit(); }
void UIManager::OnPlay(const std::string &type, const std::string &name) {
  // #region OnPlay
  Data::currentPlayer = name;
  MainScene *mainScene = new MainScene();
  Scenes::SceneManager::SetSceneAsActive(*mainScene);

  Data::menuScene = nullptr;
  Data::mainScene = mainScene;
  // TODO: add scene object deletion on sceneManager (Crow2D)
  // #endregion
}

void UIManager::UpdatePoints(const int &points) {
  if (!renderer) return;
  renderer->bridge->Send("Points", std::to_string(points));
}

} // namespace FOUL::Behaviours
