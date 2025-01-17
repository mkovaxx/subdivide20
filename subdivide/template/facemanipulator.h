// -*- Mode: c++ -*-
// $Id: facemanipulator.h,v 1.8 2000/04/30 01:39:28 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/facemanipulator.h,v $

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

#ifndef __FACEMANIPULATOR_H__
#define __FACEMANIPULATOR_H__

#include "compat.h"

#include <set>

#include "sectorinfo.h"

// Support for modifying tags 
// on vertices and edges of a face.

template<class Mesh>
class FaceManipulatorTp {
public:
  typedef typename Mesh::FaceType FaceType;
  typedef typename Mesh::FaceType::FaceRingType FaceRingType;
  typedef typename set<FaceType*>::iterator FaceIter;

  FaceManipulatorTp(FaceType* f0) : f(f0) { ; }
   
  bool toggleEdge(EnoType eno);
  bool toggleVertex(EnoType eno);
  bool toggleSector(EnoType eno);
  void recomputeNeighbors();
private:
  FaceType* f;

  void fixVertexToggle(EnoType e);
  void specializeEdge(typename Mesh::FaceType* f, EnoType e) {
    EnoType he, te;
    if(!f->isLeaf()) {
      assert(f->headVert(e)->isSpecial());
      assert(f->tailVert(e)->isSpecial());
      f->midVert(e)->makeSpecial();
      typename Mesh::FaceType* hf = f->headSubEdge(e, he);
      typename Mesh::FaceType* tf = f->tailSubEdge(e, te);
      specializeEdge(tf, te);
      specializeEdge(hf, he);
    }
  }
};

template<class Mesh>
void  
FaceManipulatorTp<Mesh>::recomputeNeighbors() {
  // three ring face set
  set<FaceType*> s0;
  set<FaceType*> s1;
  
  s0.insert(f);
  
  // collect affected triangles
  for(int k = 0; k < 2; ++k) {
    FaceIter it;

    for(it = s0.begin(); it != s0.end(); ++it) {
      FaceType* f = (*it);
      for(EnoType e = 1; e < f->noVtx()+1; ++e) {
	FaceRingType fr;
	fr.collectRing(f, e);
	for(uint i = 0; i < fr.noFace(); ++i) {
	  EnoType ringEno;
	  s1.insert(fr.face(i, ringEno));
	}
      }
    }
    s1.swap(s0);
  }
  
  // build local mesh
  Mesh mk;

  FaceIter it;

  for(it = s0.begin(); it != s0.end(); ++it) {
    mk.insertFace((*it));
  }
  
  // subdivide mesh
  mk.subdivide();
  
}

template<class Mesh>  
bool 
FaceManipulatorTp<Mesh>::toggleEdge(EnoType e) {
  typedef typename Mesh::FaceType FaceType;
    
    bool res = false;
    
    assert(f->isToplevel());
    typename Mesh::TLFaceType* tlf = (typename Mesh::TLFaceType*) f;
    EnoType ne;
    typename Mesh::FaceType* nf = tlf->neighbor(e, ne);
    if(nf) {
      if(tlf->edgeTag(e) == FaceType::CREASE_EDGE) {
	tlf->setEdgeTag(e, FaceType::NOTAG_EDGE);
	fixVertexToggle(e);
	fixVertexToggle(-e);
	res = true;
      } else {
	tlf->setEdgeTag(e, FaceType::CREASE_EDGE);
	specializeEdge(f, e);
	fixVertexToggle(e);
	fixVertexToggle(-e);
	res = true;
      }
    }
    return res;
  }

