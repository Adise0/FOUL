#pragma once

#include "Recrut.h"
#include <Crow2D/Crow2D.h>
#include <unordered_map>
#include <vector>

namespace FOUL::Behaviours {

class Ball;

enum class PlatformType { Normal, Player, Ball, FireBall, Wall, Count };
enum class BallType { Normal, Fire };
class LevelManager : public Crow2D::Components::Behaviour {
  // #region Data
public:
  static LevelManager *Singleton;
  static float levelSpeed;
  static std::unordered_map<Crow2D::GameObject *, PlatformType> platforms;
  static int points;

private:
  static constexpr short MinPlatformsPerRow = 2;
  static constexpr short MaxPlatformsPerRow = 6;
  static constexpr float SpawnY = 9;
  static constexpr float PlatformWidth = 4;

public:
  bool gameOver = false;

private:
  float currentWaitTime = levelSpeed;
  float currentTimer = currentWaitTime;
  int nextPlatform = 0;
  std::vector<Crow2D::GameObject *> balls;
  std::unordered_map<Crow2D::GameObject *, short> walls;

  Crow2D::Types::Sprite *ballSprite;
  Crow2D::Types::Sprite *fireBallSprite;


public:
  // #endregion

  // #region Crow2D
private:
  void Awake() override;
  void Start() override;
  void Update() override;
  // #endregion

  // #region Methods
public:
  void Reset();
  Ball *SpawnBall(const BallType &type, const Crow2D::Types::Vector2 &pos,
                  const Crow2D::Types::Vector2 &dir = Crow2D::Types::Vector2::Down);
  void RemoveBall(Crow2D::GameObject *ball);

  void HitPlatform(Crow2D::GameObject *platform, Ball *ball);
  void FireFireBall();

private:
  void DestroyPlatform(Crow2D::GameObject *platform);
  void SpawnRecrut(const Crow2D::Types::Vector2 &pos, const RecrutType &type);
  void SpawnFireBall(const Crow2D::Types::Vector2 &pos);
  void CheckBalls();
  void SetupSingleton();
  void SpawnRow();

  // #endregion
};
} // namespace FOUL::Behaviours
