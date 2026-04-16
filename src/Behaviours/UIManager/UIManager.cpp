#include "UIManager.h"
#include "Data.h"
#include "MainScene.h"
#include <Crow2D/SceneManager.h>
#include <Crow2D/components/SoundEmitter.h>
#include <Crow2D/dataObjects/AudioClip.h>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <string>

namespace FOUL::Behaviours {
using namespace Crow2D;
using namespace Crow2D::Components;
using namespace Crow2D::Types;
using namespace Crow2D::Inputs;
using namespace Crow2D::Sound;

void UIManager::Awake() {
  // #region Awake

  if (!mainRenderer) throw std::runtime_error("UIManager requires a UIRenderer component");

  emitter = &gameObject->AddComponent<SoundEmitter>();
  SoundEmitter &musicEmitter = gameObject->AddComponent<SoundEmitter>();


  select = new Audioclip("sounds/ui/select.mp3");
  click = new Audioclip("sounds/ui/click.mp3");

  float vol = 0.7f;

  if (gameObject->scene->name == "Menu Scene") music = new Audioclip("sounds/ui/menu.mp3", true);
  else {
    music = new Audioclip("sounds/ui/inGame.mp3", true);
    vol = 0.5f;
  }

  musicEmitter.SetVolume(vol);
  musicEmitter.Play(music);
  mainRenderer->bridge->On("__quit", OnQuit);
  mainRenderer->bridge->On("Play", OnPlay);
  mainRenderer->bridge->On("Leaderboard",
                           [this](const std::string, const std::string) { OnLeaderboard(); });

  mainRenderer->bridge->On("PlaySound",
                           [this](const std::string, const std::string &name) { PlaySound(name); });
  // #endregion
}
void UIManager::OnLeaderboard() {
  std::map<std::string, int> pbs = LoadPBs();
  std::string json = "[";
  for (auto &[name, points] : pbs) {
    json += "{\"name\": \"" + name + "\", \"points\": " + std::to_string(points) + "},";
  }
  if (json.back() == ',') json.pop_back();
  json += "]";

  mainRenderer->bridge->Send("Leaderboard", json);
}


void UIManager::OnQuit(const std::string &type, const std::string &payload) { Application::Quit(); }
void UIManager::OnPlay(const std::string &type, const std::string &name) {
  // #region OnPlay
  Data::currentPlayer = name;
  MainScene *mainScene = new MainScene();
  Scenes::SceneManager::SetSceneAsActive(*mainScene);

  // if (Data::mainScene) delete Data::mainScene;
  // Data::mainScene = mainScene;
  // TODO: add scene object deletion on sceneManager (Crow2D)
  // #endregion
}

void UIManager::UpdatePoints(const int &points) {
  if (!mainRenderer) return;
  mainRenderer->bridge->Send("Points", std::to_string(points));
}

void UIManager::GameOver(const std::string &name, const int &points, const bool &isPersonalBest) {
  if (!mainRenderer) return;
  mainRenderer->bridge->Send(
      "GameOver", "{\"name\": \"" + name + "\", \"points\": \"" + std::to_string(points) +
                      "\", \"isPersonalBest\": " + (isPersonalBest ? "true" : "false") + "}");
}

void UIManager::Respawn(const int &respawn) {
  if (!mainRenderer) return;
  mainRenderer->bridge->Send("Respawn", std::to_string(respawn));
}

void UIManager::SetPause(const bool &pause, const bool &isTutorial) {
  if (!pauseRenderer) return;
  if (pause) pauseRenderer->bridge->Send("Enable", std::to_string(isTutorial));
  else pauseRenderer->bridge->Send("Disable");
}

std::map<std::string, int> UIManager::LoadPBs() const {
  // #region LoadPBs
  std::map<std::string, int> pbs;
  std::ifstream file(SavesFile);

  if (!file || !file.is_open()) return pbs;

  size_t nOfPlayers;
  file.read(reinterpret_cast<char *>(&nOfPlayers), sizeof(nOfPlayers));

  for (short i = 0; i < nOfPlayers; i++) {
    size_t nameLength;
    file.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
    std::string playerName(nameLength, '\0');
    file.read(playerName.data(), nameLength);

    int points;
    file.read(reinterpret_cast<char *>(&points), sizeof(points));

    pbs[playerName] = points;
  }

  return pbs;
  // #endregion
}

void UIManager::SavePBs(const std::map<std::string, int> &pbs) const {
  // #region SavePBs
  std::ofstream file(SavesFile);
  if (!file || !file.is_open()) throw std::runtime_error("Error saving pbs");

  size_t nOfPlayers = pbs.size();
  file.write(reinterpret_cast<const char *>(&nOfPlayers), sizeof(nOfPlayers));

  for (auto &[playerName, points] : pbs) {
    size_t nameLength = playerName.length();
    file.write(reinterpret_cast<const char *>(&nameLength), sizeof(nameLength));
    file.write(playerName.data(), nameLength);

    file.write(reinterpret_cast<const char *>(&points), sizeof(points));
  }
  // #endregion
}

const int UIManager::GetPB(const std::string &name) const {
  std::map<std::string, int> pbs = LoadPBs();
  auto it = pbs.find(name);
  return it == pbs.end() ? -1 : it->second;
}

void UIManager::SavePB(const std::string &name, const int &points) const {
  std::map<std::string, int> pbs = LoadPBs();
  pbs[name] = points;
  SavePBs(pbs);
}
void UIManager::PlaySound(const std::string &name) {

  if (name == "select") emitter->Play(select);
  if (name == "click") emitter->Play(click);
}

void UIManager::OnDisable() {
  delete select;
  delete click;
  delete music;
}

} // namespace FOUL::Behaviours
