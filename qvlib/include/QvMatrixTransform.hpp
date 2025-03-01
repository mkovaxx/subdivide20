#ifndef _QV_MATRIX_TRANSFORM_
#define _QV_MATRIX_TRANSFORM_

#include <QvSFMatrix.hpp>
#include <QvSubNode.hpp>

class QvMatrixTransform : public QvNode {

    QV_NODE_HEADER(QvMatrixTransform);

  public:
    // Fields
    QvSFMatrix matrix; // Transformation matrix
};

#endif /* _QV_MATRIX_TRANSFORM_ */
