

#include "MainScene.h"
#include "Ball.h"
#include "Data.h"
#include "LevelManager.h"
#include "PlayerController.h"
#include "UIManager.h"


namespace FOUL {

using namespace Crow2D;
using namespace Crow2D::Scenes;
using namespace Crow2D::Components;
using namespace Crow2D::Types;
using namespace Crow2D::Rendering;
using namespace Behaviours;

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



  global.AddComponent<Renderer>(Primitives::Square, WindowManager::resolution / cam.screenPpu,
                                SDL_Color{94, 159, 57, 255});

  LevelManager &levelManager = global.AddComponent<LevelManager>();
  global.AddComponent<UIRenderer>("ui/InGame/InGame.html");
  UIManager &uiManager = global.AddComponent<UIManager>();

  UIRenderer &mainRenderer = global.AddComponent<UIRenderer>("ui/InGame/InGame.html");
  UIRenderer &pauseRenderer = global.AddComponent<UIRenderer>("ui/Pause/Pause.html");
  uiManager.mainRenderer = &mainRenderer;
  uiManager.pauseRenderer = &pauseRenderer;

  global.transform->position -= Vector3(0, 0, 100);



  // #endregion
}


void MainScene::LoadPlayer() {
  // #region LoadPlayer
  GameObject &playerControllderGO = rootGameObject->CreateChild("PlayerController");
  PlayerController &playerController = playerControllderGO.AddComponent<PlayerController>();
  playerControllderGO.transform->position = Vector3(0, Data::PaddleY, 0);


  // #endregion
}
} // namespace FOUL
