#pragma once
#include "../TagRegistry.hpp"
#include <string_view>

class GamePlay {
public:
  static TagValue Attempts(std::string_view arg, bool isSimulation) {
    int attempts = 0;
    if (isSimulation) {
      attempts = (arg == "total") ? 6767 : 67;
    } else {
      auto playLayer = PlayLayer::get();
      if (!playLayer)
        return 0;
      attempts = (arg == "total") ? playLayer->m_level->m_attempts
                                  : playLayer->m_attempts;
    }
    return attempts;
  }

  static TagValue Progress(std::string_view arg, bool isSimulation) {
    float currentPercent = 0.0f;
    if (isSimulation) {
      currentPercent = 67.6767676767676767f;
    } else {
      auto playLayer = PlayLayer::get();
      if (!playLayer)
        return 0.0f;
      currentPercent = playLayer->getCurrentPercent();
    }

    if (!arg.empty()) {
      bool stripZeros = arg.back() == '#';
      std::string_view precisionArg =
          stripZeros ? arg.substr(0, arg.size() - 1) : arg;

      int precision =
          geode::utils::numFromString<int>(precisionArg).unwrapOr(0);
      if (precision > 10)
        precision = 10;
      if (precision < 0)
        precision = 0;

      std::string formatted = fmt::format("{:.{}f}", currentPercent, precision);

      if (stripZeros && formatted.find('.') != std::string::npos) {
        formatted.erase(formatted.find_last_not_of('0') + 1, std::string::npos);
        if (formatted.back() == '.') {
          formatted.pop_back();
        }
      }
      return formatted;
    }

    return currentPercent;
  }

  static void registerTags() {
    auto &reg = TagRegistry::get();
    reg.registerTag("Attempts", Attempts, false);
    reg.registerTag("Progress", Progress, false);
  }
};