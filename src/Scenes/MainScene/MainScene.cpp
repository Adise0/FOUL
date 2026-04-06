

#include "MainScene.h"
#include "Data.h"
#include "PlayerController.h"
#include <Crow2D/Crow2D.h>
#include <Crow2D/GameObject.h>

namespace FOUL {

using namespace Crow2D;
using namespace Crow2D::Scenes;
using namespace Crow2D::Components;
using namespace Crow2D::Types;
using namespace Crow2D::Rendering;


MainScene::MainScene() : Scene("Main Scene") {}
void MainScene::Load() {
  // #region Load
  LoadBackground();
  LoadPlayer();
  // #endregion
}

void MainScene::LoadBackground() {
  // #region LoadBackground
  GameObject &global = rootGameObject->CreateChild("Background");

  Camera &cam = global.AddComponent<Camera>();
  cam.size = 20;

  Sprite *bg = new Sprite("sprites/FieldBackground.png");
  global.AddComponent<Renderer>(bg, WindowManager::resolution / cam.screenPpu);
  // #endregion
}


void MainScene::LoadPlayer() {
  // #region LoadPlayer
  GameObject &playerControllderGO = rootGameObject->CreateChild("PlayerController");
  playerControllderGO.AddComponent<Behaviours::PlayerController>();
  playerControllderGO.transform->position = Vector3(0, Data::PaddleY, 0);
  // #endregion
}
} // namespace FOUL
