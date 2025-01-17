// -*- Mode: c++ -*-
// $Id: subviewer.cpp,v 1.13 2001/02/07 16:15:04 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/examples/subviewer.cpp,v $

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

#include "compat.h"

#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

#include "general.h"         // maxdepth

#include "pickviewer.h"      // viewer class

#include "tagivgraph.h"      // inventor graph
#include "tagflatmesh.h"     // flatmesh data structure

#include "pickabletri.h"     // triangle mesh + rendering + picking
#include "pickablequad.h"    // quad mesh + rendering + picking

#include "trimanipulator.h"  // manipulation of triangles
#include "quadmanipulator.h" // manipulation of quads

#include "sectorinfo.h"      // sector information used for call backs

int depth = 3;

void init(int argc, char** argv, PickableTri &triObj, PickableQuad& quadObj) {
  TagIvGraph ivGraph;
  if(argc < 2) {
    cerr<<"usage: "<<argv[0]<<" infile"<<endl;
    exit(1);
  } else {
    bool res = ivGraph.read(argv[1]);    
    if(!res) {
      cerr<<"could not read "<<argv[1]<<endl;
      exit(1);
    }

    if(argc > 2)
      depth = min(max(atoi(argv[2]), 0), GEN_MAX_DEPTH);
  }
  
  TagFlatMesh tagFlatMesh;

  ivGraph.toTagFlatMesh(&tagFlatMesh, true);
  TriMesh triMesh(tagFlatMesh);
  triObj.setMesh(triMesh);
  
  ivGraph.toTagFlatMesh(&tagFlatMesh, false);
  QuadMesh quadMesh(tagFlatMesh);
  quadObj.setMesh(quadMesh);
}

//----------------------------------------------------------------------------

// we need global variables to remember what we have selected
PickedStuff pickedTriStuff;
PickedStuff pickedQuadStuff;

// call back to toggle ui state
void toggleViewerStateCB(void* o) 
{ ((PickViewer*)o)->toggleState(); }

// flatness higher cb
void triFlatUpCB(void* triObject) {
  TriManipulator tm(pickedTriStuff.face());
  if(pickedTriStuff.type() == PickedStuff::PICK_SECTOR){
    VnoType vno = tm.headVno(pickedTriStuff.eno());
    if(tm.getVertexTag(vno) == TriManipulator::CORNER_VERTEX) {
      // get the sector information
      SectorInfo* si = tm.getSectorInfo(vno);
      assert(si);
      float f = si->modifiedFlatness();
      f+=0.1;
      f = min(f, 1.0f);
      cerr<<"set flatness: "<<f<<endl;
      f = min(f, 1.0f);
      si->setModifiedFlatness(f);
      tm.recomputeNeighbors();
      ((PickableTri*)triObject)->rerender();
    }
  }
}

// flatness higher cb
void quadFlatUpCB(void* quadObject) {
  QuadManipulator tm(pickedQuadStuff.face());
  if(pickedQuadStuff.type() == PickedStuff::PICK_SECTOR){
    VnoType vno = tm.headVno(pickedQuadStuff.eno());
    if(tm.getVertexTag(vno) == QuadManipulator::CORNER_VERTEX) {
      // get the sector information
      SectorInfo* si = tm.getSectorInfo(vno);
      float f = si->modifiedFlatness();
      f+=0.1;
      f = min(f, 1.0f);
      cerr<<"set flatness: "<<f<<endl;
      f = min(f, 1.0f);
      si->setModifiedFlatness(f);
      tm.recomputeNeighbors();
      ((PickableQuad*)quadObject)->rerender();
    }
  }
}

