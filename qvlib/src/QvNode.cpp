#include <QvDB.hpp>
#include <QvDebugError.hpp>
#include <QvField.hpp>
#include <QvFieldData.hpp>
#include <QvInput.hpp>
#include <QvNodes.hpp>
#include <QvReadError.hpp>
#include <QvUnknownNode.hpp>
#include <ctype.h>
#include <stdio.h>

// The global name dictionary
QvDict* QvNode::nameDict = NULL;

// Syntax for writing instances to files
#define OPEN_BRACE '{'
#define CLOSE_BRACE '}'
#define DEFINITION_KEYWORD "DEF"
#define REFERENCE_KEYWORD "USE"
#define NULL_KEYWORD "NULL"

void QvNode::init() {
    if (nameDict == NULL) {
        nameDict = new QvDict;
    }
    //    std::cerr<<"QvNode::init()"<<std::endl;
}

QvNode::QvNode() {
    //  std::cerr<<"QvNode::QvNode("<<this<<")"<<std::endl;
    refCount = 0;
    objName = new QvName("");
}

QvNode::~QvNode() {
    if (!!(*objName)) {
        removeName(this, objName->getString());
    }
    delete objName;
}

void QvNode::ref() const { ((QvNode*)this)->refCount++; }

void QvNode::unref() const {
    if (--((QvNode*)this)->refCount == 0) {
        delete (QvNode*)this;
    }
}

void QvNode::unrefNoDelete() const { ((QvNode*)this)->refCount--; }

const QvName& QvNode::getName() const { return *objName; }

void QvNode::setName(const QvName& newName) {
    if (!!(*objName)) {
        removeName(this, objName->getString());
    }
    delete objName;

    const char* str = newName.getString();
    QvBool isBad = 0;

    if (newName.getLength() > 0 && !QvName::isNodeNameStartChar(str[0])) {
        isBad = TRUE;
    }

    int i;
    for (i = 1; i < newName.getLength() && !isBad; i++) {
        isBad = !QvName::isNodeNameChar(str[i]);
    }

    if (isBad) {
        QvString goodString;

        if (!QvName::isNodeNameStartChar(str[0])) {
            goodString += "_";
        }
        for (i = 0; i < newName.getLength(); i++) {
            char temp[2];
            temp[0] = str[i];
            temp[1] = '\0';
            if (!QvName::isNodeNameChar(str[i])) {
                goodString += "_";
            } else {
                goodString += temp;
            }
        }
#ifdef DEBUG
        QvDebugError::post("QvNode::setName",
                           "Bad characters in"
                           " name '%s'.  Replacing with name '%s'",
                           str, goodString.getString());
#endif
        objName = new QvName(goodString);
    } else {
        objName = new QvName(newName);
    }
    if (!!(*objName)) {
        addName(this, objName->getString());
    }
}

void QvNode::addName(QvNode* b, const char* name) {
    QvPList* list;
    void* t;
    if (!nameDict->find((unsigned long)name, t)) {
        list = new QvPList;
        nameDict->enter((unsigned long)name, list);
    } else {
        list = (QvPList*)t;
    }

    list->append(b);
}

void QvNode::removeName(QvNode* b, const char* name) {
    QvPList* list;
    QvBool found;
    void* t;
    int i;

    found = nameDict->find((unsigned long)name, t);

    if (found) {
        list = (QvPList*)t;
        i = list->find(b);

        if (i < 0) {
            found = FALSE;
        }

        else {
            list->remove(i);
        }
    }

    if (!found) {
        QvDebugError::post("QvNode::removeName", "Name \"%s\" (node %x) is not in dictionary", name, b);
    }
}

QvBool QvNode::read(QvInput* in, QvNode*& node) {

    //  std::cerr<<"QvNode::read("<<","<<in<<","<<node<<")"<<std::endl;

    QvBool ret;
    QvName name;

    if (!in->read(name, TRUE)) {
        node = NULL;
        ret = in->headerOk;
    }

    else if (!name || name == NULL_KEYWORD) {
        node = NULL;
        ret = TRUE;
    }

    else if (name == REFERENCE_KEYWORD) {
        node = readReference(in);
        ret = (node != NULL);
    }

    else {
        //      printf("name!!!!\t%s\n",name.getString());
        ret = readNode(in, name, node);
    }
    return ret;
}

