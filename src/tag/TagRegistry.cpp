#include "TagRegistry.hpp"
#include "../ctre/ctre.hpp"
#include <fmt/format.h>
#include <iterator>

TagRegistry &TagRegistry::get() {
  static TagRegistry instance;
  return instance;
}

void TagRegistry::registerTag(const std::string &name,
                              std::function<TagValue()> func,
                              bool availableInIdle) {
  m_tags[name] = {[func](std::string_view, bool) { return func(); },
                  availableInIdle};
}

void TagRegistry::registerTag(const std::string &name, TagHandler handler,
                              bool availableInIdle) {
  m_tags[name] = {handler, availableInIdle};
}

std::vector<FormatSegment> TagRegistry::parse(std::string_view input) {
  std::vector<FormatSegment> segments;
  size_t last_pos = 0;

  for (auto match : ctre::search_all<"\\{([^{}:]+)(?::([^{}]+))?\\}">(input)) {
    auto full_match = match.template get<0>();
    size_t match_start = full_match.data() - input.data();

    if (match_start > last_pos) {
      segments.push_back(
          {false, std::string(input.substr(last_pos, match_start - last_pos)),
           ""});
    }

    segments.push_back({true, std::string(match.template get<1>().to_view()),
                        match.template get<2>()
                            ? std::string(match.template get<2>().to_view())
                            : ""});

    last_pos = match_start + full_match.size();
  }

  if (last_pos < input.size()) {
    segments.push_back({false, std::string(input.substr(last_pos)), ""});
  }
  return segments;
}

std::string
TagRegistry::renderSegments(const std::vector<FormatSegment> &segments,
                            bool isPlaying) {
  std::string result;

  for (const auto &seg : segments) {
    if (seg.isTag) {
      if (m_tags.contains(seg.text)) {
        const auto &tagData = m_tags[seg.text];
        if (isPlaying || tagData.availableInIdle || m_simulationMode) {
          bool isSimulation = m_simulationMode && !isPlaying;
          TagValue value = tagData.handler(seg.tagArg, isSimulation);

          std::visit(
              [&result](auto &&val) {
                fmt::format_to(std::back_inserter(result), "{}", val);
              },
              value);
        } else {
          fmt::format_to(std::back_inserter(result), "{{{}{}}}", seg.text,
                         seg.tagArg.empty() ? "" : ":" + seg.tagArg);
        }
      } else {
        fmt::format_to(std::back_inserter(result), "{{{}{}}}", seg.text,
                       seg.tagArg.empty() ? "" : ":" + seg.tagArg);
      }
    } else {
      result.append(seg.text);
    }
  }

  return result;
}

std::string TagRegistry::process(std::string_view input, bool isPlaying) {
  return renderSegments(parse(input), isPlaying);
}