// flatness higher cb
void triFlatDownCB(void* triObject) {
  TriManipulator tm(pickedTriStuff.face());
  if(pickedTriStuff.type() == PickedStuff::PICK_SECTOR){
    VnoType vno = tm.headVno(pickedTriStuff.eno());
    if(tm.getVertexTag(vno) == TriManipulator::CORNER_VERTEX) {
      // get the sector information
      SectorInfo* si = tm.getSectorInfo(vno);
      float f = si->modifiedFlatness();
      f-=0.1;
      f = max(f, 0.0f);
      cerr<<"set flatness: "<<f<<endl;
      si->setModifiedFlatness(f);
      tm.recomputeNeighbors();
      ((PickableTri*)triObject)->rerender();
    }
  }
}

// flatness down cb
void quadFlatDownCB(void* quadObject) {
  QuadManipulator tm(pickedQuadStuff.face());
  if(pickedQuadStuff.type() == PickedStuff::PICK_SECTOR){
    VnoType vno = tm.headVno(pickedQuadStuff.eno());
    if(tm.getVertexTag(vno) == QuadManipulator::CORNER_VERTEX) {
      // get the sector information
      SectorInfo* si = tm.getSectorInfo(vno);
      float f = si->modifiedFlatness();
      f-=0.1;
      cerr<<"set flatness: "<<f<<endl;
      f = max(f, 0.0f);
      si->setModifiedFlatness(f);
      tm.recomputeNeighbors();
      ((PickableQuad*)quadObject)->rerender();
    }
  }
}


//----------------------------------------------------------------------

// theta higher cb
void triThetaUpCB(void* triObject) {
  TriManipulator tm(pickedTriStuff.face());
  if(pickedTriStuff.type() == PickedStuff::PICK_SECTOR){
    VnoType vno = tm.headVno(pickedTriStuff.eno());
    if(tm.getVertexTag(vno) == TriManipulator::CORNER_VERTEX) {
      // get the sector information
      SectorInfo* si = tm.getSectorInfo(vno);
      assert(si);
      float f = si->theta();
      f+=0.05;
      if(si->sectorTag() == SectorInfo::CONVEX_SECTOR)
	f = min(float(M_PI), max(f, 0.0f));
      else 
	f = min(2.0f*float(M_PI), max(f, float(M_PI)));
      cerr<<"set theta: "<<f<<endl;
      si->setTheta(f);
      tm.recomputeNeighbors();
      ((PickableTri*)triObject)->rerender();
    }
  }
}

// theta higher cb
void quadThetaUpCB(void* quadObject) {
  QuadManipulator tm(pickedQuadStuff.face());
  if(pickedQuadStuff.type() == PickedStuff::PICK_SECTOR){
    VnoType vno = tm.headVno(pickedQuadStuff.eno());
    if(tm.getVertexTag(vno) == QuadManipulator::CORNER_VERTEX) {
      // get the sector information
      SectorInfo* si = tm.getSectorInfo(vno);
      float f = si->theta();
      f+=0.05;
      if(si->sectorTag() == SectorInfo::CONVEX_SECTOR)
	f = min(float(M_PI), max(f, 0.0f));
      else 
	f = min(2.0f*float(M_PI), max(f, float(M_PI)));
      cerr<<"set theta: "<<f<<endl;
      si->setTheta(f);
      tm.recomputeNeighbors();
      ((PickableQuad*)quadObject)->rerender();
    }
  }
}

// theta higher cb
void triThetaDownCB(void* triObject) {
  TriManipulator tm(pickedTriStuff.face());
  if(pickedTriStuff.type() == PickedStuff::PICK_SECTOR){
    VnoType vno = tm.headVno(pickedTriStuff.eno());
    if(tm.getVertexTag(vno) == TriManipulator::CORNER_VERTEX) {
      // get the sector information
      SectorInfo* si = tm.getSectorInfo(vno);
      float f = si->theta();
      f-=0.05;
      if(si->sectorTag() == SectorInfo::CONVEX_SECTOR)
	f = min(float(M_PI), max(f, 0.0f));
      else 
	f = min(2.0f*float(M_PI), max(f, float(M_PI)));
      cerr<<"set theta: "<<f<<endl;
      si->setTheta(f);
      tm.recomputeNeighbors();
      ((PickableTri*)triObject)->rerender();
    }
  }
}

