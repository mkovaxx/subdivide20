#ifndef _QV_SPOT_LIGHT_
#define _QV_SPOT_LIGHT_

#include <QvSFBool.hpp>
#include <QvSFColor.hpp>
#include <QvSFFloat.hpp>
#include <QvSFVec3f.hpp>
#include <QvSubNode.hpp>

class QvSpotLight : public QvNode {

    QV_NODE_HEADER(QvSpotLight);

  public:
    // Fields:
    QvSFBool on;           // Whether light is on
    QvSFFloat intensity;   // Source intensity (0 to 1)
    QvSFColor color;       // RGB source color
    QvSFVec3f location;    // Source location
    QvSFVec3f direction;   // Primary direction of illumination
    QvSFFloat dropOffRate; // Rate of intensity drop-off from primary
                           // direction: 0 = constant intensity,
                           // 1 = sharp drop-off
    QvSFFloat cutOffAngle; // Angle (in radians) outside of which
                           // intensity is zero, measured from
                           // edge of cone to other edge
};

#endif /* _QV_SPOT_LIGHT_ */
