#pragma once

#include <string>
#include <Geode/utils/Random.hpp>

class Object {
public:
    std::string name;
    std::string uuid;
    float x;
    float y;
    float rotation;

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

};