// theta down cb
void quadThetaDownCB(void* quadObject) {
  QuadManipulator tm(pickedQuadStuff.face());
  if(pickedQuadStuff.type() == PickedStuff::PICK_SECTOR){
    VnoType vno = tm.headVno(pickedQuadStuff.eno());
    if(tm.getVertexTag(vno) == QuadManipulator::CORNER_VERTEX) {
      // get the sector information
      SectorInfo* si = tm.getSectorInfo(vno);
      float f = si->theta();
      f-=0.05;
      if(si->sectorTag() == SectorInfo::CONVEX_SECTOR)
	f = min(float(M_PI), max(f, 0.0f));
      else 
	f = min(2.0f*float(M_PI), max(f, float(M_PI)));
      cerr<<"set theta: "<<f<<endl;
      si->setTheta(f);
      tm.recomputeNeighbors();
      ((PickableQuad*)quadObject)->rerender();
    }
  }
}

//----------------------------------------------------------------------

void triPickCB(PickedStuff* pickedStuff, void* triObject) {
  pickedTriStuff = *pickedStuff;   // remember the picked stuff
  TriManipulator tm(pickedStuff->face());
  bool res = false;
  switch(pickedStuff->type()) {
  case PickedStuff::PICK_VERTEX:   
    res = tm.toggleVertex(pickedStuff->eno()); 
    break;
  case PickedStuff::PICK_EDGE:     
    res = tm.toggleEdge(pickedStuff->eno()); 
    break;
  case PickedStuff::PICK_SECTOR:   
    res = tm.toggleSector(pickedStuff->eno()); 
    break;
  case PickedStuff::PICK_NORMAL:   break;
  case PickedStuff::PICK_NOTHING:  break;
  }
  if(res) {
    tm.recomputeNeighbors();
    ((PickableTri*)triObject)->rerender();
  }
}

void quadPickCB(PickedStuff* pickedStuff, void* quadObject) {
  pickedQuadStuff = *pickedStuff;   // remember the picked stuff
  QuadManipulator tm(pickedStuff->face());
  bool res = false;
  switch(pickedStuff->type()) {
  case PickedStuff::PICK_VERTEX:   
    res = tm.toggleVertex(pickedStuff->eno()); 
    break;
  case PickedStuff::PICK_EDGE:     
    res = tm.toggleEdge(pickedStuff->eno()); 
    break;
  case PickedStuff::PICK_SECTOR:   
    res = tm.toggleSector(pickedStuff->eno()); 
    break;
  case PickedStuff::PICK_NORMAL:   break;
  case PickedStuff::PICK_NOTHING:  break;
  }
  if(res) {
    tm.recomputeNeighbors();
    ((PickableQuad*)quadObject)->rerender();
  }
}

// subdivide a triangular mesh object
void subTriObjectCB(void* o) { 
  PickableTri* triMeshObject = (PickableTri*) o;
  TriMesh* tm = &triMeshObject->getMesh();
  tm->subdivide(tm->meshDepth()+1);
  // clear display lists
  triMeshObject->rerender();

  cerr<<"subdivide (Loop). depth = "<<tm->meshDepth()<<endl;
}

// subdivide a quadrialteral mesh object
void subQuadObjectCB(void* o) { 
  PickableQuad* quadMeshObject = (PickableQuad*) o;
  quadMeshObject->getMesh().subdivide(quadMeshObject->getMesh().meshDepth()+1);
  // clear display lists
  quadMeshObject->rerender();

  cerr<< "subdivide (Catmull-Clark). depth = "
      << quadMeshObject->getMesh().meshDepth()<<endl;
}

// write the control mesh
void writeTriCtrlCB(void* o) {
  cerr<<"write control mesh to trimesh.wrl"<<endl;

  TriMesh triMesh = ((PickableTri*)o)->getMesh();

  // create a flat mesh
  TagFlatMesh tagFlatMesh;
  triMesh.toTagFlatMesh(&tagFlatMesh);

  // create an ivgraph and write to file "trimesh.wrl"
  TagIvGraph tagIvGraph;
  tagIvGraph.fromTagFlatMesh(&tagFlatMesh);
  tagIvGraph.write("trimesh.wrl");
}