QvBool QvNode::readInstance(QvInput* in) {

    //  std::cerr<<"QvNode::readInstance("<<this<<","<<in<<")"<<std::endl;

    QvName typeString;
    QvFieldData* fieldData = getFieldData();

    if (in->read(typeString, TRUE)) {
        //      std::cerr<<"got:"<<typeString.getString()<<std::endl;
        if (typeString == "fields") {
            if (!fieldData->readFieldTypes(in, this)) {
                QvReadError::post(in, "Bad field specifications for node");
                return FALSE;
            }
        } else {
            in->putBack(typeString.getString());
        }
    } else {
        ;
        //      std::cerr<<"got no string!"<<std::endl;
    }

    if (!fieldData->read(in, this)) {
        return FALSE;
    }

    return TRUE;
}

QvNode* QvNode::readReference(QvInput* in) {

    //  std::cerr<<"QvNode::readReference("<<in<<")"<<std::endl;

    QvName refName;
    QvNode* node;

    if (!in->read(refName, FALSE)) {
        QvReadError::post(in, "Premature end of file after " REFERENCE_KEYWORD);
        node = NULL;
    }

    else if ((node = in->findReference(refName)) == NULL) {
        QvReadError::post(in, "Unknown reference \"%s\"", refName.getString());
    }

    return node;
}

QvBool QvNode::readNode(QvInput* in, QvName& className, QvNode*& node) {

    //  std::cerr<<"->QvNode::readNode("<<","<<in<<","<<className.getString()<<","
    //      <<node<<")"<<std::endl;

    QvBool gotChar;
    QvName refName;
    char c;
    QvBool ret = TRUE, flush = FALSE;

    node = NULL;

    if (className == DEFINITION_KEYWORD) {
        if (!in->read(refName, FALSE) || !in->read(className, TRUE)) {
            QvReadError::post(in, "Premature end of file after " DEFINITION_KEYWORD);
            ret = FALSE;
        }

        if (!refName) {
            QvReadError::post(in, "No name given after ", DEFINITION_KEYWORD);
            ret = FALSE;
        }

        if (!className) {
            QvReadError::post(in, "Invalid definition of %s", refName.getString());
            ret = FALSE;
        }
    }

    if (ret) {

        if (!(gotChar = in->read(c)) || c != OPEN_BRACE) {
            if (gotChar) {
                QvReadError::post(in, "Expected '%c'; got '%c'", OPEN_BRACE, c);
            } else {
                QvReadError::post(in, "Expected '%c'; got EOF", OPEN_BRACE);
            }
            ret = FALSE;
        }

        else {
            ret = readNodeInstance(in, className, refName, node);

            if (!ret) {
                flush = TRUE;
            }

            else if (!(gotChar = in->read(c)) || c != CLOSE_BRACE) {
                if (gotChar) {
                    QvReadError::post(in, "Expected '%c'; got '%c'", CLOSE_BRACE, c);
                } else {
                    QvReadError::post(in, "Expected '%c'; got EOF", CLOSE_BRACE);
                }
                ret = FALSE;
            }
        }
    }

    if (!ret && flush) {
        flushInput(in);
    }

    //  std::cerr<<"<-QvNode::readNode("<<","<<in<<","<<className.getString()<<","
    //      <<node<<")"<<std::endl;

    //  std::cerr<<"\tinfo:\tfieldData = "<<node->getFieldData()<<std::endl;
    //  std::cerr<<"\tinfo:\tnumFields = "<<node->getFieldData()->getNumFields()<<std::endl<<std::endl;

    return ret;
}

QvBool QvNode::readNodeInstance(QvInput* in, const QvName& className, const QvName& refName, QvNode*& node) {

    //  std::cerr<<"QvNode::readNodeInstance("<<","<<in<<","<<className.getString()<<","
    //      <<refName.getString()<<","<<node<<")"<<std::endl;

    node = createInstance(in, className);
    if (node == NULL) {
        return FALSE;
    }

    if (!(!refName)) {
        in->addReference(refName, node);
    }

    return node->readInstance(in);
}

