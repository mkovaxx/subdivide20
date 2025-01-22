#ifndef _QV_SF_USHORT_
#define _QV_SF_USHORT_

#include <QvSubField.hpp>

class QvSFUShort : public QvSField {
  public:
    unsigned short value;
    QV_SFIELD_HEADER(QvSFUShort);
};

#endif /* _QV_SF_USHORT_ */
