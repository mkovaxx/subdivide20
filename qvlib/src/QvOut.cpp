#include <QvOut.h>

#include<QvNode.h>

#include<QvSFFloat.h>
#include<QvSFVec3f.h>
#include<QvSFRotation.h>
#include<QvSFMatrix.h>
#include<QvMFLong.h>
#include<QvMFVec3f.h>
#include<QvMFInt32.h>
#include<QvMFFloat.h>
#include<QvMFString.h>
#include<QvSFEnum.h>
#include<QvSFUShort.h>
#include<QvMFColor.h>
#include<QvSFString.h>
#include<QvMFVec2f.h>
#include<QvSFVec2f.h>
#include<QvSFLong.h>
#include<QvSFImage.h>
#include<QvSFColor.h>
#include<QvSFBool.h>
#include<QvSFLong.h>

#include<QvUnknownNode.h>
#include<QvCoordinate3.h>
#include<QvPerspectiveCamera.h>
#include<QvIndexedFaceSet.h>
#include<QvIndexedLineSet.h>
#include<QvSeparator.h>
#include<QvMaterial.h>
#include<QvDrawStyle.h>
#include<QvMatrixTransform.h>
#include<fstream.h>

static void writeNode(QvNode* node, int indent, ostream& outfile);

QvBool QvOut::_error;

QvOut::QvOut(char* f){
  strcpy(filename, f);
  ofstream outStream(filename);
  if(!outStream) {
    cerr<<"error: unable to open output file: "<<filename<<endl;
    QvOut::_error = true;
  } else {
    outStream<<"#VRML V1.0 ascii"<<endl;
  }
}

void QvOut::write(QvNode* node) {
	ofstream outStream(filename, ios::out | ios::app);
  writeNode(node, 0, outStream);
}

//---------------------------------------------------------------------
// write fields


char* getFieldDefName(QvField* field) {

  char* name;

  if (dynamic_cast<QvMFColor*>(field))         name = "MFColor";
  else if (dynamic_cast<QvMFVec3f*>(field))    name = "MFVec3f";
  else if (dynamic_cast<QvMFVec2f*>(field))    name = "MFVec2f";
  else if (dynamic_cast<QvMFString*>(field))   name = "MFString";
  else if (dynamic_cast<QvMFLong*>(field))     name = "MFLong";
  else if (dynamic_cast<QvMFInt32*>(field))    name = "MFInt32";
  else if (dynamic_cast<QvMFFloat*>(field))    name = "MFFloat";

  else if (dynamic_cast<QvSFMatrix*>(field))   name = "SFMatrix";
  else if (dynamic_cast<QvSFLong*>(field))     name = "SFLong";
  else if (dynamic_cast<QvSFImage*>(field))    name = "SFImage";
  else if (dynamic_cast<QvSFFloat*>(field))    name = "SFFloat";
  else if (dynamic_cast<QvSFEnum*>(field))     name = "SFEnum";
  else if (dynamic_cast<QvSFColor*>(field))    name = "SFColor";
  else if (dynamic_cast<QvSFBool*>(field))     name = "SFBool";
  else if (dynamic_cast<QvSFBitMask*>(field))  name = "SFBitMask";
  else if (dynamic_cast<QvSFRotation*>(field)) name = "SFRotation";
  else if (dynamic_cast<QvSFString*>(field))   name = "SFString";
  else if (dynamic_cast<QvSFVec2f*>(field))    name = "SFVec2f";
  else if (dynamic_cast<QvSFVec3f*>(field))    name = "SFVec3f";

  else {
    cerr<<"QvOut: unknown field"<<endl;
    name = "";
    QvOut::_error = true;
  }

  return name;
}

static void 
printIndent(int indent, ostream& outfile) {
  int i;
  for (i = 0; i < indent; i++) 
    outfile<<"\t";
}

static void 
writeMFFloat(QvMFFloat* field, int indent, ostream& outfile) {
  outfile<<" [ ";
  for (int i = 0; i < field->num; ++i) {
    if (i % 4 == 0) {
      outfile<<endl;
      printIndent(indent+1, outfile);
    }
    outfile<<field->values[i]<<", ";
  }
  outfile<<endl;
  printIndent(indent, outfile); outfile<<"]"<<endl;
}

