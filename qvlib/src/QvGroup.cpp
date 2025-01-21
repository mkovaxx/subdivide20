#include <QvChildList.h>
#include <QvFieldData.h>
#include <QvGroup.h>
#include <QvInput.h>
#include <QvReadError.h>

QV_NODE_SOURCE(QvGroup);

QvGroup::QvGroup() {
    children = new QvChildList();
    QV_NODE_CONSTRUCTOR(QvGroup);
    isBuiltIn = TRUE;
}

QvGroup::~QvGroup() { delete children; }

QvNode* QvGroup::getChild(int index) const { return (*children)[index]; }

int QvGroup::getNumChildren() const { return children->getLength(); }

QvChildList* QvGroup::getChildren() const { return children; }

QvBool QvGroup::readInstance(QvInput* in) {

    //  std::cerr<<"QvGroup::readInstance("<<this<<","<<in<<")"<<std::endl;

    QvName typeString;
    QvFieldData* fieldData = getFieldData();

    if (!isBuiltIn) {
        if (in->read(typeString, TRUE)) {
            if (typeString == "fields") {
                if (!fieldData->readFieldTypes(in, this)) {
                    QvReadError::post(in, "Bad field specifications for node");
                    return FALSE;
                } else {
                    ;
                    //	    std::cerr<<"couldn't read field types"<<std::endl;
                }
            } else {
                ;
                //	  std::cerr<<"got no fields!"<<std::endl;
            }
        } else {
            //	std::cerr<<"got no string!"<<std::endl;
            in->putBack(typeString.getString());
        }
    } else {
        ;
        //      std::cerr<<"buildin!"<<std::endl;
    }

    return (fieldData->read(in, this, FALSE) && readChildren(in));
}

QvBool QvGroup::readChildren(QvInput* in) {
    //  std::cerr<<"QvGroup::readChildren("<<this<<","<<in<<")"<<std::endl;

    QvNode* child;
    QvBool ret = TRUE;

    while (TRUE) {
        if (read(in, child)) {
            if (child != NULL) {
                children->append(child);
            } else {
                break;
            }
        } else {
            ret = FALSE;
            break;
        }
    }

    return ret;
}
