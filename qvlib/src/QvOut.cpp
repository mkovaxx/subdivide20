#include <QvOut.hpp>

#include <QvNode.hpp>

#include <QvMFColor.hpp>
#include <QvMFFloat.hpp>
#include <QvMFInt32.hpp>
#include <QvMFLong.hpp>
#include <QvMFString.hpp>
#include <QvMFVec2f.hpp>
#include <QvMFVec3f.hpp>
#include <QvSFBool.hpp>
#include <QvSFColor.hpp>
#include <QvSFEnum.hpp>
#include <QvSFFloat.hpp>
#include <QvSFImage.hpp>
#include <QvSFLong.hpp>
#include <QvSFMatrix.hpp>
#include <QvSFRotation.hpp>
#include <QvSFString.hpp>
#include <QvSFUShort.hpp>
#include <QvSFVec2f.hpp>
#include <QvSFVec3f.hpp>

#include <QvCoordinate3.hpp>
#include <QvDrawStyle.hpp>
#include <QvIndexedFaceSet.hpp>
#include <QvIndexedLineSet.hpp>
#include <QvMaterial.hpp>
#include <QvMatrixTransform.hpp>
#include <QvPerspectiveCamera.hpp>
#include <QvSeparator.hpp>
#include <QvUnknownNode.hpp>
#include <fstream>

static void writeNode(QvNode* node, int indent, std::ostream& outfile);

QvBool QvOut::_error;

QvOut::QvOut(char* f) {
    strcpy(filename, f);
    std::ofstream outStream(filename);
    if (!outStream) {
        std::cerr << "error: unable to open output file: " << filename << std::endl;
        QvOut::_error = true;
    } else {
        outStream << "#VRML V1.0 ascii" << std::endl;
    }
}

void QvOut::write(QvNode* node) {
    std::ofstream outStream(filename, std::ios::out | std::ios::app);
    writeNode(node, 0, outStream);
}

//---------------------------------------------------------------------
// write fields

char* getFieldDefName(QvField* field) {

    char* name;

    if (dynamic_cast<QvMFColor*>(field)) {
        name = "MFColor";
    } else if (dynamic_cast<QvMFVec3f*>(field)) {
        name = "MFVec3f";
    } else if (dynamic_cast<QvMFVec2f*>(field)) {
        name = "MFVec2f";
    } else if (dynamic_cast<QvMFString*>(field)) {
        name = "MFString";
    } else if (dynamic_cast<QvMFLong*>(field)) {
        name = "MFLong";
    } else if (dynamic_cast<QvMFInt32*>(field)) {
        name = "MFInt32";
    } else if (dynamic_cast<QvMFFloat*>(field)) {
        name = "MFFloat";
    }

    else if (dynamic_cast<QvSFMatrix*>(field)) {
        name = "SFMatrix";
    } else if (dynamic_cast<QvSFLong*>(field)) {
        name = "SFLong";
    } else if (dynamic_cast<QvSFImage*>(field)) {
        name = "SFImage";
    } else if (dynamic_cast<QvSFFloat*>(field)) {
        name = "SFFloat";
    } else if (dynamic_cast<QvSFEnum*>(field)) {
        name = "SFEnum";
    } else if (dynamic_cast<QvSFColor*>(field)) {
        name = "SFColor";
    } else if (dynamic_cast<QvSFBool*>(field)) {
        name = "SFBool";
    } else if (dynamic_cast<QvSFBitMask*>(field)) {
        name = "SFBitMask";
    } else if (dynamic_cast<QvSFRotation*>(field)) {
        name = "SFRotation";
    } else if (dynamic_cast<QvSFString*>(field)) {
        name = "SFString";
    } else if (dynamic_cast<QvSFVec2f*>(field)) {
        name = "SFVec2f";
    } else if (dynamic_cast<QvSFVec3f*>(field)) {
        name = "SFVec3f";
    }

    else {
        std::cerr << "QvOut: unknown field" << std::endl;
        name = "";
        QvOut::_error = true;
    }

    return name;
}

static void printIndent(int indent, std::ostream& outfile) {
    int i;
    for (i = 0; i < indent; i++) {
        outfile << "\t";
    }
}

static void writeMFFloat(QvMFFloat* field, int indent, std::ostream& outfile) {
    outfile << " [ ";
    for (int i = 0; i < field->num; ++i) {
        if (i % 4 == 0) {
            outfile << std::endl;
            printIndent(indent + 1, outfile);
        }
        outfile << field->values[i] << ", ";
    }
    outfile << std::endl;
    printIndent(indent, outfile);
    outfile << "]" << std::endl;
}

