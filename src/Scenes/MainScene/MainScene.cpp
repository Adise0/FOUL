

#include "MainScene.h"
#include "Ball.h"
#include "Data.h"
#include "LevelManager.h"
#include "PlayerController.h"
#include <Crow2D/Crow2D.h>
#include <Crow2D/GameObject.h>
#include <Crow2D/components/Renderer.h>
#include <Crow2D/components/RigidBody.h>
#include <Crow2D/components/colliders/CircleCollider.h>
#include <Crow2D/dataObjects/Sprite.h>
#include <Crow2D/dataObjects/Vectors.h>

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

  global.AddComponent<LevelManager>();
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
