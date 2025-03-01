#ifndef _QV_COORDINATE3_
#define _QV_COORDINATE3_

#include <QvMFVec3f.hpp>
#include <QvSubNode.hpp>

class QvCoordinate3 : public QvNode {

    QV_NODE_HEADER(QvCoordinate3);

  public:
    // Fields
    QvMFVec3f point; // Coordinate point(s)
};

#endif /* _QV_COORDINATE3_ */
