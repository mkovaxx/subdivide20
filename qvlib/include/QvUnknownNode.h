#ifndef  _QV_UNKNOWN_NODE_
#define  _QV_UNKNOWN_NODE_

#include <QvGroup.h>

class QvUnknownNode : public QvGroup {
  
  //    QV_NODE_HEADER(QvUnknownNode);
  //---------------------------------------

public:								      
  QvUnknownNode();
  virtual ~QvUnknownNode();
  virtual void	traverse(QvState *state);			      
private:								      
  QvBool	firstInstance;					      
  QvFieldData	*unknownFieldData;				              
public:  
  virtual QvFieldData *getFieldData() { return unknownFieldData; }

  //---------------------------------------

public:
  const char	*className;
  //  QvFieldData *instanceFieldData;
  
  void setClassName(const char *name);
};

#endif /* _QV_UNKNOWN_NODE_ */

