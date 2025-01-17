// -*- Mode: c++ -*-
// $Id: convface.h,v 1.2 2000/04/29 07:50:08 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/convface.h,v $


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

#ifndef __CONVFACE_H__
#define __CONVFACE_H__

#include "compat.h"

// a collection of accessors to vertices, children and parent
// this functions are common for triangular and quad hierarchies.
// There is no data associated with this template; the pointers to 
// children and parent are in BaseFace, vertices are stored in 
// triangle and quad specific classes, because for triangles 
// vertex array is fixed size (we assume surfaces triangulated 
// before subdivision), but for quad hierarchies the top level can 
// have faces with arbitrary number of vertices.

template<class BaseFace>
class ConvenientFaceTp : public BaseFace {
public:
  typedef typename BaseFace::Face Face;
  typedef typename BaseFace::TLFace TLFace;


  ConvenientFaceTp() { ; }
  virtual ~ConvenientFaceTp() { ; }
 
  Face* parent() const { return (Face*)_p; }
  Face* child(CnoType c)  const { assert(checkCno(c)); return &_c[c]; }
  Vertex* vert(VnoType v) const { assert(checkVno(v)); return _v[v]; }

  bool isLeaf() const { return childCount() == 0; }
  bool isToplevel() const { return _p == 0; }
  CnoType no() const { return _no; }

  Face* toplevelFace() const 
    { return isRoot()? ((Face*)this) : parent->toplevelFace(); }

  int depth() const 
    { if(!isToplevel()) return 1+parent()->depth(); else return 0; }

  // vertex methods
  VnoType nextVno(VnoType v) const 
    { assert(checkVno(v)); return (v+1) % noVtx(); }

  VnoType prevVno(VnoType v) const 
    { assert(checkVno(v)); return (v+noVtx()-1) % noVtx(); }
  Face* parentVertex(VnoType v, VnoType& pv) const { 
    Face* pf = parent();
    if(pf) {
      for(pv = 0; pv < pf->noVtx(); ++pv)
	if(pf->vert(pv) == vert(v)) {
	  return pf;
	}
    }
    pf = 0;
    pv = 0;
    return pf;
  }
  Face* toplevelVertex(VnoType v, VnoType& tv) const { 
    tv = 0;
    if(isToplevel()) {
      tv = v;
      return (Face*)this;
    } else {
      VnoType pv;
      Face* pf = parentVertex(v, pv);
      if(pf) 
	return pf->toplevelVertex(pv, tv);
      else
	return 0;
    }
  }
  // edge methods
  EnoType nextEno(EnoType e) const 
    { return (e<0) ? (-(-e+noVtx()-2)%noVtx()-1) : (e%noVtx()+1) ; }
  EnoType prevEno(EnoType e) const 
    { return (e<0) ?( -(-e)%noVtx()-1) : ((e+noVtx()-2)%noVtx() + 1); }
  EnoType reverseEno(EnoType e) const { return -e; }
  EnoType enoTo(VnoType v) const 
    { assert(checkVno(v)); return (v == 0) ? noVtx() : v; }
  VnoType headVno(EnoType e) const { return (e>0) ? (e%noVtx()) : ((-e)-1); }
  VnoType tailVno(EnoType e) const { return (e>0) ? (e-1) : ((-e)%noVtx()); }
  Vertex* headVert(EnoType e) const { return vert(headVno(e)); }
  Vertex* tailVert(EnoType e) const { return vert(tailVno(e)); }
  Vertex* midVert(EnoType e) const 
    { EnoType me; Face* mf = midEdge(e, me); assert(mf); 
    return mf->headVert(me); }

  Face* toplevelEdge(EnoType e, EnoType &te) const { 
    if(isToplevel()) { te = e; return (Face*)this; } 
    else {
      EnoType pe;
      Face* pf = parentEdge(e, pe); 
      if(pe) return pf->toplevelEdge(pe, te);
      else { te = 0; return 0; }
    }
  }

  OrientationType orientation() const { return _orient; }
  EnoType directEno(EnoType e, OrientationType o = CCW) const { 
    if(o == _orient) return abs(e);
    else return -abs(e);
  }
  OrientationType edgeDir(EnoType e) const 
    { return (e>0)? _orient : reverseOrientation(_orient); }

  bool checkCno(CnoType c) const
    { return (this != 0) && (c < childCount()); }
  bool checkEno(EnoType e) const
    { return (this != 0) && (0 < abs(e)) && (abs(e) < noVtx()+1); }
  bool checkVno(VnoType v) const
    { return (this != 0) && (v < noVtx()); }

  // method for replacing a vertex...
  void setVert(VnoType v, Vertex* vert) { 
    assert(checkVno(v));
    assert(v < 4);
    Vertex::ref(vert); 
    Vertex::unref(_v[v]); 
    _v[v] = vert; 
  }
};

#endif /* __CONVFACE_H__ */
