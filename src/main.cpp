

#include "Scenes/MainScene/MainScene.h"
#include <Crow2D/Crow2D.h>
#include <cstdio>

using namespace FOUL;
using namespace Crow2D;
int main() {

  Config::Init("FOUL!");
  MainScene *mainScene = new MainScene();

  Engine::Run(*mainScene);
}
