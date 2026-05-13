#pragma once

#include "TagRegistry.hpp"
#include <Geode/Geode.hpp>
#include <string>
#include <string_view>
#include <vector>

using namespace geode::prelude;

class TaggedText {
  std::string m_raw;
  std::vector<FormatSegment> m_segments;
  bool m_dirty = true;

public:
  TaggedText() = default;
  TaggedText(std::string_view raw) : m_raw(raw), m_dirty(true) {}

  void setRaw(std::string_view raw);
  const std::string &getRaw() const { return m_raw; }

  void compile();
  std::string render(bool isPlaying);
  void apply(CCLabelBMFont *label, bool isPlaying);
};
