#include "TaggedText.hpp"

void TaggedText::setRaw(std::string_view raw) {
  if (m_raw != raw) {
    m_raw = raw;
    m_dirty = true;
  }
}

void TaggedText::compile() {
  if (!m_dirty)
    return;
  m_segments = TagRegistry::get().parse(m_raw);
  m_dirty = false;
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
