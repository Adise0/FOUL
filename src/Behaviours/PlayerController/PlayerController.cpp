#include "PlayerController.h"
#include "Data.h"
#include <Crow2D/InputManager.h>
#include <Crow2D/dataObjects/Vectors.h>
#include <SDL3/SDL_mouse.h>
#include <algorithm>
#include <cstdio>
#include <string>

namespace FOUL::Behaviours {
using namespace Crow2D;
using namespace Crow2D::Components;
using namespace Crow2D::Types;
using namespace Crow2D::Inputs;

void PlayerController::Awake() {
  for (int i = 0; i < playerCount; i++) {
    Vector3 playerPos(
        (float)i * Data::xPerPlayer - ((float)(playerCount - 1) * Data::xPerPlayer) / 2, 0, 0);
    GameObject &playerGO = gameObject->CreateChild(("Player_" + std::to_string(i)).c_str());
    Renderer &playerRnederer = playerGO.AddComponent<Renderer>(
        Primitives::Circle, Vector2(Data::xPerPlayer - 0.1f, Data::xPerPlayer));

    playerGO.transform->localPosition = playerPos;
  }

  Vector2 colliderSize(Data::xPerPlayer * (playerCount + 1), Data::xPerPlayer);
  collider = &gameObject->AddComponent<BoxCollider>(colliderSize);
  collider->isTrigger = true;
}


void PlayerController::Update() {
  // #region Update
  Move();
  CheckBalls();
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

  deg = std::clamp(deg, -80.0f, 80.0f);
  transform->rotation = deg;

  Vector2 delta = dir * speed * Time::deltaTime;
  Vector3 pos = transform->position + Vector3(delta);
  const float halfWidth = Data::xPerPlayer * playerCount / 2;

  pos.x = std::clamp(pos.x, -Data::XLimit + halfWidth, Data::XLimit - halfWidth);
  transform->position = pos;
  // #endregion
}

void PlayerController::CheckBalls() {
  // #region CheckBall

  for (Ball *ball : balls) {
    if (ball->transform->position.get().y > Data::PaddleY - 1) continue;
    ball->transform->position = Vector3(ball->transform->position.get().x, 0, 0);
    ball->direction = Vector2::Down;
  }
  // #endregion
}

void PlayerController::OnColliderEnter(const Collider &other) {
  // #region OnTriggerEnter
  Ball *ball = other.gameObject->GetComponent<Ball>();
  if (!ball) return;

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

  if (dotLeft > 0 && dotRight > 0) {
    ball->direction = right;
  } else if (dotLeft < 0 && dotRight < 0) {
    ball->direction = -right;
  } else {
    float dotForward = (ballPos - paddlePos).Normalized().x * forward.x +
                       (ballPos - paddlePos).Normalized().y * forward.y;
    ball->direction = dotForward >= 0 ? forward : -forward;
  }
  // #endregion
}
} // namespace FOUL::Behaviours
