#include "Ball.h"
#include "Data.h"
#include "LevelManager.h"
#include "PlayerController.h"
#include <Crow2D/dataObjects/Vectors.h>
#include <SDL3/SDL_pixels.h>
#include <cstdio>

namespace FOUL::Behaviours {
using namespace Crow2D;
using namespace Crow2D::Components;
using namespace Crow2D::Types;
using namespace Crow2D::Inputs;

constexpr float RAD2DEG = 180.0f / 3.14159265358979f;

void Ball::Start() {
  // #region Awake


  const Vector2 segmentSize =
      ballType == BallType::Normal ? Vector2(0.5f, 0.2f) : Vector2(0.7f, 0.2);

  const SDL_Color baseColor =
      ballType == BallType::Normal ? SDL_Color{255, 255, 255, 255} : SDL_Color{255, 160, 0, 255};


  for (short i = 0; i < TrailLength; i++) {
    GameObject &segmentGO = gameObject->CreateChild("TrailSegment");
    segmentGO.transform->position += Vector3(0, 0, -1);
    Renderer &segmentRenderer = segmentGO.AddComponent<Renderer>(Primitives::Circle, segmentSize);
    Uint8 alpha = (Uint8)(200 * (1.0f - (float)i / TrailLength));
    segmentRenderer.SetColor({baseColor.r, baseColor.g, baseColor.b, alpha});
    trailRenderers[i] = &segmentRenderer;
    trailPositions.push_back(segmentGO.transform->position);
    trailRotations.push_back(0);
  }

  renderer = gameObject->GetComponent<Renderer>();
  // #endregion
}

void Ball::Update() {
  // #region Update
  printf("Update!\n");
  if (LevelManager::Singleton->isGameOver) return;
  if (LevelManager::isRespawning && ballType == BallType::Normal) return;
  Move();
  // #endregion
}

void Ball::Move() {
  // #region Move
  transform->Translate(direction * speed * Time::deltaTime);
  Vector2 pos = Vector2(transform->position);

  if ((pos.x <= -Data::XLimit && direction.x < 0) || (pos.x >= Data::XLimit && direction.x > 0))
    direction.x = -direction.x;
  if (ballType != BallType::Fire && pos.y >= 9.75f && direction.y > 0) direction.y = -direction.y;

  if (ballType == BallType::Fire && pos.y > 10) Destroy(gameObject);

  if (Vector3::Distance(transform->position, _prevPos) < 0.1f) return;
  _prevPos = transform->position;
  _prevAngle = transform->rotation;

  trailPositions.push_front(Vector3(pos, -1));
  trailPositions.pop_back();

  float angle = atan2f(direction.x, direction.y) * RAD2DEG;
  trailRotations.push_front(angle);
  trailRotations.pop_back();

  for (short i = 0; i < TrailLength; i++) {
    trailRenderers[i]->transform->position = trailPositions[i];
    trailRenderers[i]->transform->rotation = trailRotations[i];
  }

  // #endregion
}

void Ball::OnTriggerEnter(const Collider &other) {
  // #region OnTriggerEnter
  GameObject *go = other.gameObject;

  if (!go->name.get().starts_with("Platform")) return;

  if (ballType == BallType::Fire) {
    LevelManager::Singleton->HitPlatform(go, this);
    return;
  }
  BoxCollider *box = go->GetComponent<BoxCollider>();
  float angle = go->transform->rotation * (3.14159f / 180.0f);
  float cosine = std::cos(-angle), sine = std::sin(-angle);

  Vector2 ballPos(transform->position);
  Vector2 boxPos(go->transform->position);
  float dx = ballPos.x - boxPos.x, dy = ballPos.y - boxPos.y;

  float localX = dx * cosine - dy * sine;
  float localY = dx * sine + dy * cosine;

  float hw = box->rect.get().w * 0.5f;
  float hh = box->rect.get().h * 0.5f;

  Vector2 localNormal = std::abs(localX / hw) > std::abs(localY / hh)
                            ? Vector2(localX > 0 ? 1.f : -1.f, 0)
                            : Vector2(0, localY > 0 ? 1.f : -1.f);

  Vector2 normal = {localNormal.x * std::cos(angle) - localNormal.y * std::sin(angle),
                    localNormal.x * std::sin(angle) + localNormal.y * std::cos(angle)};

  direction = direction - normal * (2.0f * direction.Dot(normal));


  LevelManager::Singleton->HitPlatform(go, this);
  // #endregion
}

void Ball::OnDestroy() {}

} // namespace FOUL::Behaviours
