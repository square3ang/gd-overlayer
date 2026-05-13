#pragma once
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>

using TagValue = std::variant<std::string, int, float, double>;
using TagHandler = std::function<TagValue(std::string_view)>;

class TagRegistry {
public:
  static TagRegistry &get();

  void registerTag(const std::string &name, std::function<TagValue()> func,
                   bool availableInIdle = true);

  void registerTag(const std::string &name, TagHandler handler,
                   bool availableInIdle = true);

  std::string process(std::string_view input, bool isPlaying);
  void clearCache();

private:
  struct TagData {
    TagHandler handler;
    bool availableInIdle;
  };

  struct FormatSegment {
    bool isTag;
    std::string text;
    std::string tagArg;
  };

  TagRegistry() = default;
  std::unordered_map<std::string, TagData> m_tags;
  std::unordered_map<std::string, std::vector<FormatSegment>> m_cache;
};
