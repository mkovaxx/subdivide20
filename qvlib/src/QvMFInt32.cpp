#include <QvMFInt32.hpp>

QV_MFIELD_SOURCE(QvMFInt32, int, 1, FALSE)

QvBool QvMFInt32::read1Value(QvInput* in, int index) { return in->read(values[index]); }
