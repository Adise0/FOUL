#pragma once

#include "Recrut.h"
#include "UIManager.h"
#include <Crow2D/Crow2D.h>
#include <Crow2D/properties/PrivateSetProperty.h>
#include <Crow2D/properties/Property.h>
#include <cmath>
#include <unordered_map>
#include <vector>

namespace FOUL::Behaviours {

class Ball;

enum class PlatformType { Normal, Player, Ball, FireBall, Wall, Count };
enum class BallType { Normal, Fire };
class LevelManager : public Crow2D::Components::Behaviour {
  // #region Get/Set
  const float GetPoints() const;
  void SetPoints(const float &points);
  // #endregion

  // #region Data
public:
  static LevelManager *Singleton;
  static std::unordered_map<Crow2D::GameObject *, PlatformType> platforms;
  static PRIVATE_SET_PROPERTY(LevelManager, bool, isRespawning);

private:
  static constexpr short MinPlatformsPerRow = 2;
  static constexpr short MaxPlatformsPerRow = 6;
  static constexpr float SpawnY = 10.5f;
  static constexpr float PlatformWidth = 4;

public:
  bool gameOver = false;
  PROPERTY(LevelManager, const float, points, GetPoints, SetPoints);

private:
  UIManager *uiManager = nullptr;
  float _points = 0;
  float currentWaitTime = 0;
  float platformSpeed = 0;
  float currentTimer = 0;
  float respawnTime = 3;
  float respawnTimer = 0;
  int nextPlatform = 0;
  std::vector<Crow2D::GameObject *> balls;
  std::unordered_map<Crow2D::GameObject *, short> walls;

  Crow2D::Types::Sprite *ballSprite;
  Crow2D::Types::Sprite *fireBallSprite;
  std::vector<Crow2D::Types::Sprite *> wallSprites;
  Crow2D::Types::Sprite *platformSprite;
  Crow2D::Types::Sprite *ballPlatformSprite;
  Crow2D::Types::Sprite *fireballPlatformSprite;
  Crow2D::Types::Sprite *playerPlatformSprite;

  int _prevRespawn = 0;
  // #endregion


  // #region Crow2D
private:
  void Awake() override;
  void Start() override;
  void Update() override;
  void OnDestroy() override;
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
  void MovePlatforms();
  void Pause(const bool &isTutorial = false);
  void Resume();

  // #endregion
};
} // namespace FOUL::Behaviours