QvNode* QvNode::createInstance(QvInput* in, const QvName& className) {
    QvNode* instance;
    QvString unknownString;

    //    std::cerr<<"QvNode::createInstance("<<","<<in<<","<<className.getString()<<")"<<std::endl;

    instance = createInstanceFromName(className);

    if (instance == NULL) {

        if (!in->read(unknownString) || unknownString != "fields") {
            QvReadError::post(in, "Unknown class \"%s\"", className.getString());
            return NULL;
        }

        else if (unknownString == "fields") {
            //	  printf("new QvUnknownNode!\n");
            QvUnknownNode* tmpNode = new QvUnknownNode;
            tmpNode->setClassName(className.getString());
            instance = tmpNode;
            in->putBack(unknownString.getString());
        }
    }

    return instance;
}

QvNode* QvNode::createInstanceFromName(const QvName& className) {

    //  std::cerr<<"QvNode::createInstanceFromName("<<","<<className.getString()<<")"<<std::endl;

#define TRY_CLASS(name, class)                                                                                         \
    else if (className == name) {                                                                                      \
        inst = new class;                                                                                              \
    }

    QvNode* inst = NULL;

    if (0)
        ; // So "else" works in first TRY_CLASS
    TRY_CLASS("AsciiText", QvAsciiText)
    TRY_CLASS("Cone", QvCone)
    TRY_CLASS("Coordinate3", QvCoordinate3)
    TRY_CLASS("Cube", QvCube)
    TRY_CLASS("Cylinder", QvCylinder)
    TRY_CLASS("DirectionalLight", QvDirectionalLight)
    TRY_CLASS("FontStyle", QvFontStyle)
    TRY_CLASS("Group", QvGroup)
    TRY_CLASS("IndexedFaceSet", QvIndexedFaceSet)
    TRY_CLASS("IndexedLineSet", QvIndexedLineSet)
    TRY_CLASS("Info", QvInfo)
    TRY_CLASS("LOD", QvLOD)
    TRY_CLASS("Material", QvMaterial)
    TRY_CLASS("MaterialBinding", QvMaterialBinding)
    TRY_CLASS("MatrixTransform", QvMatrixTransform)
    TRY_CLASS("Normal", QvNormal)
    TRY_CLASS("NormalBinding", QvNormalBinding)
    TRY_CLASS("OrthographicCamera", QvOrthographicCamera)
    TRY_CLASS("PerspectiveCamera", QvPerspectiveCamera)
    TRY_CLASS("PointLight", QvPointLight)
    TRY_CLASS("PointSet", QvPointSet)
    TRY_CLASS("Rotation", QvRotation)
    TRY_CLASS("Scale", QvScale)
    TRY_CLASS("Separator", QvSeparator)
    TRY_CLASS("ShapeHints", QvShapeHints)
    TRY_CLASS("Sphere", QvSphere)
    TRY_CLASS("SpotLight", QvSpotLight)
    TRY_CLASS("Switch", QvSwitch)
    TRY_CLASS("Texture2", QvTexture2)
    TRY_CLASS("Texture2Transform", QvTexture2Transform)
    TRY_CLASS("TextureCoordinate2", QvTextureCoordinate2)
    TRY_CLASS("TextureCoordinateBinding", QvTextureCoordinateBinding)
    TRY_CLASS("Transform", QvTransform)
    TRY_CLASS("TransformSeparator", QvTransformSeparator)
    TRY_CLASS("Translation", QvTranslation)
    TRY_CLASS("WWWAnchor", QvWWWAnchor)
    TRY_CLASS("WWWInline", QvWWWInline)
    TRY_CLASS("DrawStyle", QvDrawStyle)
    return inst;

#undef TRY_CLASS
}

void QvNode::flushInput(QvInput* in) {
    int nestLevel = 1;
    char c;

    while (nestLevel > 0 && in->get(c)) {

        if (c == CLOSE_BRACE) {
            nestLevel--;
        }

        else if (c == OPEN_BRACE) {
            nestLevel++;
        }
    }
}

#undef OPEN_BRACE
#undef CLOSE_BRACE
#undef DEFINITION_KEYWORD
#undef REFERENCE_KEYWORD
#undef NULL_KEYWORD
