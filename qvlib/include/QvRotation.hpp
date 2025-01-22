#ifndef _QV_ROTATION_
#define _QV_ROTATION_

#include <QvSFRotation.hpp>
#include <QvSubNode.hpp>

class QvRotation : public QvNode {

    QV_NODE_HEADER(QvRotation);

  public:
    // Fields
    QvSFRotation rotation; // Rotation
};

#endif /* _QV_ROTATION_ */
