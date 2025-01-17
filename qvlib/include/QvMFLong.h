#ifndef  _QV_MF_LONG_
#define  _QV_MF_LONG_

#include <QvSubField.h>

class QvMFLong : public QvMField {
  public:
    long *values;
    void set1Value(int n, int val) {
      if (n >= num) {
	allocValues(num+1);
	values[num] = val;
	num += 1;
      } else {
	values[n] = val;
      }
    }
    QV_MFIELD_HEADER(QvMFLong);
};

#endif /* _QV_MF_LONG_ */
