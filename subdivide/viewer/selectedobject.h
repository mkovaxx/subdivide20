// -*- Mode: c++ -*-
// $Id: selectedobject.h,v 1.4 2000/04/29 12:29:27 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/viewer/selectedobject.h,v $

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

#ifndef __SELECTEDOBJECT_H__
#define __SELECTEDOBJECT_H__

#include "compat.h"

#include <set>

#include "pickobject.h"
#include "sectorinfo.h"

template<class Mesh>
class SelectedObjectTp : public SelectedPart {
public:
  SelectedObjectTp() : _selectedEdge(0,0), _selectedType(PickObject::PICK_NOTHING) { ; }
  SelectedObjectTp(PickObject::PickElementType t,
		   typename Mesh::EdgeType e) : _selectedEdge(e), _selectedType(t) { ; }
  PickObject::PickElementType type() const { return _selectedType; }
  typename Mesh::EdgeType edge() const { return _selectedEdge; }
  void setEdge(const typename Mesh::EdgeType& e) { _selectedEdge = e; }
  void setType(PickObject::PickElementType t) { _selectedType = t; }

  virtual bool canModifyFlatness() const {
    typename Mesh::FaceType* f = _selectedEdge.face();
    EnoType e = _selectedEdge.eno();
    if((_selectedType == PickObject::PICK_SECTOR)) {
      SectorInfo* si = f->sectorInfo(f->headVno(e));;
      if(si != 0) {
	return true;
      }
    } 
    return false;
  }
  
  virtual bool canModifyTheta() const {
    typedef typename Mesh::FaceType FaceType;

    typename Mesh::FaceType* f = _selectedEdge.face();
    EnoType e = _selectedEdge.eno();
    if((_selectedType == PickObject::PICK_SECTOR) &&
       (f->vertexTag(f->headVno(e)) == FaceType::CORNER_VERTEX)) {
      SectorInfo* si = f->sectorInfo(f->headVno(e));
      if(si->sectorTag() == SectorInfo::CONCAVE_SECTOR)
	return true;
    } 
    return false;
  }
  
  virtual void setFlatness(float flat) {
    assert(canModifyFlatness());
    typename Mesh::FaceType* f = _selectedEdge.face();
    EnoType e = _selectedEdge.eno();
    f->sectorInfo(f->headVno(e))->setModifiedFlatness(flat);
    localSub(_selectedEdge.face());
  }
  
  virtual void setTheta(float theta) {
    assert(canModifyTheta());
    typename Mesh::FaceType* f = _selectedEdge.face();
    EnoType e = _selectedEdge.eno();
    f->sectorInfo(f->headVno(e))->setTheta(theta);
    localSub(_selectedEdge.face());
  }
  
  virtual float getFlatness() const {
    assert(canModifyFlatness());
    typename Mesh::FaceType* f = _selectedEdge.face();
    EnoType e = _selectedEdge.eno();
    return f->sectorInfo(f->headVno(e))->modifiedFlatness();
  }
  
  virtual float getTheta() const {
    assert(canModifyTheta());
    typename Mesh::FaceType* f = _selectedEdge.face();
    EnoType e = _selectedEdge.eno();
    return f->sectorInfo(f->headVno(e))->theta();
  }
  
  virtual bool toggle() {
    bool res = false;
    if(_selectedType == PickObject::PICK_EDGE) 
      res = toggleEdge(_selectedEdge.face(), _selectedEdge.eno());
    else if(_selectedType == PickObject::PICK_VERTEX)
      res = toggleVertex(_selectedEdge.face(), _selectedEdge.eno());
    else if(_selectedType == PickObject::PICK_SECTOR)
      res = toggleSector(_selectedEdge.face(), _selectedEdge.eno());
    
    if(res) 
      localSub(_selectedEdge.face());
    
    return res;
  }

  virtual void localSub() { localSub(); }

private:
  typedef typename Mesh::EdgeType EdgeType;
  EdgeType _selectedEdge;
  PickObject::PickElementType _selectedType;

  bool toggleEdge(typename Mesh::FaceType* f, EnoType e) {
    typedef typename Mesh::FaceType FaceType;

    bool res = false;

    assert(f->isToplevel());
    typename Mesh::TLFaceType* tlf = (typename Mesh::TLFaceType*) f;
    EnoType ne;
    typename Mesh::FaceType* nf = tlf->neighbor(e, ne);
    if(nf) {
      if(tlf->edgeTag(e) == FaceType::CREASE_EDGE) {
	tlf->setEdgeTag(e, FaceType::NOTAG_EDGE);
	fixVertexToggle(tlf, e);
	fixVertexToggle(tlf, -e);
	res = true;
      } else {
	tlf->setEdgeTag(e, FaceType::CREASE_EDGE);
	specializeEdge(f, e);
	fixVertexToggle(tlf, e);
	fixVertexToggle(tlf, -e);
	res = true;
      }
    }
    return res;
  }

  void localSub(typename Mesh::FaceType* f) {
    // three ring face set
    set<Mesh::FaceType*> s0;
    set<Mesh::FaceType*> s1;

    s0.insert(f);

    // collect affected triangles
    for(int k = 0; k < 2; ++k) {
      set<Mesh::FaceType*>::iterator it;
      for(it = s0.begin(); it != s0.end(); ++it) {
	typename Mesh::FaceType* f = (*it);
	for(EnoType e = 1; e < f->noVtx()+1; ++e) {
	  typename Mesh::FaceType::FaceRingType fr;
	  fr.collectRing(f, e);
	  for(int i = 0; i < fr.noFace(); ++i) {
	    EnoType ringEno;
	    s1.insert(fr.face(i, ringEno));
	  }
	}
      }
      s1.swap(s0);
    }

    // build local mesh
    Mesh mk;
    set<Mesh::FaceType*>::iterator it;
    for(it = s0.begin(); it != s0.end(); ++it) {
      mk.insertFace((*it));
    }

    // subdivide mesh
    mk.subdivide();
    
  }

  bool toggleVertex(typename Mesh::FaceType* f, EnoType e) {
    typedef typename Mesh::FaceType FaceType;
    bool res = false;
    assert(f->isToplevel());

    typename FaceType::FaceRingType fr;
    fr.collectRing(f, e);
    int creaseCount = 0;
    int i;
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
  bool toggleSector(typename Mesh::FaceType* f, EnoType e) {
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

  void fixVertexToggle(typename Mesh::FaceType* f, EnoType e) {
    typedef typename Mesh::FaceType FaceType;
    typedef typename Mesh::TLFaceType TLFaceType;
    typedef typename FaceType::FaceRingType FaceRingType;
    FaceRingType fr;

    fr.collectRing(f, e);
    int creaseCount = 0;
    int i;
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

#endif /* __SELECTEDOBJECT_H__ */
