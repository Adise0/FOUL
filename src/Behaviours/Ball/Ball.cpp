#include "Ball.h"
#include "Data.h"
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

} // namespace FOUL::Behaviours
