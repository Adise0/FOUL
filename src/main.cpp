

#include "Data.h"
#include "Scenes/MenuScene/MenuScene.h"
#include <Crow2D/Crow2D.h>
#include <cstdio>

using namespace FOUL;
using namespace Crow2D;
int main() {
  std::srand(time(NULL));
  Config::Init("FOUL!");

  MenuScene *menuScene = new MenuScene();
  Data::menuScene = menuScene;
  Engine::Run(*menuScene);
  printf("Bye bye!\n");
}
