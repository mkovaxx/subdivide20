#include <QvMFLong.hpp>

QV_MFIELD_SOURCE(QvMFLong, long, 1, FALSE)

QvBool QvMFLong::read1Value(QvInput* in, int index) { return in->read(values[index]); }
