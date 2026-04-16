#define NOMINMAX

#include "LevelManager.h"
#include "Ball.h"
#include "Data.h"
#include "MenuScene.h"
#include "PlayerController.h"
#include "Recrut.h"
#include "UIManager.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

namespace FOUL::Behaviours {
using namespace Crow2D;
using namespace Crow2D::Components;
using namespace Crow2D::Types;
using namespace Crow2D::Inputs;
using namespace Crow2D::Utils;
using namespace Crow2D::Sound;

static constexpr float DEG2RAD = 3.14159265358979323846f / 180.0f;
static const int weights[] = {
    65, // Normal
    7,  // Player
    10, // Ball
    2,  // FireBall
    15, //Wall
};


LevelManager *LevelManager::Singleton = nullptr;
std::unordered_map<Crow2D::GameObject *, PlatformType> LevelManager::platforms;
PrivateSetProperty<LevelManager, bool> LevelManager::isRespawning;

const float LevelManager::GetPoints() const { return _points; };
void LevelManager::SetPoints(const float &points) {
  float newPoints = std::floor(points);
  if (uiManager && newPoints != _points) uiManager->UpdatePoints(newPoints);
  _points = points;
}


void LevelManager::SetupSingleton() {
  // #region SetupSingleton
  // if (Singleton != nullptr && Singleton != this) {
  //   Destroy(this);
  //   return;
  // }
  Singleton = this;
  // #endregion
}

void LevelManager::Awake() {
  // #region Awake
  uiManager = gameObject->GetComponent<UIManager>();
  isRespawning.set(false);
  isGameOver.set(false);
  SetupSingleton();
  fireBallSprite = new Sprite("sprites/FireBall.png", SDL_ScaleMode::SDL_SCALEMODE_PIXELART);
  ballSprite = new Sprite("sprites/Ball.png", SDL_ScaleMode::SDL_SCALEMODE_PIXELART);

  for (int i = 0; i < 3; i++) {
    wallSprites.push_back(
        new Sprite(("sprites/platforms/wall_" + std::to_string(i) + ".png").c_str(),
                   SDL_ScaleMode::SDL_SCALEMODE_PIXELART));
  }

  platformSprite =
      new Sprite("sprites/platforms/normal.png", SDL_ScaleMode::SDL_SCALEMODE_PIXELART);
  ballPlatformSprite =
      new Sprite("sprites/platforms/ball.png", SDL_ScaleMode::SDL_SCALEMODE_PIXELART);
  fireballPlatformSprite =
      new Sprite("sprites/platforms/fire.png", SDL_ScaleMode::SDL_SCALEMODE_PIXELART);
  playerPlatformSprite =
      new Sprite("sprites/platforms/player.png", SDL_ScaleMode::SDL_SCALEMODE_PIXELART);


  amiance = new Audioclip("sounds/ambiance.wav", true);
  startWhistle = new Audioclip("sounds/start_whistle.wav");
  endWhistle = new Audioclip("sounds/end_whistle.wav");
  foulWhistle = new Audioclip("sounds/foul.wav");
  // #endregion
}

void LevelManager::Start() {
  // #region Start
  Reset();
  SpawnBall(BallType::Normal, Vector2::Zero);

  uiManager->pauseRenderer->bridge->On("Resume",
                                       [this](const std::string, const std::string) { Resume(); });

  //TODO: get show tut from persistent data instead isRespawning.set(true);

  Pause(true);
  uiManager->pauseRenderer->bridge->On(
      "__loaded", [this](const std::string, const std::string) { Pause(true); });



  ambianceEmitter->SetVolume(0.7f);
  ambianceEmitter->Play(amiance);

  // #endregion
}

void LevelManager::Update() {
  // #region Update
  if (isGameOver) {
    if (InputManager::GetKey("Escape").wasPressedThisFrame) {
      MenuScene *menuScene = new MenuScene();
      Scenes::SceneManager::SetSceneAsActive(*menuScene);

      // Singleton = nullptr;
      platforms.clear();
      isRespawning.set(false);

      // if (Data::menuScene) delete Data::menuScene;
      // Data::menuScene = menuScene;
    }
    return;
  }
  if (!hasShownTutorial) return;

  if (InputManager::GetKey("Escape").wasPressedThisFrame && !pauseGrace) {
    Pause();
    return;
  }

  pauseGrace = false;

  if (isRespawning) {
    respawnTimer += Time::deltaTime;
    if (respawnTimer >= respawnTime) {
      isRespawning.set(false);
      respawnTimer = 0;
      uiManager->Respawn(0);
    } else {
      int respawn = respawnTime - std::floor(respawnTimer);
      if (_prevRespawn != respawn) {
        _prevRespawn = respawn;
        uiManager->Respawn(respawn);
      }
    }
    return;
  }

  points += Time::deltaTime;

  CheckBalls();
  MovePlatforms();


  currentTimer += Time::deltaTime;
  if (currentTimer < currentWaitTime) return;


  currentTimer = 0;

  SpawnRow();
  // #endregion
}

void LevelManager::Reset() {
  // #region Reset
  currentWaitTime = 8;
  platformSpeed = 0.3f;
  currentTimer = INFINITY;
  respawnTimer = 0;
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
    points += 1;
    break;

  case PlatformType::Player:
    SpawnRecrut((Vector2)platform->transform->position, RecrutType::Player);
    points += 1;
    break;

  case PlatformType::Wall:
    {
      auto wallIt = walls.find(platform);
      if (wallIt != walls.end()) {
        wallIt->second++;
        if (wallIt->second < 3 && ball->ballType != BallType::Fire) {
          wallIt->first->GetComponent<Renderer>()->SetSprite(wallSprites[wallIt->second]);
          preventDestroy = true;
        } else {
          points += 10;
        }
      }
      break;
    }

  case PlatformType::FireBall:
    SpawnRecrut((Vector2)platform->transform->position, RecrutType::FireBall);
    break;

  default:
    points += 5;
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

    nextPlatform++;
    GameObject &platform =
        gameObject->CreateChild(("Platform_" + std::to_string(nextPlatform)).c_str());

    BoxCollider &col = platform.AddComponent<BoxCollider>(Vector2(PlatformWidth, 0.5f));
    col.isTrigger = true;



    Vector3 pos((float)i * (PlatformWidth + Gap) -
                    (float)(nOfPlatforms * (PlatformWidth + Gap)) / 2,
                SpawnY, 0);
    platform.transform->position = pos;

    int total = 0;
    for (int w : weights)
      total += w;

    int roll = std::rand() % total;

    int cumulative = 0;
    int platformType = 0;
    for (int j = 0; j < (int)PlatformType::Count; j++) {
      cumulative += weights[j];
      if (roll < cumulative) {
        platformType = j;
        break;
      }
    }

    PlatformType type = (PlatformType)platformType;
    if (type == PlatformType::Player && PlayerController::Singleton->playerCount >= 6)
      type = PlatformType::Normal;

    Sprite *sprite;

    switch (type) {
    case PlatformType::Ball:
      sprite = ballPlatformSprite;
      break;
    case PlatformType::Player:
      sprite = playerPlatformSprite;
      break;

    case PlatformType::Wall:
      sprite = wallSprites[0];
      walls[&platform] = 0;
      break;

    case PlatformType::FireBall:
      sprite = fireballPlatformSprite;
      break;

    default:
      sprite = platformSprite;
      break;
    }

    platform.AddComponent<Renderer>(sprite, Vector2(PlatformWidth, 1));
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

  SoundEmitter &emitter = ballGO.AddComponent<SoundEmitter>();
  Ball *ball = &ballGO.AddComponent<Ball>();
  ball->emitter = &emitter;
  ball->ballType = type;

  ballGO.transform->position = Vector3(pos);


  if (type == BallType::Normal) {
    ball->direction = dir;
    balls.push_back(ball);
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
  for (Ball *ball : balls) {

    ball->speed = 15.0f + (platformSpeed - 0.5f) * 3.0f;

    if (ball->transform->position.get().y > Data::PaddleY - 4) continue;
    ball->transform->position = Vector3(ball->transform->position.get().x, 0, 0);
    Destroy(*ball->gameObject);
    ballFell = true;
  }
  if (!ballFell || isGameOver) return;

  std::erase_if(balls, [](const Ball *ball) { return (bool)ball->isDeleted; });
  if (balls.empty()) {

    PlayerController::Singleton->RemovePlayer();
    if (isGameOver) return;
    Ball *ball = SpawnBall(BallType::Normal, Vector2::Zero);
    whistleEmitter->Play(foulWhistle);
    isRespawning.set(true);
    // TODO: wait a second
  }
  // #endregion
}

void LevelManager::SpawnRecrut(const Vector2 &pos, const RecrutType &type) {
  // #region SpawnRecrut
  GameObject &recrutGO = gameObject->scene->rootGameObject->CreateChild("Recrut");
  recrutGO.transform->position = Vector3(pos);

  switch (type) {
  case RecrutType::Player:
    {
      std::vector<Sprite *> &sprites = PlayerController::Singleton->playerSprites;
      short sprIndex = std::rand() % sprites.size();

      recrutGO.AddComponent<Renderer>(sprites[sprIndex],
                                      Vector2(Data::xPerPlayer - 0.2f, Data::xPerPlayer));
      recrutGO.transform->Rotate(90);
      break;
    }
  case RecrutType::FireBall:
    recrutGO.AddComponent<Renderer>(fireBallSprite, Vector2(0.5f, 0.5f));
    break;

  default:
    throw std::runtime_error("Unknown recrut type");
  }

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
  left->direction = rotate(forward, -30);
  center->direction = forward;
  right->direction = rotate(forward, 30);
  // #endregion
}

void LevelManager::MovePlatforms() {
  // #region MovePlatforms
  platformSpeed += 1.0f / 60.0f * 0.15f * Time::deltaTime;
  currentWaitTime -= 1.0f / 60.0f * 0.4f * Time::deltaTime;
  currentWaitTime = std::max(currentWaitTime, 2.0f);

  for (auto &[platform, _] : platforms) {
    platform->transform->position += Vector3::Down * platformSpeed * Time::deltaTime;
    if (platform->transform->position.get().y <= Data::PaddleY) GameOver();
  }
  // #endregion
}

void LevelManager::Pause(const bool &isTutorial) {
  // #region Pause
  Time::timeScale = 0;
  uiManager->SetPause(true, isTutorial);
  // #endregion
}

void LevelManager::GameOver() {
  // #region GameOver
  whistleEmitter->Play(endWhistle);

  isGameOver.set(true);
  int pb = uiManager->GetPB(Data::currentPlayer);
  bool isPb = pb == -1 || pb < points;
  uiManager->GameOver(Data::currentPlayer, points, isPb);
  if (isPb) uiManager->SavePB(Data::currentPlayer, points);
  // #endregion
}

void LevelManager::Resume() {
  // #region Resume

  whistleEmitter->Play(foulWhistle);
  uiManager->SetPause(false);
  Time::timeScale = 1.0f;
  pauseGrace = true;
  hasShownTutorial = true;
  // #endregion
}

void LevelManager::OnDestroy() {
  // #region OnDestroy


  delete fireBallSprite;
  delete ballSprite;
  for (Sprite *spr : wallSprites) {
    delete spr;
  }

  delete platformSprite;
  delete ballPlatformSprite;
  delete playerPlatformSprite;
  delete fireballPlatformSprite;

  delete amiance;
  delete startWhistle;
  delete endWhistle;
  delete foulWhistle;
  // #endregion
}

} // namespace FOUL::Behaviours
