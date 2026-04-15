#include "UIManager.h"
#include "Data.h"
#include "MainScene.h"
#include <Crow2D/SceneManager.h>
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

void UIManager::Awake() {
  // #region Awake

  if (!mainRenderer) throw std::runtime_error("UIManager requires a UIRenderer component");

  mainRenderer->bridge->On("__quit", OnQuit);
  mainRenderer->bridge->On("Play", OnPlay);
  // #endregion
}


void UIManager::OnQuit(const std::string &type, const std::string &payload) { Application::Quit(); }
void UIManager::OnPlay(const std::string &type, const std::string &name) {
  // #region OnPlay
  Data::currentPlayer = name;
  MainScene *mainScene = new MainScene();
  Scenes::SceneManager::SetSceneAsActive(*mainScene);

  if (Data::mainScene) delete Data::mainScene;
  Data::mainScene = mainScene;
  // TODO: add scene object deletion on sceneManager (Crow2D)
  // #endregion
}

void UIManager::UpdatePoints(const int &points) {
  if (!mainRenderer) return;
  mainRenderer->bridge->Send("Points", std::to_string(points));
}
void UIManager::GameOver(const std::string &name, const int &points, const bool &isPersonalBest) {
  if (!mainRenderer) return;
  printf("Saving %s %s\n", name.c_str(), std::to_string(points).c_str());
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

} // namespace FOUL::Behaviours
