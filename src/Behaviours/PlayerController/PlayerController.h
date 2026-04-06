#pragma once

#include <Crow2D/Crow2D.h>
#include <Crow2D/components/Renderer.h>
#include <Crow2D/components/colliders/BoxCollider.h>
#include <vector>

namespace FOUL::Behaviours {

class PlayerController : public Crow2D::Components::Behaviour {
  // #region Data
private:
  Crow2D::GameObject *barrier = nullptr;
  short playerCount = 3;
  std::vector<Crow2D::Components::Renderer> players;
  Crow2D::Components::BoxCollider *collider;
  // #endregion

  // #region Crow2D
  void Awake() override;
  void Update() override;
  // #endregion

  // #region Methods
  void Move();
  // #endregion
};
} // namespace FOUL::Behaviours