static void writeMFInt32(QvMFInt32* field, int indent, std::ostream& outfile) {
    outfile << " [ ";
    for (int i = 0; i < field->num; ++i) {
        if (i % 3 == 0) {
            outfile << std::endl;
            printIndent(indent + 1, outfile);
        }
        outfile << field->values[i] << ", ";
    }
    outfile << std::endl;
    printIndent(indent, outfile);
    outfile << "]" << std::endl;
}

static void writeMFVec3f(QvMFVec3f* field, int indent, std::ostream& outfile) {
    outfile << " [ " << std::endl;
    printIndent(indent + 1, outfile);
    for (int i = 0; i < field->num; ++i) {
        outfile << field->values[i * 3 + 0] << "  " << field->values[i * 3 + 1] << "  " << field->values[i * 3 + 2]
                << "," << std::endl;
        printIndent(indent + 1, outfile);
    }
    outfile << std::endl;
    printIndent(indent, outfile);
    outfile << "]" << std::endl;
}

static void writeMFString(QvMFString* field, int indent, std::ostream& outfile) {
    outfile << " [ " << std::endl;
    for (int i = 0; i < field->num; ++i) {
        printIndent(indent + 1, outfile);
        outfile << "\"" << field->values[i].getString() << "\", " << std::endl;
    }
    printIndent(indent, outfile);
    outfile << "]" << std::endl;
}

static void writeMFLong(QvMFLong* field, int indent, std::ostream& outfile) {
    outfile << " [ ";
    for (int i = 0; i < field->num; ++i) {
        if (i % 4 == 0) {
            outfile << std::endl;
            printIndent(indent + 1, outfile);
        }
        outfile << field->values[i] << ", ";
    }
    outfile << std::endl;
    printIndent(indent, outfile);
    outfile << "]" << std::endl;
}

static void writeSFFloat(QvSFFloat* field, int, std::ostream& outfile) { outfile << " " << field->value << std::endl; }

static void writeSFLong(QvSFLong* field, int, std::ostream& outfile) { outfile << " " << field->value << std::endl; }

static void writeSFVec3f(QvSFVec3f* field, int, std::ostream& outfile) {
    outfile << " " << field->value[0] << " " << field->value[1] << " " << field->value[2] << std::endl;
}

static void writeSFMatrix(QvSFMatrix* field, int indent, std::ostream& outfile) {
    outfile << std::endl;
    for (int i = 0; i < 4; ++i) {
        printIndent(indent + 1, outfile);
        outfile << " " << field->value[i][0] << "  " << field->value[i][1] << "  " << field->value[i][2] << "  "
                << field->value[i][3] << std::endl;
    }
}

static void writeSFRotation(QvSFRotation* field, int, std::ostream& outfile) {
    outfile << " " << field->axis[0] << " " << field->axis[1] << " " << field->axis[2] << "     " << field->angle
            << std::endl;
}

static void writeSFEnum(QvSFEnum* field, int, std::ostream& outfile) {
    QvName name;
    QvBool found = field->findEnumName(field->value, name);
    if (found == TRUE) {
        outfile << " " << name.getString() << std::endl;
    } else {
        outfile << " " << field->value << std::endl;
    }
}

static void writeMFColor(QvMFColor* field, int, std::ostream& outfile) {
    outfile << " ";
    for (int i = 0; i < 3 * field->num; ++i) {
        outfile << field->values[i] << " ";
    }
    outfile << std::endl;
}

static void writeSFUShort(QvSFUShort* field, int, std::ostream& outfile) {
    outfile << " " << field->value << std::endl;
}

