#ifndef _QV_OUT_
#define _QV_OUT_

#include <QvBasic.h>

// HB: Warning!  This class does not write all Inventor nodes, only
// the ones which are needed for our subdivision code (indexed face
// sets, line sets, material properties, matrix transforms, camera,
// ...).  Extending it to handle more nodes should not be too
// difficult.

class QvNode;

class QvOut {
  public:
    QvOut(char* filename);
    void write(QvNode* rootNode);
    QvBool error() { return _error; }

  private:
    char filename[80];

  public:
    static QvBool _error;
};

#endif /* _QV_OUT_ */
