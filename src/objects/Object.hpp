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

  bool m_settingsOpen = false;

  Object() { m_uuid = geode::utils::random::generateUUID(); }
  virtual void init() {}
  virtual void draw() {}
  virtual void drawSettings() {}
  virtual void update() {}
  virtual void everyFrame() {}

  virtual void destroy() {}
  virtual matjson::Value serialize() { return matjson::Value::object(); }
  virtual void deserialize(matjson::Value const &data) {}
};