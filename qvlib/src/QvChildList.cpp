#include <QvChildList.hpp>

QvChildList::QvChildList() : QvNodeList() {}

QvChildList::~QvChildList() { truncate(0); }
