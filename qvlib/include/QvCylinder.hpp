#ifndef _QV_CYLINDER_
#define _QV_CYLINDER_

#include <QvSFBitMask.hpp>
#include <QvSFFloat.hpp>
#include <QvSubNode.hpp>

class QvCylinder : public QvNode {

    QV_NODE_HEADER(QvCylinder);

  public:
    enum Part {        // Cylinder parts
        SIDES = 0x01,  // The tubular part
        TOP = 0x02,    // The top circular face
        BOTTOM = 0x04, // The bottom circular face
        ALL = 0x07     // All parts
    };

    // Fields
    QvSFBitMask parts; // Visible parts of cylinder
    QvSFFloat radius;  // Radius in x and z dimensions
    QvSFFloat height;  // Size in y dimension
};

#endif /* _QV_CYLINDER_ */
