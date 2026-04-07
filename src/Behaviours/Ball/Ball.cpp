#include "Ball.h"
#include "Data.h"
#include "LevelManager.h"
#include "PlayerController.h"
#include <Crow2D/GameObject.h>
#include <Crow2D/dataObjects/Vectors.h>

namespace FOUL::Behaviours {
using namespace Crow2D;
using namespace Crow2D::Components;
using namespace Crow2D::Types;
using namespace Crow2D::Inputs;

void Ball::Awake() {
  // #region Awake
  renderer = gameObject->GetComponent<Renderer>();
  // #endregion
}


void Ball::Update() {
  // #region Update
  if (LevelManager::Singleton->gameOver) return;

  if (waitTime > 0) {
    waitTime -= Time::deltaTime;
    return;
  } else if (waitTime != 0) waitTime = 0;
  Move();
  // #endregion
}

void Ball::Move() {
  // #region Move
  transform->Translate(direction * speed * Time::deltaTime);
  Vector2 pos = Vector2(transform->position);

  if ((pos.x <= -Data::XLimit && direction.x < 0) || (pos.x >= Data::XLimit && direction.x > 0))
    direction.x = -direction.x;
  if (pos.y >= 9.75f && direction.y > 0) direction.y = -direction.y;
  // #endregion
}

void Ball::OnTriggerEnter(const Collider &other) {
  // #region OnTriggerEnter
  GameObject *go = other.gameObject;

  if (!go->name.get().starts_with("Platform")) return;


  BoxCollider *box = go->GetComponent<BoxCollider>();
  float angle = go->transform->rotation * (3.14159f / 180.0f);
  float cosine = std::cos(-angle), sine = std::sin(-angle);

  Vector2 ballPos(transform->position);
  Vector2 boxPos(go->transform->position);
  float dx = ballPos.x - boxPos.x, dy = ballPos.y - boxPos.y;

  float localX = dx * cosine - dy * sine;
  float localY = dx * sine + dy * cosine;

  float hw = box->rect.get().w * 0.5f;
  float hh = box->rect.get().h * 0.5f;

  Vector2 localNormal = std::abs(localX / hw) > std::abs(localY / hh)
                            ? Vector2(localX > 0 ? 1.f : -1.f, 0)
                            : Vector2(0, localY > 0 ? 1.f : -1.f);

  Vector2 normal = {localNormal.x * std::cos(angle) - localNormal.y * std::sin(angle),
                    localNormal.x * std::sin(angle) + localNormal.y * std::cos(angle)};

  direction = direction - normal * (2.0f * direction.Dot(normal));


  LevelManager::Singleton->HitPlatform(go);

  // #endregion
}

} // namespace FOUL::Behaviours