// write the control mesh
void writeQuadCtrlCB(void* o) {
  cerr<<"write control mesh to quadmesh.wrl"<<endl;

  QuadMesh quadMesh = ((PickableQuad*)o)->getMesh();

  // create a flat mesh
  TagFlatMesh tagFlatMesh;
  quadMesh.toTagFlatMesh(&tagFlatMesh);

  // create an ivgraph and write to file "quadmesh.wrl"
  TagIvGraph tagIvGraph;
  tagIvGraph.fromTagFlatMesh(&tagFlatMesh);
  tagIvGraph.write("quadmesh.wrl");
}


// write the control mesh
void writeTriSubCB(void* o) {
  cerr<<"write subdivided mesh to trimeshs.wrl"<<endl;

  TriMesh* leafMesh = ((PickableTri*)o)->getMesh().leafMesh();
  TriMesh leafClone;
  leafClone.setClone(*leafMesh);
  // create a flat mesh
  TagFlatMesh tagFlatMesh;
  leafClone.toTagFlatMesh(&tagFlatMesh);
  
  // create an ivgraph and write to file "trimeshs.wrl"
  TagIvGraph tagIvGraph;
  tagIvGraph.fromTagFlatMesh(&tagFlatMesh);
  tagIvGraph.write("trimeshs.wrl");

  delete leafMesh;
}

// write the control mesh
void writeQuadSubCB(void* o) {
  cerr<<"write subdivided mesh to quadmeshs.wrl"<<endl;

  QuadMesh* leafMesh = ((PickableQuad*)o)->getMesh().leafMesh();
  QuadMesh leafClone;
  leafClone.setClone(*leafMesh);

  // create a flat mesh
  TagFlatMesh tagFlatMesh;
  leafClone.toTagFlatMesh(&tagFlatMesh);

  // create an ivgraph and write to file "tmesh.wrl"
  TagIvGraph tagIvGraph;
  tagIvGraph.fromTagFlatMesh(&tagFlatMesh);
  tagIvGraph.write("quadmeshs.wrl");

  delete leafMesh;
}

void toggleTriViewerStateCB(void* o) {
  unsigned char pickall =  
    PickedStuff::PICK_NOTHING | 
    PickedStuff::PICK_VERTEX | 
    PickedStuff::PICK_EDGE | 
    PickedStuff::PICK_SECTOR | 
    PickedStuff::PICK_NORMAL;
  
  unsigned char& c = ((PickableTri*)o)->tlRenderMode();
  if(c == pickall)            
    c = PickedStuff::PICK_NOTHING;          // show mesh
  else if(c== PickedStuff::PICK_NOTHING)    
    c = PickedStuff::PICK_SECTOR;          // show tl mesh
  else if (c == PickedStuff::PICK_SECTOR)
    c = pickall;              // show both
  
  ((PickableTri*)o)->rerender();
  
}

void toggleQuadViewerStateCB(void* o) {
  unsigned char pickall =  
    PickedStuff::PICK_NOTHING | 
    PickedStuff::PICK_VERTEX | 
    PickedStuff::PICK_EDGE | 
    PickedStuff::PICK_SECTOR | 
    PickedStuff::PICK_NORMAL;

  unsigned char& c = ((PickableQuad*)o)->tlRenderMode();
  if(c == pickall)            
    c = PickedStuff::PICK_NOTHING;          // show mesh
  else if(c== PickedStuff::PICK_NOTHING)    
    c = PickedStuff::PICK_SECTOR;          // show tl mesh
  else if (c == PickedStuff::PICK_SECTOR)
    c = pickall;              // show both
  
  ((PickableQuad*)o)->rerender();
  
}


//----------------------------------------------------------------------------

