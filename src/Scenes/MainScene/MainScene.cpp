

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

  Sprite *bg = new Sprite("sprites/FieldBackground.png");
  global.AddComponent<Renderer>(bg, WindowManager::resolution / cam.screenPpu);

  global.AddComponent<LevelManager>();
  // #endregion
}


void MainScene::LoadPlayer() {
  // #region LoadPlayer
  GameObject &playerControllderGO = rootGameObject->CreateChild("PlayerController");
  PlayerController &playerController = playerControllderGO.AddComponent<PlayerController>();
  playerControllderGO.transform->position = Vector3(0, Data::PaddleY, 0);

  GameObject &ballGO = rootGameObject->CreateChild("Ball");
  Ball &ball = ballGO.AddComponent<Ball>();
  ballGO.AddComponent<Renderer>(Primitives::Circle, Vector2(0.5f, 0.5f));
  CircleCollider &ballCollider = ballGO.AddComponent<CircleCollider>(0.25f);
  ballCollider.drawGizmos = true;
  RigidBody &ballRB = ballGO.AddComponent<RigidBody>();
  ballRB.collisionMode = CollisionMode::Continuous;

  playerController.balls.push_back(&ball);
  // #endregion
}
} // namespace FOUL
