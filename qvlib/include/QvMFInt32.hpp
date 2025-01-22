#ifndef _QV_MF_INT32_
#define _QV_MF_INT32_

#include <QvSubField.hpp>

class QvMFInt32 : public QvMField {
  public:
    int* values;
    QV_MFIELD_HEADER(QvMFInt32);
};

#endif /* _QV_MF_INT32_ */
