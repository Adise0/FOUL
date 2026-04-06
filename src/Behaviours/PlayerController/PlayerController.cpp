#include "PlayerController.h"
#include "Data.h"
#include <Crow2D/InputManager.h>
#include <Crow2D/components/colliders/BoxCollider.h>
#include <Crow2D/dataObjects/Sprite.h>
#include <Crow2D/dataObjects/Vectors.h>
#include <algorithm>
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

  Vector2 colliderSize(Data::xPerPlayer * playerCount, Data::xPerPlayer);
  collider = &gameObject->AddComponent<BoxCollider>(colliderSize);
}


void PlayerController::Update() {
  // #region Update
  Move();
  // #endregion
}

void PlayerController::Move() {
  // #region Move
  Vector2 dir;
  if (InputManager::GetKey("A").isPressed) dir += Vector2::Left;
  if (InputManager::GetKey("D").isPressed) dir += Vector2::Right;

  Vector2 delta = dir * speed * Time::deltaTime;
  Vector3 pos = transform->position + Vector3(delta);
  const float halfWidth = Data::xPerPlayer * playerCount / 2;

  pos.x = std::clamp(pos.x, -Data::XLimit + halfWidth, Data::XLimit - halfWidth);
  transform->position = pos;

  // #endregion
}
} // namespace FOUL::Behaviours
