#include "Recrut.h"
#include "LevelManager.h"
#include "PlayerController.h"

namespace FOUL::Behaviours {
using namespace Crow2D;
using namespace Crow2D::Components;
using namespace Crow2D::Types;
using namespace Crow2D::Inputs;

void Recrut::Awake() {
  // #region Awake
  // #endregion
}


void Recrut::Update() {
  // #region Update
  if (LevelManager::Singleton->gameOver) return;
  Move();
  // #endregion
}

void Recrut::Move() {
  // #region Move
  transform->Translate(Vector2::Down * speed * Time::deltaTime);
  // #endregion
}

void Recrut::OnTriggerEnter(const Collider &other) {
  // #region OnTriggerEnter
  GameObject *go = other.gameObject;

  if (go->name != "PlayerController") return;

  switch (recrutType) {
  case RecrutType::Player:
    PlayerController::Singleton->AddPlayer();
    break;
  case RecrutType::FireBall:
    LevelManager::Singleton->FireFireBall();
    break;
  }
  Destroy(gameObject);
  // #endregion
}

} // namespace FOUL::Behaviours
