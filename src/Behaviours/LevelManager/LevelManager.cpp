#include "LevelManager.h"
#include "Ball.h"
#include "Data.h"
#include "PlayerController.h"
#include "Recrut.h"
#include <Crow2D/GameObject.h>
#include <Crow2D/components/colliders/CircleCollider.h>
#include <Crow2D/dataObjects/Vectors.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

namespace FOUL::Behaviours {
using namespace Crow2D;
using namespace Crow2D::Components;
using namespace Crow2D::Types;

static constexpr float DEG2RAD = 3.14159265358979323846f / 180.0f;


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
  fireBallSprite = new Sprite("sprites/FireBall.png", SDL_ScaleMode::SDL_SCALEMODE_PIXELART);
  ballSprite = new Sprite("sprites/Ball.png", SDL_ScaleMode::SDL_SCALEMODE_PIXELART);

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

  SpawnRow();
  // #endregion
}

void LevelManager::Reset() {
  // #region Reset
  levelSpeed = 7;
  currentWaitTime = levelSpeed;
  currentTimer = currentWaitTime;
  // #endregion
}

void LevelManager::HitPlatform(GameObject *platform, Ball *ball) {
  // #region HitPlatform
  if (!platform) return;

  auto it = platforms.find(platform);
  if (it == platforms.end()) return;

  bool preventDestroy = false;
  switch (it->second) {
  case PlatformType::Ball:
    SpawnBall(BallType::Normal, (Vector2)platform->transform->position);
    break;

  case PlatformType::Player:
    SpawnRecrut((Vector2)platform->transform->position, RecrutType::Player);
    break;

  case PlatformType::Wall:
    {
      auto wallIt = walls.find(platform);
      if (wallIt != walls.end()) {
        wallIt->second--;
        if (wallIt->second > 0 && ball->ballType != BallType::Fire) {
          preventDestroy = true;
          break;
        }
        // Change sprite
      }
      break;
    }

  case PlatformType::FireBall:
    SpawnRecrut((Vector2)platform->transform->position, RecrutType::FireBall);
    break;

  default:
    break;
  }

  if (preventDestroy) return;
  DestroyPlatform(platform);
  // #endregion
}

void LevelManager::DestroyPlatform(GameObject *platform) {
  // #region DestroyPlatform
  if (!platform) return;

  auto it = platforms.find(platform);
  if (it == platforms.end()) return;

  auto wallIt = walls.find(platform);
  if (wallIt != walls.end()) walls.erase(wallIt);

  Destroy(*platform);
  platforms.erase(it);
  // #endregion
}

void LevelManager::SpawnRow() {
  // #region SpawnRow
  const int nOfPlatforms =
      MinPlatformsPerRow + std::rand() % (MaxPlatformsPerRow - MinPlatformsPerRow);

  const float Gap = 0.7f;

  for (short i = 0; i <= nOfPlatforms; i++) {

    int platformType = std::rand() % (int)PlatformType::Count;
    nextPlatform++;
    GameObject &platform =
        gameObject->CreateChild(("Platform_" + std::to_string(nextPlatform)).c_str());

    BoxCollider &col = platform.AddComponent<BoxCollider>(Vector2(PlatformWidth, 0.5f));
    col.isTrigger = true;
    Renderer &renderer =
        platform.AddComponent<Renderer>(Primitives::Square, Vector2(PlatformWidth, 0.5f));



    Vector3 pos((float)i * (PlatformWidth + Gap) -
                    (float)(nOfPlatforms * (PlatformWidth + Gap)) / 2,
                SpawnY, 0);
    platform.transform->position = pos;

    PlatformType type = (PlatformType)platformType;
    switch (type) {
    case PlatformType::Ball:
      renderer.SetColor(SDL_Color{255, 0, 255, 255});
      break;
    case PlatformType::Player:
      renderer.SetColor(SDL_Color{0, 0, 255, 255});
      break;

    case PlatformType::Wall:
      renderer.SetColor(SDL_Color{255, 0, 0, 255});
      walls[&platform] = 3;
      break;

    case PlatformType::FireBall:
      renderer.SetColor(SDL_Color{255, 255, 0, 255});
      break;

    default:
      break;
    }

    platforms[&platform] = type;
  }
  // #endregion
}

