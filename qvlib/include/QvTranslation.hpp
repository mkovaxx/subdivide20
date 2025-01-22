#ifndef _QV_TRANSLATION_
#define _QV_TRANSLATION_

#include <QvSFVec3f.hpp>
#include <QvSubNode.hpp>

class QvTranslation : public QvNode {

    QV_NODE_HEADER(QvTranslation);

  public:
    // Fields
    QvSFVec3f translation; // Translation vector
};

#endif /* _QV_TRANSLATION_ */
