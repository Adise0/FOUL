#pragma once

#include <Crow2D/Crow2D.h>
#include <Crow2D/dataObjects/AudioClip.h>
#include <string>
#include <vector>

namespace FOUL::Behaviours {


class UIManager : public Crow2D::Components::Behaviour {
  // #region Data
public:
  Crow2D::Components::UIRenderer *pauseRenderer = nullptr;
  Crow2D::Components::UIRenderer *mainRenderer = nullptr;

private:
  const std::string SavesFile = Crow2D::Crow2dUtils::RootDirectory + "/players.bin";
  Crow2D::Components::SoundEmitter *emitter = nullptr;

  Crow2D::Sound::Audioclip *select = nullptr;
  Crow2D::Sound::Audioclip *click = nullptr;
  // #endregion

  // #region Crow2D
private:
  void Awake() override;
  // void Update() override;
  void OnDisable() override;
  // #endregion

  // #region Methods
public:
  void UpdatePoints(const int &points);
  void GameOver(const std::string &name, const int &points, const bool &isPersonalBest);
  void Respawn(const int &respawn);
  void SetPause(const bool &pause, const bool &isTutorial = false);

  const int GetPB(const std::string &name) const;
  void SavePB(const std::string &name, const int &points) const;

private:
  std::map<std::string, int> LoadPBs() const;
  void SavePBs(const std::map<std::string, int> &map) const;

  static void OnQuit(const std::string &type, const std::string &payload);
  static void OnPlay(const std::string &type, const std::string &name);
  void OnLeaderboard();

  void PlaySound(const std::string &name);


  // #endregion
};
} // namespace FOUL::Behaviours