static void writeField(QvField* field, int indent, std::ostream& outfile) {
    QvMFFloat* fieldMFFloat = 0;
    QvMFInt32* fieldMFInt32 = 0;
    QvMFVec3f* fieldMFVec3f = 0;
    QvMFString* fieldMFString = 0;
    QvMFLong* fieldMFLong = 0;
    QvMFColor* fieldMFColor = 0;

    QvSFLong* fieldSFLong = 0;
    QvSFEnum* fieldSFEnum = 0;
    QvSFUShort* fieldSFUShort = 0;
    QvSFFloat* fieldSFFloat = 0;
    QvSFVec3f* fieldSFVec3f = 0;
    QvSFMatrix* fieldSFMatrix = 0;
    QvSFRotation* fieldSFRotation = 0;

    if ((fieldMFFloat = dynamic_cast<QvMFFloat*>(field))) {
        writeMFFloat(fieldMFFloat, indent, outfile);
    }

    else if ((fieldMFInt32 = dynamic_cast<QvMFInt32*>(field))) {
        writeMFInt32(fieldMFInt32, indent, outfile);
    }

    else if ((fieldMFVec3f = dynamic_cast<QvMFVec3f*>(field))) {
        writeMFVec3f(fieldMFVec3f, indent, outfile);
    }

    else if ((fieldMFString = dynamic_cast<QvMFString*>(field))) {
        writeMFString(fieldMFString, indent, outfile);
    }

    else if ((fieldMFLong = dynamic_cast<QvMFLong*>(field))) {
        writeMFLong(fieldMFLong, indent, outfile);
    }

    else if ((fieldSFFloat = dynamic_cast<QvSFFloat*>(field))) {
        writeSFFloat(fieldSFFloat, indent, outfile);
    }

    else if ((fieldSFMatrix = dynamic_cast<QvSFMatrix*>(field))) {
        writeSFMatrix(fieldSFMatrix, indent, outfile);
    }

    else if ((fieldSFRotation = dynamic_cast<QvSFRotation*>(field))) {
        writeSFRotation(fieldSFRotation, indent, outfile);
    }

    else if ((fieldSFVec3f = dynamic_cast<QvSFVec3f*>(field))) {
        writeSFVec3f(fieldSFVec3f, indent, outfile);
    }

    else if ((fieldMFColor = dynamic_cast<QvMFColor*>(field))) {
        writeMFColor(fieldMFColor, indent, outfile);
    }

    else if ((fieldSFEnum = dynamic_cast<QvSFEnum*>(field))) {
        writeSFEnum(fieldSFEnum, indent, outfile);
    }

    else if ((fieldSFUShort = dynamic_cast<QvSFUShort*>(field))) {
        writeSFUShort(fieldSFUShort, indent, outfile);
    }

    else if ((fieldSFLong = dynamic_cast<QvSFLong*>(field))) {
        writeSFLong(fieldSFLong, indent, outfile);
    }

    else {
        std::cerr << "QvOut: unknown field!" << std::endl;
        QvOut::_error = true;
    }
}

//---------------------------------------------------------------------
// write nodes

static void writeFieldDef(QvNode* node, int indent, std::ostream& outfile) {
    printIndent(indent, outfile);
    outfile << "fields [" << std::endl;
    for (int i = 0; i < node->getFieldData()->getNumFields(); ++i) {
        printIndent(indent + 1, outfile);
        outfile << getFieldDefName(node->getFieldData()->getField(node, i)) << " ";
        outfile << node->getFieldData()->getFieldName(i).getString() << "," << std::endl;
    }
    printIndent(indent, outfile);
    outfile << "]" << std::endl;
}

static void writeNodeData(QvNode* node, int indent, std::ostream& outfile) {
    for (int i = 0; i < node->getFieldData()->getNumFields(); ++i) {
        printIndent(indent, outfile);
        outfile << node->getFieldData()->getFieldName(i).getString();
        writeField(node->getFieldData()->getField(node, i), indent, outfile);
    }

    QvGroup* group;
    if ((group = dynamic_cast<QvGroup*>(node))) {
        outfile << std::endl;
        for (int i = 0; i < group->getNumChildren(); ++i) {
            printIndent(indent, outfile);
            writeNode(group->getChild(i), indent, outfile);
        }
    }
}

static void writeDrawStyle(QvDrawStyle* node, int indent, std::ostream& outfile) {
    outfile << "DrawStyle"
            << " {" << std::endl;
    writeNodeData(node, indent + 1, outfile);
    printIndent(indent, outfile);
    outfile << "}" << std::endl;
}

static void writeMaterial(QvMaterial* node, int indent, std::ostream& outfile) {
    outfile << "Material"
            << " {" << std::endl;
    writeNodeData(node, indent + 1, outfile);
    printIndent(indent, outfile);
    outfile << "}" << std::endl;
}

static void writeSeparator(QvSeparator* node, int indent, std::ostream& outfile) {
    outfile << "Separator"
            << " {" << std::endl;
    writeNodeData(node, indent + 1, outfile);
    printIndent(indent, outfile);
    outfile << "}" << std::endl;
}

