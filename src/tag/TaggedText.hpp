#pragma once

#include "TagRegistry.hpp"
#include <Geode/Geode.hpp>
#include <string>
#include <string_view>
#include <vector>

using namespace geode::prelude;

class TaggedText {
  std::string m_rawText;
  std::vector<FormatSegment> m_segments;
  bool m_isDirty = true;

public:
  TaggedText() = default;
  TaggedText(std::string_view raw) : m_rawText(raw), m_isDirty(true) {}

  void setRaw(std::string_view raw);
  const std::string &getRaw() const { return m_rawText; }

  void compile();
  std::string render(bool isPlaying, bool isSimulating);
  void apply(CCLabelBMFont *label, bool isPlaying, bool isSimulating);
};
