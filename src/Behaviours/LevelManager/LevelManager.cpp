#include "LevelManager.h"
#include "Data.h"
#include <Crow2D/GameObject.h>
#include <Crow2D/dataObjects/Vectors.h>
#include <cstdlib>
#include <string>
#include <vector>

namespace FOUL::Behaviours {
using namespace Crow2D;
using namespace Crow2D::Components;
using namespace Crow2D::Types;

LevelManager *LevelManager::Singleton = nullptr;
float LevelManager::levelSpeed = 7;
std::unordered_map<Crow2D::GameObject *, PlatformType> LevelManager::platforms;
int LevelManager::points;


void LevelManager::SetupSingleton() {
  // #region SetupSingleton
  if (Singleton != nullptr && Singleton != this) {
    Destroy(this);
    return;
  }
  Singleton = this;
  // #endregion
}

void LevelManager::Awake() {
  // #region Awake
  SetupSingleton();
  std::srand(time(NULL));
  // #endregion
}

void LevelManager::Start() { gameOver = false; }

void LevelManager::Update() {
  // #region Update
  if (gameOver) return;
  currentTimer += Time::deltaTime;
  if (currentTimer < currentWaitTime) return;

  for (auto &[platform, _] : platforms) {
    platform->transform->position += Vector3(0, -2, 0);
    if (platform->transform->position.get().y <= Data::PaddleY) {
      gameOver = true;
    }
  }
  currentTimer = 0;

  SpawnNormalRow();
  // #endregion
}

void LevelManager::Reset() {
  // #region Reset
  levelSpeed = 7;
  currentWaitTime = levelSpeed;
  currentTimer = currentWaitTime;
  // #endregion
}

void LevelManager::DestroyPlatform(Crow2D::GameObject *platform) {
  // #region DestroyPlatform
  if (!platform) return;
  Destroy(*platform);
  auto it = platforms.find(platform);
  if (it == platforms.end()) return;
  platforms.erase(it);
  // #endregion
}

void LevelManager::SpawnNormalRow() {
  // #region SpawnNormalRow
  const int nOfPlatforms =
      MinPlatformsPerRow + std::rand() % (MaxPlatformsPerRow - MinPlatformsPerRow);

  const float Gap = 0.7f;

  for (short i = 0; i <= nOfPlatforms; i++) {
    nextPlatform++;
    GameObject &platform =
        gameObject->CreateChild(("Platform_" + std::to_string(nextPlatform)).c_str());

    BoxCollider &col = platform.AddComponent<BoxCollider>(Vector2(PlatformWidth, 0.5f));
    col.isTrigger = true;
    platform.AddComponent<Renderer>(Primitives::Square, Vector2(PlatformWidth, 0.5f));

    Vector3 pos((float)i * (PlatformWidth + Gap) -
                    (float)(nOfPlatforms * (PlatformWidth + Gap)) / 2,
                SpawnY, 0);
    platform.transform->position = pos;

    platforms[&platform] = PlatformType::Normal;
  }
  // #endregion
}


} // namespace FOUL::Behaviours
