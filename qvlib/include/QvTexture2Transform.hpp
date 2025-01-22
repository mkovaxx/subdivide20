#ifndef _QV_TEXTURE_2_TRANSFORM_
#define _QV_TEXTURE_2_TRANSFORM_

#include <QvSFFloat.hpp>
#include <QvSFVec2f.hpp>
#include <QvSubNode.hpp>

class QvTexture2Transform : public QvNode {

    QV_NODE_HEADER(QvTexture2Transform);

  public:
    // Fields
    QvSFVec2f translation; // Translation vector
    QvSFFloat rotation;    // Rotation
    QvSFVec2f scaleFactor; // Scale factors
    QvSFVec2f center;      // Center point for scale and rotate
};

#endif /* _QV_TEXTURE_2_TRANSFORM_ */
