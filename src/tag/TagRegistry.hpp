#pragma once
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

using TagValue = std::variant<std::string, int, float, double>;
using TagHandler = std::function<TagValue(std::string_view, bool)>;

struct FormatSegment {
  bool isTag;
  std::string text;
  std::string tagArg;
};

class TagRegistry {
public:
  static TagRegistry &get();

  void registerTag(const std::string &name, std::function<TagValue()> func,
                   bool availableInIdle = true);

  void registerTag(const std::string &name, TagHandler handler,
                   bool availableInIdle = true);

  std::string process(std::string_view input, bool isPlaying, bool isSimulating);

  std::vector<FormatSegment> parse(std::string_view input);
  std::string renderSegments(const std::vector<FormatSegment> &segments,
                             bool isPlaying, bool isSimulating);



private:
  struct TagData {
    TagHandler handler;
    bool availableInIdle;
  };

  TagRegistry() = default;
  std::unordered_map<std::string, TagData> m_tags;
};
