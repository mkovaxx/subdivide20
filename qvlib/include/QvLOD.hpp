#ifndef _QV_LEVEL_OF_DETAIL_
#define _QV_LEVEL_OF_DETAIL_

#include <QvGroup.hpp>
#include <QvMFFloat.hpp>
#include <QvSFVec3f.hpp>

class QvLOD : public QvGroup {

    QV_NODE_HEADER(QvLOD);

  public:
    // Fields
    QvMFFloat range;  // Distance ranges for LOD switching
    QvSFVec3f center; // Center for distance computation
};

#endif /* _QV_LEVEL_OF_DETAIL_ */
