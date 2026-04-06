

#include "MainScene.h"
#include <Crow2D/GameObject.h>
#include <Crow2D/Scene.h>
#include <Crow2D/WindowManager.h>
#include <Crow2D/components/Camera.h>
namespace FOUL {

using namespace Crow2D;
using namespace Crow2D::Scenes;
using namespace Crow2D::Components;
using namespace Crow2D::Types;
using namespace Crow2D::Rendering;


MainScene::MainScene() : Scene("Main Scene") {}
void MainScene::Load() {
  // #region Load


  GameObject &global = rootGameObject->CreateChild("Background");

  Camera &cam = global.AddComponent<Camera>();
  cam.size = 20;

  Sprite *bg = new Sprite("sprites/FieldBackground.png");
  global.AddComponent<Renderer>(bg, WindowManager::resolution / cam.screenPpu);

  // #endregion
}
} // namespace FOUL