void registerCB(PickViewer* triViewer, 
		PickViewer* quadViewer,
		PickableTri* triObject,
		PickableQuad* quadObject) {

  // toggle call back on space
  triViewer->addKeyCallback
    (' ', PickViewer::CBPairType(&toggleViewerStateCB, triViewer));
  quadViewer->addKeyCallback
    (' ', PickViewer::CBPairType(&toggleViewerStateCB, quadViewer));
  
  // subdivide call back on s
  triViewer->addKeyCallback
    ('s', PickViewer::CBPairType(&subTriObjectCB, triObject));
  quadViewer->addKeyCallback
    ('s', PickViewer::CBPairType(&subQuadObjectCB, quadObject));

  // write ctrl mesh on w
  triViewer->addKeyCallback
    ('w', PickViewer::CBPairType(&writeTriCtrlCB, triObject));
  quadViewer->addKeyCallback
    ('w', PickViewer::CBPairType(&writeQuadCtrlCB, quadObject));
  
  // dump subdivided mesh on d
  triViewer->addKeyCallback
    ('d', PickViewer::CBPairType(&writeTriSubCB, triObject));
  quadViewer->addKeyCallback
    ('d', PickViewer::CBPairType(&writeQuadSubCB, quadObject));

  // toggle rendered view with o (for 'o'ther view)
  triViewer->addKeyCallback
    ('o', PickViewer::CBPairType(&toggleTriViewerStateCB, triObject));
  quadViewer->addKeyCallback
    ('o', PickViewer::CBPairType(&toggleQuadViewerStateCB, quadObject));

  triViewer->addPickCallback(triPickCB, triObject);
  quadViewer->addPickCallback(quadPickCB, quadObject);

  triViewer->addSpecialCallback
    ( GLUT_KEY_UP, PickViewer::CBPairType(&triFlatUpCB, triObject));
  quadViewer->addSpecialCallback
    ( GLUT_KEY_UP, PickViewer::CBPairType(&quadFlatUpCB, quadObject));

  triViewer->addSpecialCallback
    ( GLUT_KEY_DOWN, PickViewer::CBPairType(&triFlatDownCB, triObject));
  quadViewer->addSpecialCallback
    ( GLUT_KEY_DOWN, PickViewer::CBPairType(&quadFlatDownCB, quadObject));

  triViewer->addSpecialCallback
    ( GLUT_KEY_RIGHT, PickViewer::CBPairType(&triThetaUpCB, triObject));
  quadViewer->addSpecialCallback
    ( GLUT_KEY_RIGHT, PickViewer::CBPairType(&quadThetaUpCB, quadObject));

  triViewer->addSpecialCallback
    ( GLUT_KEY_LEFT, PickViewer::CBPairType(&triThetaDownCB, triObject));
  quadViewer->addSpecialCallback
    ( GLUT_KEY_LEFT, PickViewer::CBPairType(&quadThetaDownCB, quadObject));
}

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {

  PickViewer::initGL(&argc, argv);
  
  // we have two objects, a quadbased object and a tribased one
  PickableQuad quadObject;
  PickableTri triObject;
  
   // read files
  init(argc, argv, triObject, quadObject);

  // render everything but not normals
  quadObject.tlRenderMode() = triObject.tlRenderMode() = 
      PickedStuff::PICK_VERTEX | PickedStuff::PICK_EDGE | PickedStuff::PICK_SECTOR;
  
  // compute 0 subdivision level
  triObject.getMesh().subdivide(0);
  quadObject.getMesh().subdivide(0);
    
  // create a viewer for the tribased structure
  PickViewer triViewer("triViewer");
  triViewer.setObject(&triObject);
  
  // create a viewer for the quadbased structure
  PickViewer quadViewer("quadViewer");
  quadViewer.setObject(&quadObject);

  quadViewer.setPos(50, 50);
  triViewer.setPos(100, 100);
  
  registerCB(&triViewer, &quadViewer, &triObject, &quadObject);
  
  // enter glut main loop
  glutMainLoop();
  return 0;
}




