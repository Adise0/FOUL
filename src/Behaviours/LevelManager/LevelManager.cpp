#include "LevelManager.h"
#include "Ball.h"
#include "Data.h"
#include "PlayerController.h"
#include <Crow2D/GameObject.h>
#include <Crow2D/dataObjects/Vectors.h>
#include <cstdlib>
#include <stdexcept>
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
short LevelManager::freeBalls = 3;


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

void LevelManager::Start() {
  gameOver = false;
  SpawnBall(BallType::Normal, Vector2::Zero);
}

void LevelManager::Update() {
  // #region Update
  if (gameOver) return;
  CheckBalls();

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
  freeBalls = 3;
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

void LevelManager::SpawnBall(const BallType &type, const Vector2 &pos,
                             const Crow2D::Types::Vector2 &dir) {
  // #region SpawnBall
  GameObject &ballGO = gameObject->scene->rootGameObject->CreateChild("Ball");
  ballGO.AddComponent<Renderer>(Primitives::Circle, Vector2(0.5f, 0.5f));
  CircleCollider &ballCollider = ballGO.AddComponent<CircleCollider>(0.25f);
  ballCollider.drawGizmos = true;
  RigidBody &ballRB = ballGO.AddComponent<RigidBody>();
  ballRB.collisionMode = CollisionMode::Continuous;

  Ball *ball;
  switch (type) {
  case BallType::Normal:
    ball = &ballGO.AddComponent<Ball>();
    break;
  // case BallType::Fire:
  //   // ballGO.AddComponent<FireBall>();
  //   break;
  default:
    throw std::runtime_error("Unknown ball type " + std::to_string((int)type));
  }

  ball->direction = dir;
  balls.push_back(&ballGO);
  // #endregion
}

void LevelManager::CheckBalls() {
  // #region CheckBall
  bool ballFell = false;
  for (GameObject *ball : balls) {
    if (ball->transform->position.get().y > Data::PaddleY - 1) continue;
    ball->transform->position = Vector3(ball->transform->position.get().x, 0, 0);
    Destroy(*ball);
    PlayerController::Singleton->RemovePlayer();
    ballFell = true;
  }
  if (!ballFell || gameOver) return;

  std::erase_if(balls, [](const GameObject *ball) { return (bool)ball->isDeleted; });
  if (balls.empty()) {
    if (freeBalls == 0) {
      LevelManager::Singleton->gameOver = true;
      return;
    }
    freeBalls--;
    SpawnBall(BallType::Normal, Vector2::Zero);
    // TODO: wait a second
  }
  // #endregion
}


} // namespace FOUL::Behaviours
