#pragma once
#include "objects/Object.hpp"
#include <memory>
#include <vector>

class Overlayer {
public:
  static Overlayer *get();

  std::vector<std::unique_ptr<Object>> m_objects;
  bool m_settingsOpen = false;

  void save();
  void load();

  void registerTags();
  void renderGUI();
  void setupGUI();

private:
  Overlayer() = default;
};
