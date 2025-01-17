// -*- Mode: c++ -*-
// $Id: tagivgraph.cpp,v 1.6 2000/04/30 07:35:47 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/src/tagivgraph.cpp,v $

/* Subdivide V2.0
   Copyright (C) 2000 Henning Biermann, Denis Zorin, NYU

This file is part of Subdivide.

Subdivide is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

Subdivide is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with Subdivide; see the file COPYING.  If not, write to the Free
Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include <vector>

#include <QvChildList.h>
#include <QvSeparator.h>
#include <QvNode.h>
#include <QvUnknownNode.h>
#include <QvIndexedLineSet.h>

#include <QvDrawStyle.h>
#include <QvMaterial.h>

#include <QvGroup.h>
#include <QvSFFloat.h>
#include <QvSFVec3f.h>
#include <QvSFLong.h>
#include <QvMFLong.h>

#include "tagivgraph.h"
#include "sectorinfo.h"

static void addVert(QvSeparator* n, const char* className, const vector<int>& indexVec);
static void addCreaseEdge(QvSeparator* n, const vector<int>& indexVec);
static void addSector(QvSeparator* n, const vector<TagFlatMesh::FlatSectorType>& secVec);
static void addDrawStyleNode(QvSeparator* n, float linewidth);
static void addMaterialNode(QvSeparator* n, const cvec3f& color);

static void getSector(QvNode* n, vector<TagFlatMesh::FlatSectorType>& secVec);
static void getVert(QvNode* n, const char* className, vector<int>& vertVec);
static void getCreaseEdge(QvNode* n, vector<int>& creaseEdgeVec);

TagIvGraph::TagIvGraph() { ; }

TagIvGraph::~TagIvGraph() { ; }

bool TagIvGraph::toTagFlatMesh(TagFlatMesh* im, bool split) {
  bool b = IvGraph::toFlatMesh(im, split);
  if(b) {
    getSector((QvSeparator*)getRoot(), im->sectorInfoVec);
    getVert((QvSeparator*)getRoot(), "DartVertex", im->dartVertVec);
    getVert((QvSeparator*)getRoot(), "CornerVertex", im->cornerVertVec);
    getVert((QvSeparator*)getRoot(), "CreaseVertex", im->creaseVertVec);
    getCreaseEdge((QvSeparator*)getRoot(), im->creaseEdgeVec);
  }
  return b;
}

void TagIvGraph::fromTagFlatMesh(TagFlatMesh* tagFlatMesh) {
  IvGraph::fromFlatMesh(tagFlatMesh);
  addVert(getRoot(), "CornerVertex", tagFlatMesh->cornerVertVec);
  addVert(getRoot(), "DartVertex", tagFlatMesh->dartVertVec);
  addVert(getRoot(), "CreaseVertex", tagFlatMesh->creaseVertVec);
  addSector(getRoot(), tagFlatMesh->sectorInfoVec);
  addDrawStyleNode(getRoot(), 3.0);
  addMaterialNode(getRoot(), cvec3f(0.0, 1.0, 0.0));
  addCreaseEdge(getRoot(), tagFlatMesh->creaseEdgeVec);
}

//-------------------------------------------------------------

template<class QV_FIELDPTR>
void
findField(QvNode* node, QvName name, QV_FIELDPTR& f) {
  f = 0;
  for (int i = 0; i < node->getFieldData()->getNumFields(); ++i) {
    if (node->getFieldData()->getFieldName(i) == name)
      f = dynamic_cast<QV_FIELDPTR>(node->getFieldData()->getField(node, i));
  }
}

static
TagFlatMesh::FlatSectorType convertToFlatSector(QvUnknownNode* n) {

  QvSFLong* faceIndexField;
  QvSFLong* vertexIndexField;
  QvSFLong* sectorTagField;
  QvSFFloat* thetaField;
  QvSFFloat* flatnessField;
  QvSFFloat* normalTField;
  QvSFVec3f* normalField;

  findField(n, "faceIndex", faceIndexField);
  findField(n, "vertexIndex", vertexIndexField);
  findField(n, "sectorTag", sectorTagField);
  findField(n, "flatness", flatnessField);
  findField(n, "theta", thetaField);
  findField(n, "normal", normalField);
  findField(n, "normalT", normalTField);

  if((faceIndexField == 0) || (vertexIndexField == 0))
    return TagFlatMesh::FlatSectorType(pair<int, int>(0, 0), 0);
  
  int fIndex = faceIndexField->value;
  int vIndex = vertexIndexField->value;

  SectorInfo* si = new SectorInfo();
  if(sectorTagField) 
    si->setSectorTag(SectorInfo::SectorTagType(sectorTagField->value));

  if(flatnessField) 
    si->setModifiedFlatness(flatnessField->value);

  if(thetaField)
    si->setTheta(thetaField->value);

  if(normalTField)
    si->setModifiedNormalT(normalTField->value);

  if(normalField) 
    si->setModifiedNormal(cvec3f(normalField->value[0], 
				 normalField->value[1],
				 normalField->value[2]));
  
  return TagFlatMesh::FlatSectorType(pair<int, int>(fIndex, vIndex), si);
}

static QvSFFloat* makeSFFloat(float f) 
{ QvSFFloat* field = new QvSFFloat(); field->value = f; return field; }

static QvSFVec3f* makeSFVec3f(cvec3f vec) 
{ QvSFVec3f* field = new QvSFVec3f(); 
 field->value[0] = vec(0); 
 field->value[1] = vec(1); 
 field->value[2] = vec(2); 
 return field; 
}

static QvSFLong* makeSFLong(long l) 
{ QvSFLong* field = new QvSFLong(); field->value = l; return field; }


static QvMFLong* makeMFLong(const vector<int>& v) {
  QvMFLong* field = new QvMFLong();
  field->allocValues(v.size());
  for (uint i = 0; i < v.size(); ++i)
    field->values[i] = v[i];
  return field;
}

static
QvUnknownNode* convertFromFlatSector(TagFlatMesh::FlatSectorType fs) {
  SectorInfo* si = fs.second; 
  assert(si);
  QvUnknownNode* n = new QvUnknownNode();
  n->ref();
  n->setClassName("Sector");
  n->getFieldData()->addField(n, "faceIndex", makeSFLong(fs.first.first));
  n->getFieldData()->addField(n, "vertexIndex", makeSFLong(fs.first.second));
  n->getFieldData()->addField(n, "sectorTag", makeSFLong(si->sectorTag()));
  n->getFieldData()->addField(n, "flatness", makeSFFloat(si->modifiedFlatness()));
  n->getFieldData()->addField(n, "theta", makeSFFloat(si->theta()));
  n->getFieldData()->addField(n, "normal", makeSFVec3f(si->modifiedNormal()));
  n->getFieldData()->addField(n, "normalT", makeSFFloat(si->modifiedNormalT()));
  return n;
}

void addSector(QvSeparator* n, const vector<TagFlatMesh::FlatSectorType>& secVec) {
  for(uint i = 0; i < secVec.size(); ++i) {
    QvUnknownNode* u = convertFromFlatSector(secVec[i]);
    n->getChildren()->append(u);
  }
}

void addVert(QvSeparator* n, const char* className, const vector<int>& indexVec) {
  QvUnknownNode* v = new QvUnknownNode();
  v->ref();
  v->setClassName(className);
  v->getFieldData()->addField(v, "vertexIndex", makeMFLong(indexVec));
  n->getChildren()->append(v);
}

void addDrawStyleNode(QvSeparator* n, float lineWidth) {
  QvDrawStyle* node = new QvDrawStyle();
  node->ref();
  node->lineWidth.value = lineWidth;
  n->getChildren()->append(node);
}

void addMaterialNode(QvSeparator* n, const cvec3f& color) {
  QvMaterial* node = new QvMaterial();
  node->ref();
  node->diffuseColor.allocValues(1);
  node->diffuseColor.values[0] = color(0);
  node->diffuseColor.values[1] = color(1);
  node->diffuseColor.values[2] = color(2);
  n->getChildren()->append(node);
}

void addCreaseEdge(QvSeparator* n, const vector<int>& indexVec) {
  QvIndexedLineSet* ls = new QvIndexedLineSet();
  ls->ref();
  ls->coordIndex.allocValues(indexVec.size());
  ls->setName("creaseEdge");
  for (uint i = 0; i < indexVec.size(); ++i)
    ls->coordIndex.values[i] = indexVec[i];
  n->getChildren()->append(ls);
}

void getVert(QvNode* n, const char* className, vector<int>& vertVec) {
  QvGroup* grp;
  QvUnknownNode* un;
  if((un = dynamic_cast<QvUnknownNode*>(n))) {
    if(strcmp(un->className, className) == 0) {
      QvMFLong* indexField = 0;
      findField(un, "vertexIndex", indexField);
      assert(indexField);
      for(int i = 0; i < indexField->num; ++i) 
	vertVec.push_back(indexField->values[i]);
    }
  }
  
  if(vertVec.size() == 0)
    if( (grp = dynamic_cast<QvGroup*>(n) )) 
      for (int i = 0; i < grp->getNumChildren(); i++) 
	getVert(grp->getChild(i), className, vertVec);
}

void getCreaseEdge(QvNode* n, vector<int>& creaseEdgeVec) {
  QvGroup* grp;
  QvIndexedLineSet* un;
  if((un = dynamic_cast<QvIndexedLineSet*>(n))) {
    for(int i = 0; i < un->coordIndex.num; ++i) 
      creaseEdgeVec.push_back(un->coordIndex.values[i]);
  }
  
  if((grp = dynamic_cast<QvGroup*>(n) )) { 
    for (int i = 0; i < grp->getNumChildren(); i++) {
      getCreaseEdge(grp->getChild(i), creaseEdgeVec);
    }
  }  
}

void getSector(QvNode* n, vector<TagFlatMesh::FlatSectorType>& secVec) {
  QvGroup* grp;
  QvUnknownNode* un;

  if((un = dynamic_cast<QvUnknownNode*>(n))) {
    if(strcmp(un->className, "Sector") == 0) {
      TagFlatMesh::FlatSectorType fs = convertToFlatSector(un);
      assert(fs.second);
      SectorInfo::ref(fs.second);
      if(fs.second != 0)
	secVec.push_back(fs);
    }
  }
  if( (grp = dynamic_cast<QvGroup*>(n) )) { 
    int nc=grp->getNumChildren();
    for (int i = 0; i < nc; i++) {
      getSector(grp->getChild(i), secVec);
    }
  }
}

