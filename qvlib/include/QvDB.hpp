#ifndef _QV_DB_
#define _QV_DB_

#include <QvBasic.hpp>

class QvInput;
class QvNode;
class QvField;

class QvDB {
  public:
    static const char* versionString;

    static void init();
    static QvBool read(QvInput* in, QvNode*& rootNode);
};

#endif /* _QV_DB_ */
