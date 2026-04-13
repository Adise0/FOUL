#pragma once

#include "LevelManager.h"
#include <Crow2D/Crow2D.h>
#include <Crow2D/dataObjects/Vectors.h>
#include <deque>
#include <vector>

namespace FOUL::Behaviours {

class Ball : public Crow2D::Components::Behaviour {
  // #region Data
public:
  Crow2D::Types::Vector2 direction = Crow2D::Types::Vector2::Down;
  Crow2D::Components::Renderer *renderer;
  BallType ballType = BallType::Normal;
  float speed = 15;

private:
  static constexpr short TrailLength = 16;
  std::deque<Crow2D::Types::Vector3> trailPositions;
  std::deque<float> trailRotations;
  Crow2D::Components::Renderer *trailRenderers[TrailLength]{};
  Crow2D::Types::Vector3 _prevPos;
  float _prevAngle = 0;
  // #endregion

  // #region Crow2D
private:
  void Start() override;
  void Update() override;
  void OnDestroy() override;
  // #endregion

  // #region Methods
private:
  void Move();

  void OnTriggerEnter(const Crow2D::Components::Collider &other) override;
  // #endregion
};
} // namespace FOUL::Behaviours
