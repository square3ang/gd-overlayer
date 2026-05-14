#include "Object.hpp"

Object::Object() { m_uuid = geode::utils::random::generateUUID(); }

matjson::Value Object::serialize() {
  return matjson::makeObject({
      {"name", m_name},
      {"uuid", m_uuid},
      {"x", m_position.x},
      {"y", m_position.y},
      {"pivotX", m_pivot.x},
      {"pivotY", m_pivot.y},
      {"rotation", m_rotation},
      {"scaleX", m_scaleX},
      {"scaleY", m_scaleY},
      {"linkScale", m_linkScale},
  });
}

void Object::deserialize(matjson::Value const &data) {
  m_name = data["name"].asString().unwrapOr(m_name);
  m_uuid = data["uuid"].asString().unwrapOr(m_uuid);
  m_position.x = data["x"].asDouble().unwrapOr(m_position.x);
  m_position.y = data["y"].asDouble().unwrapOr(m_position.y);
  m_pivot.x = data["pivotX"].asDouble().unwrapOr(m_pivot.x);
  m_pivot.y = data["pivotY"].asDouble().unwrapOr(m_pivot.y);
  m_rotation = data["rotation"].asDouble().unwrapOr(m_rotation);
  m_scaleX = data["scaleX"].asDouble().unwrapOr(m_scaleX);
  m_scaleY = data["scaleY"].asDouble().unwrapOr(m_scaleY);
  m_linkScale = data["linkScale"].asBool().unwrapOr(m_linkScale);
}
