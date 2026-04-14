#include "UIManager.h"
#include <stdexcept>

namespace FOUL::Behaviours {
using namespace Crow2D;
using namespace Crow2D::Components;
using namespace Crow2D::Types;
using namespace Crow2D::Inputs;

void UIManager::Awake() {
  // #region Awake
  renderer = gameObject->GetComponent<UIRenderer>();
  if (!renderer) throw std::runtime_error("UIManager requires a UIRenderer component");

  renderer->bridge->On("__quit", OnQuit);
  // #endregion
}


void UIManager::OnQuit(const std::string &type, const std::string &payload) { Application::Quit(); }
} // namespace FOUL::Behaviours
