#pragma once

#include <string>
#include <Geode/Utils.hpp>

class Object {
public:
    std::string name = "Object";
    std::string uuid;
    float x = 0.0f;
    float y = 0.0f;
    float pivotX = 0.5f;
    float pivotY = 0.5f;
    float rotation = 0.0f;

    bool settingsOpen = false;

    Object() {
        uuid = geode::utils::random::generateUUID();
    }
    virtual void init() {

    }
    virtual void draw() {

    }
    virtual void drawSettings() {

    }
    virtual void update() {

    }

    virtual void destroy() {

    }
    virtual matjson::Value serialize() {
        return matjson::Value::object();
    }
    virtual void deserialize(matjson::Value const& data) {

    }

};