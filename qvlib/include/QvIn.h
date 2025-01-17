#ifndef _QV_IN_
#define _QV_IN_

#include <QvInput.h>

class QvSeparator;

class QvIn {
public:
  QvIn(char* filename);
  QvSeparator* read();
  QvBool error() const { return _error; }
private:
  QvInput qvInput;
  QvBool _error;
};

#endif /* _QV_IN_ */
