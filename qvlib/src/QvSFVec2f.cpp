#include <QvSFVec2f.hpp>

QV_SFIELD_SOURCE(QvSFVec2f)

QvBool QvSFVec2f::readValue(QvInput* in) { return (in->read(value[0]) && in->read(value[1])); }
