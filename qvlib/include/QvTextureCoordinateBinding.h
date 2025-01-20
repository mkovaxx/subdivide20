#ifndef _QV_TEXTURE_COORDINATE_BINDING_
#define _QV_TEXTURE_COORDINATE_BINDING_

#include <QvSFEnum.h>
#include <QvSubNode.h>

class QvTextureCoordinateBinding : public QvNode {

    QV_NODE_HEADER(QvTextureCoordinateBinding);

  public:
    enum Binding {
        DEFAULT,
        OVERALL,
        PER_PART,
        PER_PART_INDEXED,
        PER_FACE,
        PER_FACE_INDEXED,
        PER_VERTEX,
        PER_VERTEX_INDEXED
    };

    // Fields:
    QvSFEnum value;
};

#endif /* _QV_TEXTURE_COORDINATE_BINDING_ */
