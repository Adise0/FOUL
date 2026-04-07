#pragma once

#include "Ball.h"
#include <Crow2D/Crow2D.h>
#include <Crow2D/GameObject.h>
#include <Crow2D/components/Renderer.h>
#include <Crow2D/components/colliders/BoxCollider.h>
#include <Crow2D/components/colliders/Collider.h>
#include <vector>

namespace FOUL::Behaviours {

class PlayerController : public Crow2D::Components::Behaviour {
  // #region Data
public:
  static PlayerController *Singleton;


private:
  Crow2D::GameObject *barrier = nullptr;
  short playerCount = 3;
  short _playerCount = 0;
  float speed = 12;
  std::vector<Crow2D::GameObject *> players;
  Crow2D::Components::BoxCollider *collider = nullptr;
  // #endregion

  // #region Crow2D
  void Awake() override;
  void Start() override;
  void Update() override;
  void OnColliderEnter(const Crow2D::Components::Collider &other) override;
  // #endregion

  // #region Methods
private:
  void SetupSingleton();
  void Move();
  void UpdatePlayers();

  void BounceNormalBall(Ball *ball) const;

public:
  void RemovePlayer();
  void AddPlayer();
  // #endregion
};
} // namespace FOUL::Behaviours
