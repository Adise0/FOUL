

#include "MainScene.h"
#include <Crow2D/Crow2D.h>
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
  global.AddComponent<Renderer>(Primitives::Square, Vector2(1, 1));
  // #endregion
}
} // namespace FOUL
