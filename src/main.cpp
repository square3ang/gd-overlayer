#include <Geode/Geode.hpp>

#include "Overlayer.hpp"
#include "tag/TagRegistry.hpp"

using namespace geode::prelude;

#include <Geode/modify/AppDelegate.hpp>
class $modify(MyAppDelegate, AppDelegate) {
  void trySaveGame(bool force) {
    AppDelegate::trySaveGame(force);
    Overlayer::get()->save();
  }
};

#include <Geode/modify/CCDirector.hpp>
class $modify(MyCCDirector, CCDirector) {
  void drawScene() {
    CCDirector::drawScene();
    for (auto &obj : Overlayer::get()->m_objects) {
      obj->everyFrame();
    }
  }

  void willSwitchToScene(CCScene *scene) {
    CCDirector::willSwitchToScene(scene);
    TagRegistry::get().clearCache();
  }
};

$on_mod(Loaded) {
  listenForKeybindSettingPresses(
      "toggle-menu",
      [](Keybind const &keybind, bool down, bool repeat, double timestamp) {
        if (down)
          Overlayer::get()->m_settingsOpen = !Overlayer::get()->m_settingsOpen;
      });

  auto overlayer = Overlayer::get();
  overlayer->load();
  overlayer->registerTags();
  overlayer->setupGUI();
}