Ball *LevelManager::SpawnBall(const BallType &type, const Vector2 &pos, const Vector2 &dir) {
  // #region SpawnBall
  GameObject &ballGO = gameObject->scene->rootGameObject->CreateChild("Ball");



  CircleCollider &ballCollider = ballGO.AddComponent<CircleCollider>(0.25f);



  RigidBody &ballRB = ballGO.AddComponent<RigidBody>();
  ballRB.collisionMode = CollisionMode::Continuous;

  Ball *ball = &ballGO.AddComponent<Ball>();
  ball->ballType = type;

  ballGO.transform->position = Vector3(pos);


  if (type == BallType::Normal) {
    ball->direction = dir;
    balls.push_back(&ballGO);
    ballGO.AddComponent<Renderer>(ballSprite, Vector2(0.5f, 0.5f));
  } else {
    ballGO.AddComponent<Renderer>(fireBallSprite, Vector2(0.7f, 0.7f));
    ballCollider.SetRadius(0.7f);
    ballCollider.isTrigger = true;
    ball->speed = 25;
  }
  return ball;
  // #endregion
}

void LevelManager::CheckBalls() {
  // #region CheckBall
  bool ballFell = false;
  for (GameObject *ball : balls) {
    if (ball->transform->position.get().y > Data::PaddleY - 4) continue;
    ball->transform->position = Vector3(ball->transform->position.get().x, 0, 0);
    Destroy(*ball);
    ballFell = true;
  }
  if (!ballFell || gameOver) return;

  std::erase_if(balls, [](const GameObject *ball) { return (bool)ball->isDeleted; });
  if (balls.empty()) {

    PlayerController::Singleton->RemovePlayer();
    if (gameOver) return;
    Ball *ball = SpawnBall(BallType::Normal, Vector2::Zero);
    ball->waitTime = 1;
    // TODO: wait a second
  }
  // #endregion
}

void LevelManager::SpawnRecrut(const Vector2 &pos, const RecrutType &type) {
  // #region SpawnRecrut
  GameObject &recrutGO = gameObject->scene->rootGameObject->CreateChild("Recrut");
  recrutGO.transform->position = Vector3(pos);

  SDL_Color color;
  switch (type) {
  case RecrutType::Player:
    color = {0, 0, 255, 255};
    break;
  case RecrutType::FireBall:
    color = {255, 255, 0, 255};
    break;

  default:
    throw std::runtime_error("Unknown recrut type");
  }

  recrutGO.AddComponent<Renderer>(Primitives::Circle, Vector2(0.5f, 0.5f), color);
  CircleCollider &col = recrutGO.AddComponent<CircleCollider>(0.25f);
  col.isTrigger = true;
  RigidBody &recrutRB = recrutGO.AddComponent<RigidBody>();

  Recrut &recrut = recrutGO.AddComponent<Recrut>();
  recrut.recrutType = type;
  // #endregion
}


void LevelManager::FireFireBall() {
  // #region FireFireBall


  Vector2 pos = Vector2(PlayerController::Singleton->gameObject->transform->position);
  Ball *left = SpawnBall(BallType::Fire, pos);
  Ball *center = SpawnBall(BallType::Fire, pos);
  Ball *right = SpawnBall(BallType::Fire, pos);



  const std::function<Vector2(Vector2, float)> rotate = [](Vector2 base, float angle) {
    const float rad = angle * DEG2RAD;
    return Vector2(base.x * std::cos(rad) + base.y * std::sin(rad),
                   -base.x * std::sin(rad) + base.y * std::cos(rad));
  };

  Vector2 forward = PlayerController::Singleton->gameObject->transform->forward;

  left->direction = rotate(forward, -45);
  center->direction = forward;
  right->direction = rotate(forward, 45);
  // #endregion
}

} // namespace FOUL::Behaviours
