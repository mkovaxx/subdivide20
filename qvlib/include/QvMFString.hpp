#ifndef _QV_MF_STRING_
#define _QV_MF_STRING_

#include <QvSubField.hpp>

class QvMFString : public QvMField {
  public:
    QvString* values;
    QV_MFIELD_HEADER(QvMFString);
};

#endif /* _QV_MF_STRING_ */
