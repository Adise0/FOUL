#pragma once

#include <Crow2D/Crow2D.h>
#include <vector>

namespace FOUL::Behaviours {

class Recrut : public Crow2D::Components::Behaviour {
  // #region Data
public:
public:
private:
  float speed = 10;
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