static void 
writeMFInt32(QvMFInt32* field, int indent, ostream& outfile) {
  outfile<<" [ ";
  for (int i = 0; i < field->num; ++i) {
    if (i % 3 == 0) {
      outfile<<endl;
      printIndent(indent+1, outfile);
    }
    outfile<<field->values[i]<<", ";
  }
  outfile<<endl;
  printIndent(indent, outfile); outfile<<"]"<<endl;
}

static void 
writeMFVec3f(QvMFVec3f* field, int indent, ostream& outfile) {
  outfile<<" [ "<<endl;
  printIndent(indent+1, outfile);
  for (int i = 0; i < field->num; ++i) {
    outfile<<field->values[i*3+0]<<"  "
	   <<field->values[i*3+1]<<"  "
	   <<field->values[i*3+2]<<","<<endl;
    printIndent(indent+1, outfile);
  }
  outfile<<endl;
  printIndent(indent, outfile); outfile<<"]"<<endl;
}

static void 
writeMFString(QvMFString* field, int indent, ostream& outfile) {
  outfile<<" [ "<<endl;
  for (int i = 0; i < field->num; ++i) {
    printIndent(indent+1, outfile); 
    outfile<<"\""<<field->values[i].getString()<<"\", "<<endl;
  }
  printIndent(indent, outfile); outfile<<"]"<<endl;
}

static void 
writeMFLong(QvMFLong* field, int indent, ostream& outfile) {
  outfile<<" [ ";
  for (int i = 0; i < field->num; ++i) {
    if (i % 4 == 0) {
      outfile<<endl;
      printIndent(indent+1, outfile);
    }
    outfile<<field->values[i]<<", ";
  }
  outfile<<endl;
  printIndent(indent, outfile); outfile<<"]"<<endl;
}

static void 
writeSFFloat(QvSFFloat* field, int , ostream& outfile) {
  outfile<<" "<<field->value<<endl;
}

static void 
writeSFLong(QvSFLong* field, int , ostream& outfile) {
  outfile<<" "<<field->value<<endl;
}

static void 
writeSFVec3f(QvSFVec3f* field, int , ostream& outfile) {
  outfile<<" "<<field->value[0]
	 <<" "<<field->value[1]
	 <<" "<<field->value[2]
	 <<endl;
}

static void 
writeSFMatrix(QvSFMatrix* field, int indent, ostream& outfile) {
  outfile<<endl;
  for (int i = 0; i < 4; ++i) {
    printIndent(indent+1, outfile); 
    outfile<<" "
	   <<field->value[i][0]<<"  "<<field->value[i][1]<<"  "
	   <<field->value[i][2]<<"  "<<field->value[i][3]<<endl;
  }
}

static void 
writeSFRotation(QvSFRotation* field, int , ostream& outfile) {
  outfile<<" "<<field->axis[0]
	 <<" "<<field->axis[1]
	 <<" "<<field->axis[2]
	 <<"     "<<field->angle<<endl;
}

static void
writeSFEnum(QvSFEnum* field, int , ostream& outfile) {
  QvName name;
  QvBool found = field->findEnumName(field->value, name);
  if (found == TRUE) 
    outfile<<" "<<name.getString()<<endl;
  else 
    outfile<<" "<<field->value<<endl;
}

static void
writeMFColor(QvMFColor* field, int , ostream& outfile) {
  outfile<<" ";
  for (int i = 0; i < 3*field->num; ++i) 
    outfile<<field->values[i]<<" ";
  outfile<<endl;
}

static void
writeSFUShort(QvSFUShort* field, int , ostream& outfile) {
  outfile<<" "<<field->value<<endl;
}

