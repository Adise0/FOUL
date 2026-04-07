#pragma once

#include <Crow2D/Crow2D.h>
#include <Crow2D/GameObject.h>
#include <Crow2D/dataObjects/Vectors.h>
#include <unordered_map>
#include <vector>

namespace FOUL::Behaviours {

enum class PlatformType { Normal, Goal, Player, Ball, FireBall };
enum class BallType { Normal, Fire };
class LevelManager : public Crow2D::Components::Behaviour {
  // #region Data
public:
  static LevelManager *Singleton;
  static float levelSpeed;
  static std::unordered_map<Crow2D::GameObject *, PlatformType> platforms;
  static int points;
  static short freeBalls;

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
  void DestroyPlatform(Crow2D::GameObject *platform);
  void Reset();
  void SpawnBall(const BallType &type, const Crow2D::Types::Vector2 &pos,
                 const Crow2D::Types::Vector2 &dir = Crow2D::Types::Vector2::Down);
  void RemoveBall(Crow2D::GameObject *ball);

private:
  void CheckBalls();
  void SetupSingleton();
  void SpawnNormalRow();
  // #endregion
};
} // namespace FOUL::Behaviours
