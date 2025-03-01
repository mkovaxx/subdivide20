#ifndef _QV_SEPARATOR_
#define _QV_SEPARATOR_

#include <QvGroup.hpp>
#include <QvSFEnum.hpp>

#include <QvSFBitMask.hpp>

class QvSeparator : public QvGroup {

    QV_NODE_HEADER(QvSeparator);

  public:
    enum CullEnabled { // Possible values for culling
        OFF,           // Never cull
        ON,            // Always cull
        AUTO           // Decide based on some heuristic
    };

    // Fields
    QvSFEnum renderCulling;
};

#endif /* _QV_SEPARATOR_ */