static void writePerspectiveCamera(QvPerspectiveCamera* node, int indent, std::ostream& outfile) {
    outfile << "PerspectiveCamera"
            << " {" << std::endl;
    writeNodeData(node, indent + 1, outfile);
    printIndent(indent, outfile);
    outfile << "}" << std::endl;
}

static void writeCoordinate3(QvCoordinate3* node, int indent, std::ostream& outfile) {
    outfile << "Coordinate3"
            << " {" << std::endl;
    writeNodeData(node, indent + 1, outfile);
    printIndent(indent, outfile);
    outfile << "}" << std::endl;
}

static void writeIndexedFaceSet(QvIndexedFaceSet* node, int indent, std::ostream& outfile) {
    outfile << "IndexedFaceSet"
            << " {" << std::endl;
    writeNodeData(node, indent + 1, outfile);
    printIndent(indent, outfile);
    outfile << "}" << std::endl;
}

static void writeIndexedLineSet(QvIndexedLineSet* node, int indent, std::ostream& outfile) {
    outfile << "IndexedLineSet"
            << " {" << std::endl;
    writeNodeData(node, indent + 1, outfile);
    printIndent(indent, outfile);
    outfile << "}" << std::endl;
}

static void writeMatrixTransformNode(QvMatrixTransform* node, int indent, std::ostream& outfile) {
    outfile << "MatrixTransform"
            << " {" << std::endl;
    writeNodeData(node, indent + 1, outfile);
    printIndent(indent, outfile);
    outfile << "}" << std::endl;
}

static void writeUnknownNode(QvUnknownNode* node, int indent, std::ostream& outfile) {
    outfile << node->className << " {" << std::endl;
    writeFieldDef(node, indent + 1, outfile);
    writeNodeData(node, indent + 1, outfile);
    printIndent(indent, outfile);
    outfile << "}" << std::endl;
}

//---------------------------------------------------------------------

void writeNode(QvNode* node, int indent, std::ostream& outfile) {

    if (strlen(node->getName().getString()) != 0) {
        outfile << "DEF " << node->getName().getString() << " ";
    }

    QvUnknownNode* nodeUnknown;
    QvCoordinate3* nodeCoordinate3;
    QvDrawStyle* nodeDrawStyle;
    QvMaterial* nodeMaterial;
    QvIndexedLineSet* nodeIndexedLineSet;
    QvIndexedFaceSet* nodeIndexedFaceSet;
    QvPerspectiveCamera* nodePerspectiveCamera;
    QvSeparator* nodeSeparator;
    QvMatrixTransform* nodeMatrixTransform;

    if ((nodeSeparator = dynamic_cast<QvSeparator*>(node))) {
        writeSeparator(nodeSeparator, indent, outfile);
    } else if ((nodeCoordinate3 = dynamic_cast<QvCoordinate3*>(node))) {
        writeCoordinate3(nodeCoordinate3, indent, outfile);
    } else if ((nodeDrawStyle = dynamic_cast<QvDrawStyle*>(node))) {
        writeDrawStyle(nodeDrawStyle, indent, outfile);
    } else if ((nodeMaterial = dynamic_cast<QvMaterial*>(node))) {
        writeMaterial(nodeMaterial, indent, outfile);
    } else if ((nodeIndexedLineSet = dynamic_cast<QvIndexedLineSet*>(node))) {
        writeIndexedLineSet(nodeIndexedLineSet, indent, outfile);
    } else if ((nodeIndexedFaceSet = dynamic_cast<QvIndexedFaceSet*>(node))) {
        writeIndexedFaceSet(nodeIndexedFaceSet, indent, outfile);
    } else if ((nodePerspectiveCamera = dynamic_cast<QvPerspectiveCamera*>(node))) {
        writePerspectiveCamera(nodePerspectiveCamera, indent, outfile);
    } else if ((nodeUnknown = dynamic_cast<QvUnknownNode*>(node))) {
        writeUnknownNode(nodeUnknown, indent, outfile);
    } else if ((nodeMatrixTransform = dynamic_cast<QvMatrixTransform*>(node))) {
        writeMatrixTransformNode(nodeMatrixTransform, indent, outfile);
    } else {
        std::cerr << "QvOut: unknown node" << std::endl;
        QvOut::_error = true;
    }

    outfile << std::endl;
}
