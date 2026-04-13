

#include "MenuScene.h"
#include "LevelManager.h"
#include <Crow2D/Crow2dUtils.h>
#include <Crow2D/dataObjects/Sprite.h>
#include <cstdio>

namespace FOUL {

using namespace Crow2D;
using namespace Crow2D::Scenes;
using namespace Crow2D::Components;
using namespace Crow2D::Types;
using namespace Crow2D::Rendering;


MenuScene::MenuScene() : Scene("Menu Scene") {}

void MenuScene::Load() { LoadUI(); }

void MenuScene::LoadUI() {

  GameObject &uiHolderGO = rootGameObject->CreateChild("UI Holder");
  Camera &cam = uiHolderGO.AddComponent<Camera>();
  UIRenderer &uiRenderer = uiHolderGO.AddComponent<UIRenderer>("ui/MainMenu/MainMenu.html");

  uiHolderGO.AddComponent<Renderer>(Primitives::Square, Vector2(1, 1));
}
} // namespace FOUL

