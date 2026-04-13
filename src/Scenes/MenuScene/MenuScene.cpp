

#include "MenuScene.h"

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
  UIRenderer &uiRenderer = uiHolderGO.AddComponent<UIRenderer>("index.html");
}
} // namespace FOUL

