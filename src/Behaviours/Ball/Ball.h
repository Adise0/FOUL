#pragma once

#include <Crow2D/Crow2D.h>
#include <vector>

namespace FOUL::Behaviours {

class Ball : public Crow2D::Components::Behaviour {
  // #region Data
public:
  Crow2D::Types::Vector2 direction = Crow2D::Types::Vector2::Down;
  Crow2D::Components::Renderer *renderer;

public:
  float waitTime = 0;

private:
  float speed = 15;
  // #endregion

  // #region Crow2D
private:
  void Awake() override;
  void Update() override;
  // #endregion

  // #region Methods
private:
  void Move();

  void OnTriggerEnter(const Crow2D::Components::Collider &other) override;
  // #endregion
};
} // namespace FOUL::Behaviours
