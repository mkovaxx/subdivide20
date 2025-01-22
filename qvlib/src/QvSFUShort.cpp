#include <QvSFUShort.hpp>

QV_SFIELD_SOURCE(QvSFUShort)

QvBool QvSFUShort::readValue(QvInput* in) { return in->read(value); }
