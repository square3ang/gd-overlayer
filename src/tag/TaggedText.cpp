#include "TaggedText.hpp"

void TaggedText::setRaw(std::string_view raw) {
  if (m_rawText != raw) {
    m_rawText = raw;
    m_isDirty = true;
  }
}

void TaggedText::compile() {
  if (!m_isDirty)
    return;
  m_segments = TagRegistry::get().parse(m_rawText);
  m_isDirty = false;
}

std::string TaggedText::render(bool isPlaying) {
  compile();
  return TagRegistry::get().renderSegments(m_segments, isPlaying);
}

void TaggedText::apply(CCLabelBMFont *label, bool isPlaying) {
  if (!label)
    return;
  label->setString(render(isPlaying).c_str());
}
