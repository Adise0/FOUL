#include "PlayerController.h"
#include "Data.h"
#include "LevelManager.h"
#include <Crow2D/GameObject.h>
#include <Crow2D/InputManager.h>
#include <Crow2D/dataObjects/Sprite.h>
#include <Crow2D/dataObjects/Vectors.h>
#include <SDL3/SDL_mouse.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace FOUL::Behaviours {
using namespace Crow2D;
using namespace Crow2D::Components;
using namespace Crow2D::Types;
using namespace Crow2D::Inputs;

static constexpr float DEG2RAD = 3.14159265358979323846f / 180.0f;

PlayerController *PlayerController::Singleton = nullptr;



void PlayerController::SetupSingleton() {
  // #region SetupSingleton
  // if (Singleton != nullptr && Singleton != this) {
  //   Destroy(this);
  //   return;
  // }
  Singleton = this;
  // #endregion
}


void PlayerController::Awake() {
  SetupSingleton();

  Vector2 colliderSize(Data::xPerPlayer * (playerCount + 1), Data::xPerPlayer);
  collider = &gameObject->AddComponent<BoxCollider>(colliderSize);
  collider->isTrigger = true;
  collider->drawGizmos = false;
  InitializeSprites();
}

void PlayerController::InitializeSprites() {
  // #region InitializeSprites
  const Vector2 cells(8, 6);
  const Vector2 spriteSize(21, 31);

  for (int x = 0; x < cells.x; x++) {
    for (int y = 0; y < cells.y; y++) {
      Sprite *spr = new Sprite("sprites/Characters.png",
                               {x * spriteSize.x, y * spriteSize.y, spriteSize.x, spriteSize.y});
      playerSprites.push_back(spr);
    }
  }
  // #endregion
}

void PlayerController::Start() { UpdatePlayers(); }

void PlayerController::Update() {
  // #region Update
  if (LevelManager::Singleton->isGameOver) return;
  Move();
  // #endregion
}

void PlayerController::Move() {
  // #region Move
  Vector2 dir;
  if (InputManager::GetKey("A").isPressed) dir += Vector2::Left;
  if (InputManager::GetKey("D").isPressed) dir += Vector2::Right;
  float deg = transform->rotation;
  if (InputManager::GetKey(InputManager::GetMouseButtonName(SDL_BUTTON_RIGHT)).isPressed)
    deg += 90 * Time::deltaTime;

  if (InputManager::GetKey(InputManager::GetMouseButtonName(SDL_BUTTON_LEFT)).isPressed)
    deg -= 90 * Time::deltaTime;

  deg = std::clamp(deg, -45.0f, 45.0f);
  transform->rotation = deg;

  Vector2 delta = dir * speed * Time::deltaTime;
  Vector3 pos = transform->position + Vector3(delta);
  const float halfWidth = Data::xPerPlayer * playerCount / 2;

  pos.x = std::clamp(pos.x, -Data::XLimit + halfWidth, Data::XLimit - halfWidth);
  transform->position = pos;
  // #endregion
}



void PlayerController::UpdatePlayers() {
  // #region UpdatePlayers
  if (_playerCount == playerCount) return;

  int playerDelta = playerCount - _playerCount;
  _playerCount = playerCount;

  if (playerDelta > 0) {
    for (int i = players.size(); i < playerCount; i++) {
      GameObject &playerGO = gameObject->CreateChild(("Player_" + std::to_string(i)).c_str());
      playerGO.transform->Rotate(-90);
      int sprIndex = std::rand() % playerSprites.size();

      const Sprite *spr = _recturSpr;
      if (!spr) {
        short sprIndex = std::rand() % playerSprites.size();
        spr = playerSprites[sprIndex];
      }

      playerGO.AddComponent<Renderer>(spr, Vector2(Data::xPerPlayer - 0.2f, Data::xPerPlayer));
      playerGO.AddComponent<Renderer>(Primitives::Circle,
                                      Vector2(Data::xPerPlayer - 0.1f, Data::xPerPlayer),
                                      SDL_Color{0, 0, 0, 75});
      _recturSpr = nullptr;
      players.push_back(&playerGO);
    }
  } else {
    for (int i = players.size() - 1; i >= playerCount; i--) {
      Destroy(*players[i]);
      players.pop_back();
    }
  }


  for (int i = 0; i < players.size(); i++) {
    Vector3 playerPos(
        (float)i * Data::xPerPlayer - ((float)(playerCount - 1) * Data::xPerPlayer) / 2, 0, 0);
    players[i]->transform->localPosition = playerPos;
  }

  Vector2 colliderSize(Data::xPerPlayer * (playerCount + 1), Data::xPerPlayer);
  collider->SetSize(colliderSize);
  // #endregion
}

void PlayerController::RemovePlayer() {
  // #region RemovePlayer
  playerCount--;
  UpdatePlayers();
  if (playerCount != 0) return;

  LevelManager::Singleton->GameOver();
  // #endregion
}
void PlayerController::AddPlayer(GameObject *recrut) {
  // #region AddPlayer
  playerCount++;
  _recturSpr = &recrut->GetComponent<Renderer>()->sprite.get();
  UpdatePlayers();
  // #endregion
}

void PlayerController::BounceNormalBall(Ball *ball) const {
  // #region BounceNormalBall
  Vector2 ballPos(ball->transform->position.get());
  Vector2 paddlePos(transform->position.get());
  Vector2 forward = transform->forward.get();
  Vector2 right(forward.y, -forward.x);

  float totalWidth = Data::xPerPlayer * playerCount * 0.5f;
  Vector2 leftPoint = paddlePos - right * totalWidth;
  Vector2 rightPoint = paddlePos + right * totalWidth;

  Vector2 toFromLeft = (ballPos - leftPoint).Normalized();
  Vector2 toFromRight = (ballPos - rightPoint).Normalized();

  float dotLeft = toFromLeft.x * right.x + toFromLeft.y * right.y;
  float dotRight = toFromRight.x * right.x + toFromRight.y * right.y;

  if (dotRight > 0.5f && dotLeft > 0.9f) {
    ball->direction = right;
  } else if (dotLeft < -0.5f && dotRight < -0.9f) {
    ball->direction = -right;
  } else {
    float dotForward = (ballPos - paddlePos).Normalized().x * forward.x +
                       (ballPos - paddlePos).Normalized().y * forward.y;

    int variance = (std::rand() % 11) - 5;
    float rad = variance * DEG2RAD;
    Vector2 varianceDir(std::sin(rad), std::cos(rad));
    ball->direction = dotForward >= 0 ? (forward + varianceDir).Normalized() : -forward;
  }
  // #endregion
}

void PlayerController::OnColliderEnter(const Collider &other) {
  // #region OnTriggerEnter
  Ball *ball = other.gameObject->GetComponent<Ball>();
  if (!ball || ball->ballType != BallType::Normal) return;

  BounceNormalBall(ball);
  // #endregion
}

void PlayerController::OnDestroy() {
  Singleton = nullptr;
  for (Sprite *spr : playerSprites) {
    delete spr;
  }
}

} // namespace FOUL::Behaviours