static
void writeField(QvField* field, int indent, ostream& outfile) {
  QvMFFloat  *fieldMFFloat = 0;
  QvMFInt32  *fieldMFInt32 = 0;
  QvMFVec3f  *fieldMFVec3f = 0;
  QvMFString *fieldMFString = 0;
  QvMFLong   *fieldMFLong = 0;
  QvMFColor  *fieldMFColor = 0;

  QvSFLong   *fieldSFLong = 0;
  QvSFEnum   *fieldSFEnum = 0;
  QvSFUShort *fieldSFUShort = 0;
  QvSFFloat  *fieldSFFloat = 0;
  QvSFVec3f  *fieldSFVec3f = 0;
  QvSFMatrix *fieldSFMatrix = 0;
  QvSFRotation  *fieldSFRotation = 0;

  if ((fieldMFFloat = dynamic_cast<QvMFFloat*>(field)))
    writeMFFloat(fieldMFFloat, indent, outfile);

  else if ((fieldMFInt32 = dynamic_cast<QvMFInt32*>(field)))
    writeMFInt32(fieldMFInt32, indent, outfile);

  else if ((fieldMFVec3f = dynamic_cast<QvMFVec3f*>(field)))
    writeMFVec3f(fieldMFVec3f, indent, outfile);

  else if ((fieldMFString = dynamic_cast<QvMFString*>(field)))
    writeMFString(fieldMFString, indent, outfile);

  else if ((fieldMFLong = dynamic_cast<QvMFLong*>(field)))
    writeMFLong(fieldMFLong, indent, outfile);

  else if ((fieldSFFloat = dynamic_cast<QvSFFloat*>(field)))
    writeSFFloat(fieldSFFloat, indent, outfile);

  else if ((fieldSFMatrix = dynamic_cast<QvSFMatrix*>(field))) 
    writeSFMatrix(fieldSFMatrix, indent, outfile);

  else if ((fieldSFRotation = dynamic_cast<QvSFRotation*>(field)))
    writeSFRotation(fieldSFRotation, indent, outfile);

  else if ((fieldSFVec3f = dynamic_cast<QvSFVec3f*>(field)))
    writeSFVec3f(fieldSFVec3f, indent, outfile);

  else if ((fieldMFColor = dynamic_cast<QvMFColor*>(field)))
    writeMFColor(fieldMFColor, indent, outfile);

  else if ((fieldSFEnum = dynamic_cast<QvSFEnum*>(field)))
    writeSFEnum(fieldSFEnum, indent, outfile);

  else if ((fieldSFUShort = dynamic_cast<QvSFUShort*>(field)))
    writeSFUShort(fieldSFUShort, indent, outfile);

  else if ((fieldSFLong = dynamic_cast<QvSFLong*>(field)))
    writeSFLong(fieldSFLong, indent, outfile);

  else {
    cerr<<"QvOut: unknown field!"<<endl;
    QvOut::_error = true;
  }
}


//---------------------------------------------------------------------
// write nodes

static void
writeFieldDef(QvNode* node, int indent, ostream& outfile) {
  printIndent(indent, outfile); outfile<<"fields ["<<endl;
  for (int i = 0; i < node->getFieldData()->getNumFields(); ++i) {
    printIndent(indent+1, outfile);
    outfile<<getFieldDefName(node->getFieldData()->getField(node,i))<<" ";
    outfile<<node->getFieldData()->getFieldName(i).getString()<<","<<endl;
    
  }
  printIndent(indent, outfile); outfile<<"]"<<endl;
}

static void
writeNodeData(QvNode* node, int indent, ostream& outfile) {
  for (int i = 0; i < node->getFieldData()->getNumFields(); ++i) {    
    printIndent(indent, outfile); 
    outfile<<node->getFieldData()->getFieldName(i).getString(); 
    writeField(node->getFieldData()->getField(node,i), indent, outfile);    
  }
  
  QvGroup* group;
  if ((group = dynamic_cast<QvGroup*>(node))) {
    outfile<<endl;
    for (int i = 0; i < group->getNumChildren(); ++i) {
      printIndent(indent, outfile);
      writeNode(group->getChild(i), indent, outfile);
    }
  }
}

static void 
writeDrawStyle(QvDrawStyle* node, int indent, ostream& outfile) {
  outfile<<"DrawStyle"<<" {"<<endl;
  writeNodeData(node, indent+1, outfile);
  printIndent(indent, outfile); outfile<<"}"<<endl;
}

static void
writeMaterial(QvMaterial* node, int indent, ostream& outfile) {
  outfile<<"Material"<<" {"<<endl;
  writeNodeData(node, indent+1, outfile);
  printIndent(indent, outfile); outfile<<"}"<<endl;
}

static void
writeSeparator(QvSeparator* node, int indent, ostream& outfile) {
  outfile<<"Separator"<<" {"<<endl;
  writeNodeData(node, indent+1, outfile);
  printIndent(indent, outfile); outfile<<"}"<<endl;
}

