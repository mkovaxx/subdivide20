#ifndef _QV_POINT_LIGHT_
#define _QV_POINT_LIGHT_

#include <QvSFBool.hpp>
#include <QvSFColor.hpp>
#include <QvSFFloat.hpp>
#include <QvSFVec3f.hpp>
#include <QvSubNode.hpp>

class QvPointLight : public QvNode {

    QV_NODE_HEADER(QvPointLight);

  public:
    // Fields
    QvSFBool on;         // Whether light is on
    QvSFFloat intensity; // Source intensity (0 to 1)
    QvSFColor color;     // RGB source color
    QvSFVec3f location;  // Source location
};

#endif /* _QV_POINT_LIGHT_ */
