#include "../TagRegistry.hpp"
#include <string_view>

class GamePlay {
public:
  static TagValue Attempts(std::string_view arg) {
    auto playLayer = PlayLayer::get();
    if (!playLayer)
      return 0;

    if (arg == "total") {
      return playLayer->m_level->m_attempts;
    }

    return playLayer->m_attempts;
  }
};