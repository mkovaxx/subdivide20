#include <QvInput.h>
#include <QvReadError.h>
#include <QvFieldData.h>
#include <QvChildList.h>
#include <QvGroup.h>

QV_NODE_SOURCE(QvGroup);

QvGroup::QvGroup()
{
    children = new QvChildList();
    QV_NODE_CONSTRUCTOR(QvGroup);
    isBuiltIn = TRUE;
}

QvGroup::~QvGroup()
{
    delete children;
}

QvNode *
QvGroup::getChild(int index) const
{
    return(*children)[index];
}

int
QvGroup::getNumChildren() const
{
    return children->getLength();
}

QvChildList *
QvGroup::getChildren() const
{
    return children;
}

QvBool
QvGroup::readInstance(QvInput *in)
{
  
  //  cerr<<"QvGroup::readInstance("<<this<<","<<in<<")"<<endl;

    QvName	typeString;
    QvFieldData	*fieldData = getFieldData();

    if (! isBuiltIn) {
      if (in->read(typeString, TRUE)) {
	if (typeString == "fields") {
	  if (! fieldData->readFieldTypes(in, this)) {
	    QvReadError::post(in, "Bad field specifications for node");
	    return FALSE;
	  } else {
	    ;
	    //	    cerr<<"couldn't read field types"<<endl;
	  }
	} else {
	  ;
	  //	  cerr<<"got no fields!"<<endl;
	}
      } else {
	//	cerr<<"got no string!"<<endl;
	in->putBack(typeString.getString());
      }
    } else {
      ;
      //      cerr<<"buildin!"<<endl;
    }
    
    return (fieldData->read(in, this, FALSE) && readChildren(in));
}

QvBool
QvGroup::readChildren(QvInput *in)
{
  //  cerr<<"QvGroup::readChildren("<<this<<","<<in<<")"<<endl;

    QvNode	*child;
    QvBool	ret = TRUE;

    while (TRUE) {
	if (read(in, child)) {
	    if (child != NULL)
		children->append(child);
	    else
		break;
	}
	else {
	    ret = FALSE;
	    break;
	}
    }

    return ret;
}
