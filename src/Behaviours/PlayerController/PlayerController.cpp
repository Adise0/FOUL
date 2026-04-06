#include "PlayerController.h"
#include "Data.h"
#include <Crow2D/components/colliders/BoxCollider.h>
#include <Crow2D/dataObjects/Sprite.h>
#include <string>

namespace FOUL::Behaviours {
using namespace Crow2D;
using namespace Crow2D::Components;
using namespace Crow2D::Types;

void PlayerController::Awake() {
  for (int i = 0; i < playerCount; i++) {
    Vector3 playerPos((float)i * Data::xPerPlayer - ((float)playerCount * Data::xPerPlayer) / 2, 0,
                      0);
    GameObject &playerGO = gameObject->CreateChild(("Player_" + std::to_string(i)).c_str());
    Renderer &playerRnederer = playerGO.AddComponent<Renderer>(
        Primitives::Circle, Vector2(Data::xPerPlayer - 0.1f, Data::xPerPlayer));

    playerGO.transform->localPosition = playerPos;
  }

  Vector2 colliderSize(Data::xPerPlayer * playerCount, Data::xPerPlayer);
  collider = &gameObject->AddComponent<BoxCollider>(colliderSize);
}


void PlayerController::Update() {}
void PlayerController::Move() {}
} // namespace FOUL::Behaviours
