#include <QvUnknownNode.h>

//QV_NODE_SOURCE(QvUnknownNode);

QvUnknownNode::QvUnknownNode()
{

  //QV_NODE_CONSTRUCTOR(QvUnknownNode);
  isBuiltIn = FALSE;
  //--------------------------------
  unknownFieldData = 0;
  firstInstance = TRUE;

  className = NULL;

  unknownFieldData = new QvFieldData;

}

void
QvUnknownNode::setClassName(const char *name)
{
  char* tmps = (char*)malloc(sizeof(char)*(strlen(name)+1));
  strcpy(tmps, name);
  className = tmps;
}

QvUnknownNode::~QvUnknownNode()
{

//  for (int i = 0; i < unknownFieldData->getNumFields(); i++)
//   (unknownFieldData->getField(this, i))->~QvFieldData();
  
  delete unknownFieldData;
  
  if (className != NULL)
    free((void *) className);
  
}
