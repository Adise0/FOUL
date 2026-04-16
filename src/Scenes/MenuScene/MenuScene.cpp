

#include "MenuScene.h"
#include "LevelManager.h"
#include "UIManager.h"
#include <Crow2D/Crow2dUtils.h>
#include <Crow2D/components/Renderer.h>
#include <Crow2D/dataObjects/Sprite.h>
#include <cstdio>

namespace FOUL {

using namespace Crow2D;
using namespace Crow2D::Scenes;
using namespace Crow2D::Components;
using namespace Crow2D::Types;
using namespace Crow2D::Rendering;
using namespace Behaviours;


MenuScene::MenuScene() : Scene("Menu Scene") {}

void MenuScene::Load() { LoadUI(); }

void MenuScene::LoadUI() {

  GameObject &uiHolderGO = rootGameObject->CreateChild("UI Holder");
  Camera &cam = uiHolderGO.AddComponent<Camera>();
  cam.SetAsActiveCamera();
  UIRenderer &mainRenderer = uiHolderGO.AddComponent<UIRenderer>("ui/MainMenu/MainMenu.html");

  UIManager &uiManager = uiHolderGO.AddComponent<UIManager>();
  uiManager.mainRenderer = &mainRenderer;
}
} // namespace FOUL

