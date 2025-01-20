#ifndef _QV_WWW_INLINE_
#define _QV_WWW_INLINE_

#include <QvGroup.h>
#include <QvSFEnum.h>
#include <QvSFString.h>
#include <QvSFVec3f.h>

class QvWWWInline : public QvGroup {

    QV_NODE_HEADER(QvWWWInline);

  public:
    // Fields
    QvSFString name;      // URL name
    QvSFVec3f bboxSize;   // Size of 3D bounding box
    QvSFVec3f bboxCenter; // Center of 3D bounding box
};

#endif /* _QV_WWW_INLINE_ */
