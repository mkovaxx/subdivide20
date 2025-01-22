#ifndef _QV_TRANSFORM_
#define _QV_TRANSFORM_

#include <QvSFRotation.hpp>
#include <QvSFVec3f.hpp>
#include <QvSubNode.hpp>

class QvTransform : public QvNode {

    QV_NODE_HEADER(QvTransform);

  public:
    // Fields
    QvSFVec3f translation;         // Translation vector
    QvSFRotation rotation;         // Rotation
    QvSFVec3f scaleFactor;         // Scale factors
    QvSFRotation scaleOrientation; // Defines rotational space for scale
    QvSFVec3f center;              // Center point for scale and rotate
};

#endif /* _QV_TRANSFORM_ */
