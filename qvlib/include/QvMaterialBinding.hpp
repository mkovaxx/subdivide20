#ifndef _QV_MATERIAL_BINDING_
#define _QV_MATERIAL_BINDING_

#include <QvSFEnum.hpp>
#include <QvSubNode.hpp>

class QvMaterialBinding : public QvNode {

    QV_NODE_HEADER(QvMaterialBinding);

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

#endif /* _QV_MATERIAL_BINDING_ */
