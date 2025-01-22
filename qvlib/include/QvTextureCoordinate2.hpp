#ifndef _QV_TEXTURE_COORDINATE_2_
#define _QV_TEXTURE_COORDINATE_2_

#include <QvMFVec2f.hpp>
#include <QvSubNode.hpp>

class QvTextureCoordinate2 : public QvNode {

    QV_NODE_HEADER(QvTextureCoordinate2);

  public:
    // Fields
    QvMFVec2f point; // TextureCoordinate point(s)
};

#endif /* _QV_TEXTURE_COORDINATE_2_ */