template<class Mesh>
bool FaceManipulatorTp<Mesh>::toggleVertex(EnoType e) {
  typedef typename Mesh::FaceType FaceType;
  bool res = false;
    assert(f->isToplevel());

    typename FaceType::FaceRingType fr;
    fr.collectRing(f, e);
    int creaseCount = 0;
    uint i;
    for(i = 0; i < fr.noVtx(); ++i) {
      EnoType ringEno;
      FaceType* ringFace = fr.face(i, ringEno); 
      if(ringFace->edgeTag(ringEno) == FaceType::CREASE_EDGE) 
	++creaseCount;
    }

    res = (creaseCount >= 2);

    // remove sectors...
    for(i = 0; i < fr.noFace(); ++i) {
      EnoType ringEno;
      FaceType* ringFace = fr.face(i, ringEno); 
      ((typename Mesh::TLFaceType*)ringFace)->setSectorInfo(ringFace->tailVno(ringEno), 0);
    }

    // new tag
    typename FaceType::VertexTagType vertexTag = f->vertexTag(f->headVno(e));
    if(vertexTag == FaceType::CORNER_VERTEX) 
      if(fr.isClosed()) 
	vertexTag = FaceType::NOTAG_VERTEX;
      else
	vertexTag = FaceType::CREASE_VERTEX;
    else if(vertexTag == FaceType::NOTAG_VERTEX) {
      if(creaseCount == 2) 
	vertexTag = FaceType::CREASE_VERTEX;
      else if(creaseCount > 2) 
	vertexTag = FaceType::CORNER_VERTEX;
    }
    else if(vertexTag == FaceType::CREASE_VERTEX)
      vertexTag = FaceType::CORNER_VERTEX;

    ((typename Mesh::TLFaceType*)f)->setVertexTag(f->headVno(e), vertexTag);

    // add sectors
    if((vertexTag == FaceType::CREASE_VERTEX) ||
       (vertexTag == FaceType::CORNER_VERTEX))
      for(i = 0; i < fr.noFace(); ++i) {
	EnoType ringEno;
	FaceType* ringFace = fr.face(i, ringEno); 
	SectorInfo* si = 0;
	assert(ringFace->tailVert(ringEno) == fr.centerVert());
	if((si = ringFace->sectorInfo(ringFace->tailVno(ringEno))) == 0) {
	  si = new SectorInfo();
	  if(vertexTag == FaceType::CORNER_VERTEX)
	    si->setSectorTag(SectorInfo::CONVEX_SECTOR);
	  ((typename Mesh::TLFaceType*)ringFace)->setSectorInfo(ringFace->tailVno(ringEno), si);
	}
      }

    return res;
  }

template<class Mesh>
bool 
FaceManipulatorTp<Mesh>::toggleSector(EnoType e) {
  typedef typename Mesh::FaceType FaceType; 
  typedef typename Mesh::TLFaceType TLFaceType;
  
  bool res = false;
  assert(f->isToplevel());
  TLFaceType* tlf = (TLFaceType*)f;
  
  if(tlf->vertexTag(tlf->headVno(e)) == FaceType::CORNER_VERTEX) {
    SectorInfo* si = tlf->sectorInfo(tlf->headVno(e));
    res = true;
    if(si->sectorTag() == SectorInfo::CONVEX_SECTOR) 
      si->setSectorTag(SectorInfo::CONCAVE_SECTOR);
    else
      si->setSectorTag(SectorInfo::CONVEX_SECTOR);
  }
  return res;
}

template<class Mesh>
void 
FaceManipulatorTp<Mesh>::fixVertexToggle(EnoType e) {
  typedef typename Mesh::FaceType FaceType;
  typedef typename Mesh::TLFaceType TLFaceType;
  typedef typename FaceType::FaceRingType FaceRingType;
  FaceRingType fr;
  
  fr.collectRing(f, e);
  int creaseCount = 0;
  uint i;
  for(i = 0; i < fr.noVtx(); ++i) {
    EnoType ringEno;
    FaceType* ringFace = fr.face(i, ringEno); 
    if(ringFace->edgeTag(ringEno) == FaceType::CREASE_EDGE) 
      ++creaseCount;
  }
  assert(f->headVert(e) == fr.centerVert());
  if(creaseCount < 2) 
    ((TLFaceType*)f)->setVertexTag(f->headVno(e), FaceType::NOTAG_VERTEX);
  else if(creaseCount == 2) 
    ((TLFaceType*)f)->setVertexTag(f->headVno(e), FaceType::CREASE_VERTEX);      
  else
    ((TLFaceType*)f)->setVertexTag(f->headVno(e), FaceType::CORNER_VERTEX);      
  
  for(i = 0; i < fr.noFace(); ++i) {
    EnoType ringEno;
    FaceType* ringFace = fr.face(i, ringEno); 
    ((TLFaceType*)ringFace)->setSectorInfo(ringFace->tailVno(ringEno), 0);
  }
  
  typename FaceType::VertexTagType vertexTag = f->vertexTag(f->headVno(e));
  if((vertexTag == FaceType::CREASE_VERTEX) ||
     (vertexTag == FaceType::CORNER_VERTEX))
    for(i = 0; i < fr.noFace(); ++i) {
      EnoType ringEno;
      FaceType* ringFace = fr.face(i, ringEno); 
      SectorInfo* si = 0;
      assert(ringFace->tailVert(ringEno) == fr.centerVert());
      if((si = ringFace->sectorInfo(ringFace->tailVno(ringEno))) == 0) {
	si = new SectorInfo();
	if(vertexTag == FaceType::CORNER_VERTEX)
	  si->setSectorTag(SectorInfo::CONVEX_SECTOR);
	((TLFaceType*)ringFace)->setSectorInfo(ringFace->tailVno(ringEno), si);
      }
    }
}

#endif /* __FACEMANIPULATOR_H__ */
