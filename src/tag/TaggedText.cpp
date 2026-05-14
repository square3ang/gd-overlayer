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

std::string TaggedText::render(bool isPlaying, bool isSimulating) {
  compile();
  return TagRegistry::get().renderSegments(m_segments, isPlaying, isSimulating);
}

void TaggedText::apply(CCLabelBMFont *label, bool isPlaying, bool isSimulating) {
  if (!label)
    return;
  label->setString(render(isPlaying, isSimulating).c_str());
}
