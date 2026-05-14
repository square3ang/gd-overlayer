#pragma once

#include <Geode/Utils.hpp>
#include <string>
using namespace geode::prelude;

class Object {
public:
  virtual ~Object() = default;
  std::string m_name = "Object";
  std::string m_uuid;
  CCPoint m_position = ccp(0.0f, 0.0f);
  CCPoint m_pivot = ccp(0.5f, 0.5f);
  float m_rotation = 0.0f;
  float m_scaleX = 0.5f;
  float m_scaleY = 0.5f;
  bool m_linkScale = true;

  bool m_settingsOpen = false;

  Object();
  virtual void init() {}
  virtual void draw() {}
  virtual void drawSettings() {}
  virtual void update() {}
  virtual void everyFrame() {}

  virtual void destroy() {}
  virtual matjson::Value serialize();
  virtual void deserialize(matjson::Value const &data);
};