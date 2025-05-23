#ifndef _QV_CUBE_
#define _QV_CUBE_

#include <QvSFFloat.hpp>
#include <QvSubNode.hpp>

class QvCube : public QvNode {

    QV_NODE_HEADER(QvCube);

  public:
    // Fields
    QvSFFloat width;  // Size in x dimension
    QvSFFloat height; // Size in y dimension
    QvSFFloat depth;  // Size in z dimension
};

#endif /* _QV_CUBE_ */
