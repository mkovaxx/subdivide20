#ifndef _QV_WWW_INLINE_
#define _QV_WWW_INLINE_

#include <QvGroup.hpp>
#include <QvSFEnum.hpp>
#include <QvSFString.hpp>
#include <QvSFVec3f.hpp>

class QvWWWInline : public QvGroup {

    QV_NODE_HEADER(QvWWWInline);

  public:
    // Fields
    QvSFString name;      // URL name
    QvSFVec3f bboxSize;   // Size of 3D bounding box
    QvSFVec3f bboxCenter; // Center of 3D bounding box
};

#endif /* _QV_WWW_INLINE_ */
