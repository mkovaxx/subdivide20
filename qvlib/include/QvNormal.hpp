#ifndef _QV_NORMAL_
#define _QV_NORMAL_

#include <QvMFVec3f.hpp>
#include <QvSubNode.hpp>

class QvNormal : public QvNode {

    QV_NODE_HEADER(QvNormal);

  public:
    // Fields
    QvMFVec3f vector; // Normal vector(s)
};

#endif /* _QV_NORMAL_ */
