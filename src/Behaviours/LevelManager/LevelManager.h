#pragma once

#include <Crow2D/Crow2D.h>
#include <unordered_map>
#include <vector>

namespace FOUL::Behaviours {

enum class PlatformType { Normal, Goal, Player, Ball, FireBall };

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

private:
  float currentWaitTime = levelSpeed;
  float currentTimer = currentWaitTime;
  int nextPlatform = 0;

  bool gameOver = false;
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

private:
  void SetupSingleton();
  void SpawnNormalRow();
  // #endregion
};
} // namespace FOUL::Behaviours