static void 
writePerspectiveCamera(QvPerspectiveCamera* node, int indent, ostream& outfile) {
  outfile<<"PerspectiveCamera"<<" {"<<endl;
  writeNodeData(node, indent+1, outfile);
  printIndent(indent, outfile); outfile<<"}"<<endl;
}

static void
writeCoordinate3(QvCoordinate3* node, int indent, ostream& outfile) {
  outfile<<"Coordinate3"<<" {"<<endl;
  writeNodeData(node, indent+1, outfile);
  printIndent(indent, outfile); outfile<<"}"<<endl;
}

static void
writeIndexedFaceSet(QvIndexedFaceSet* node, int indent, ostream& outfile) {
  outfile<<"IndexedFaceSet"<<" {"<<endl;
  writeNodeData(node, indent+1, outfile);
  printIndent(indent, outfile); outfile<<"}"<<endl;
}

static void
writeIndexedLineSet(QvIndexedLineSet* node, int indent, ostream& outfile) {
  outfile<<"IndexedLineSet"<<" {"<<endl;
  writeNodeData(node, indent+1, outfile);
  printIndent(indent, outfile); outfile<<"}"<<endl;
}

static void
writeMatrixTransformNode(QvMatrixTransform* node, int indent, ostream& outfile) {
  outfile<<"MatrixTransform"<<" {"<<endl;
  writeNodeData(node, indent+1, outfile);
  printIndent(indent, outfile);  outfile<<"}"<<endl;
}

static void
writeUnknownNode(QvUnknownNode* node, int indent, ostream& outfile) {
  outfile<<node->className<<" {"<<endl;
  writeFieldDef(node, indent+1, outfile);
  writeNodeData(node, indent+1, outfile);
  printIndent(indent, outfile); outfile<<"}"<<endl;
}

//---------------------------------------------------------------------

void writeNode(QvNode* node, int indent, ostream& outfile) {

  if (strlen(node->getName().getString()) != 0)
    outfile<<"DEF "<<node->getName().getString()<<" ";  
  
  QvUnknownNode *nodeUnknown;
  QvCoordinate3 *nodeCoordinate3;
  QvDrawStyle   *nodeDrawStyle;
  QvMaterial    *nodeMaterial;
  QvIndexedLineSet *nodeIndexedLineSet;
  QvIndexedFaceSet *nodeIndexedFaceSet;
  QvPerspectiveCamera *nodePerspectiveCamera;
  QvSeparator *nodeSeparator;
  QvMatrixTransform *nodeMatrixTransform;

  if ((nodeSeparator = dynamic_cast<QvSeparator*>(node)))
    writeSeparator(nodeSeparator, indent, outfile);
  else if ((nodeCoordinate3 = dynamic_cast<QvCoordinate3*>(node)))
    writeCoordinate3(nodeCoordinate3, indent, outfile);
  else if ((nodeDrawStyle = dynamic_cast<QvDrawStyle*>(node)))
    writeDrawStyle(nodeDrawStyle, indent, outfile);
  else if ((nodeMaterial = dynamic_cast<QvMaterial*>(node)))
    writeMaterial(nodeMaterial, indent, outfile);
  else if ((nodeIndexedLineSet = dynamic_cast<QvIndexedLineSet*>(node)))
    writeIndexedLineSet(nodeIndexedLineSet, indent, outfile);
  else if ((nodeIndexedFaceSet = dynamic_cast<QvIndexedFaceSet*>(node)))
    writeIndexedFaceSet(nodeIndexedFaceSet, indent, outfile);
  else if ((nodePerspectiveCamera = dynamic_cast<QvPerspectiveCamera*>(node)))
    writePerspectiveCamera(nodePerspectiveCamera, indent, outfile);
  else if ((nodeUnknown = dynamic_cast<QvUnknownNode*>(node)))
    writeUnknownNode(nodeUnknown, indent, outfile);
  else if ((nodeMatrixTransform = dynamic_cast<QvMatrixTransform*>(node)))
    writeMatrixTransformNode(nodeMatrixTransform, indent, outfile);
  else {
    cerr<<"QvOut: unknown node"<<endl;
    QvOut::_error = true;
  }

  outfile<<endl;
}

