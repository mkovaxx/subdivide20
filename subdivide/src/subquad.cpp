// -*- Mode: c++ -*-
// $Id: subquad.cpp,v 1.6 2001/02/07 05:29:40 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/src/subquad.cpp,v $

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

#include "subquad.h"

void Quad::midSub(int d) {
  if(isLeaf())
    makeChildren(d);

  cvec3f centerPos(0);
  for(EnoType e = 1; e < noVtx()+1; ++e) {
    centerPos += headPos(e, d);
    if(!hasMidPos(e, d)) 
      setMidPos(e, d, 0.5f * (headPos(e, d) + tailPos(e, d)));
    if(!hasHeadPos(e, d+1))
      setHeadPos(e, d+1, headPos(e, d));
  }
  if(!hasCenterPos(d))
    setCenterPos(d, 1/float(noVtx()) * centerPos);
  
  // clear children mid points points
  for(CnoType c = 0; c < childCount(); ++c) 
    child(c)->clearFace(d+1);
}


void Quad::subdivide(int d) {
  setCenterPos(d, computeFacePoint(d));

  EnoType e;
  for(e = 1; e < noVtx()+1; ++e)
    computeVertexPoint(e, d);
  
  for(e = 1; e < noVtx()+1; ++e) 
    computeEdgePoint(e, d);
  
  //  for(e = 1; e < noVtx()+1; ++e)
  //    computeEdgeNormal(e, d);
  
  //  computeFaceNormal(d);
  
  // clear children mid points points
  for(CnoType c = 0; c < childCount(); ++c) 
    child(c)->clearFace(d+1);
}


void Quad::clearFace(int d) {
  if(childCount() > 0) {
    for(EnoType e = 1; e < noVtx()+1; ++e) {
      EnoType me;
      Face* mf = midEdge(e, me);
      mf->headVert(me)->unset();
      mf->setNormal(mf->headVno(me), 0);

      headVert(e)->set(d);
    }
    EnoType ce;
    Face* cf = centerEdge(ce);
    cf->setNormal(cf->headVno(ce), 0);
    cf->headVert(ce)->unset();
  }
}


cvec3f Quad::computeVertexPoint(EnoType e, int d) {
  if(isLeaf())
    makeChildren(d);

  if(hasHeadPos(e, d+1))
    return headPos(e, d+1);
  else {
    cvec3f tmp;
    if(headVert(e)->isSpecial()) {
      SubdivideType subdiv;
      subdiv.getHdRuleAndRing(this, e, d); 
      tmp = subdiv.computeVertexPoint();
    } else {
      FaceRingType tr;
      tr.collectRing(this, e);
      assert(tr.isClosed());
      
      cvec3f tmpposE(0);
      cvec3f tmpposF(0);
      for(uint i = 0;  i < tr.noVtx(); ++i) {
	EnoType e;
	Face* f = tr.face(i, e);
	tmpposE += f->headPos(e, d);
	tmpposF += f->headPos(f->nextEno(e), d);
      }
      
      float k = tr.noVtx();
      float beta1 = 3.0/2.0/k;
      float beta2 = 1.0/4.0/k;
      tmp =  (1.0 - beta1 - beta2) * headPos(e, d) + 
	beta1 / k * tmpposE + beta2 / k * tmpposF;
    }
    setHeadPos(e, d+1, tmp);
    return tmp;
  }
}


void Quad::computeEdgeNormal(EnoType e, int d) {

  e = directEno(e);

  EnoType me;
  Face* mf = midEdge(e, me);

  cvec3f a0, a1, a2;

  if(!mf->hasNormal(mf->headVno(me))) {
    
    cvec3f cp(computeEdgePoint(e, d)),
      v0(headPos(e, d+1)),
      v1(computeFacePoint(d)),
      v2(tailPos(e, d+1)),
      f0(computeEdgePoint(nextEno(e), d)),
      f1(computeEdgePoint(prevEno(e), d));
    
    if(edgeTag(e) == Face::CREASE_EDGE) {
      a0 = 2.0/3.0f * cp + 1/6.0 * (v0 + v2);
      a1 = -2.0/3.0f * cp - 1/6.0 * v0 + 2/3.0 * v1 - 1/6.0 * v2 +
	1/6.0f * f0 + 1/6.0f * f1;
      a2 = 0.0f * cp + 0.5f * v0 + 0.0 * v1 - 0.5 * v2;
    } else {
      EnoType ne;
      Face* nf = neighbor(e, ne);
      cvec3f f2(nf->computeEdgePoint(nf->prevEno(ne), d)),
	v3(nf->computeFacePoint(d)),
	f3(nf->computeEdgePoint(nf->nextEno(ne), d));
      
      a0 = 4.0f/(4.0f+5.0f) * cp 
	+ 1.0f/4.0f * (v0 + v1 + v2 + v3) 
	+ 1.0f/16.0f * (f0 + f1 + f2 + f3);
      
      a1 = 0.0f * cp 
	+ 0.0f * v0 + 4.0f * v1 + 0.0 * v2 - 4.0f * v3 
	+ 1.0f * f0 + 1.0f * f1 - 1.0 * f2 - 1.0f * f3;
      
      a2 = 0.0f * cp 
	+ 4.0f * v0 + 0.0f * v1 - 4.0 * v2 + 0.0f * v3 
	+ 1.0f * f0 - 1.0f * f1 - 1.0 * f2 + 1.0f * f3;
    }
    
    EnoType me;
    Face* mf = midEdge(e, me);
    mf->setNormal(mf->headVno(me), a2.cross(a1).dir());
    mf->headVert(me)->setLimitPos(a0);
  }
}

void Quad::computeFaceNormal(int d) {
  EnoType me;
  Face* mf = centerEdge(me);
  assert(mf->headVert(me) == centerVert());
  
  if(noVtx() != 4) {
    SubdivideType subdiv;
    subdiv.getHdRuleAndRing(mf, me, d+1);
    mf->setNormal(me, subdiv.computeNormal());
    mf->vert(mf->headVno(me))->setLimitPos(subdiv.computeLimit());    
  } else {
    cvec3f cp(computeFacePoint(d));
    cvec3f v0(midPos(1, d)), f0(tailPos(1, d+1)), 
      v1(midPos(2, d)), f1(tailPos(2, d+1)),
      v2(midPos(3, d)), f2(tailPos(3, d+1)),
      v3(midPos(4, d)), f3(tailPos(4, d+1));
    
    cvec3f a0 = 4.0f/(4.0f+5.0f) * cp 
      + 1.0f/4.0f * (v0 + v1 + v2 + v3) 
      + 1.0f/16.0f * (f0 + f1 + f2 + f3);
    
    cvec3f a1 = 0.0f * cp 
      + 4.0f * (0.0f * v0 + 1.0f * v1 + 0.0f * v2 + (-1.0f) * v3)
      + 1.0f * (1.0f * f0 + 1.0f * f1 + (-1.0f) * f2 + (-1.0f) * f3);
    
    cvec3f a2 = 0.0f * cp 
      + 4.0f * (1.0f * v0 + 0.0f * v1 + (-1.0f) * v2 + (0.0f) * v3)
      + 1.0f * (1.0f * f0 + (-1.0f) * f1 + (-1.0f) * f2 + (1.0f) * f3);
  
    mf->setNormal(mf->headVno(me), a2.cross(a1).dir());
    mf->headVert(me)->setLimitPos(a0);
  }
}


void Quad::computeNormalAndLimit(int d) {
  SubdivideType subdiv;

  // compute edge points (and vertex points by chance)
  for(EnoType e = 1; e < noVtx()+1; ++e) {
    if(!hasNormal(headVno(e))) {
      subdiv.getHdRuleAndRing(this, e, d);
      setNormal(headVno(e), subdiv.computeNormal());
      headVert(e)->setLimitPos(subdiv.computeLimit());
    }
  }
}



cvec3f Quad::computeFacePoint(int d) {
  if(isLeaf())
    makeChildren(d);

  if(hasCenterPos(d))
    return centerPos(d);
  else {
    
    // compute average...
    cvec3f tmp;
    for(VnoType v = 0; v < noVtx(); ++v)
      tmp += pos(v, d);
    tmp = 1.0f/float(noVtx()) * tmp;
    
    // modify average...
    int relevantCnt = 0;
    cvec3f relevantPos(0);
    for(EnoType e = 1; e < noVtx()+1; ++e)
      if(this->headVert(e)->isSpecial()) {
	SectorInfo* si = sectorInfo(headVno(e));
	if(si && (si->hasModifiedNormal() || si->hasModifiedFlatness())) {
	  
	SubdivideType subdiv;
	subdiv.getHdRuleAndRing(this, e, d);
	
	float x1 = subdiv.rule()->x1().faceC[subdiv.ring().edgeIndex()];
	float x2 = subdiv.rule()->x2().faceC[subdiv.ring().edgeIndex()];

    // variable e1 is not referenced in release mode
	EnoType e1;
	assert(this == subdiv.ring().face(subdiv.ring().edgeIndex(), e1));
	cvec3f res(tmp);
	
	SectorInfo* si = 0;
	if((si = sectorInfo(headVno(e))) && si->hasModifiedFlatness()) {

	  res = subdiv.modifyFlatness(res, x1, x2);  
	}
	if((si = sectorInfo(headVno(e))) && si->hasModifiedNormal()) {

	  res = (subdiv.ring().isClosed())? 
	    subdiv.modifyClosedNormal(res, x1, x2) : subdiv.modifyOpenNormal(res, x1, x2);
	}
	++relevantCnt;
	relevantPos += res;
	}
      }
    
    if(relevantCnt != 0)
      tmp = 1.0f / float(relevantCnt) * relevantPos;
    setCenterPos(d, tmp);
    return tmp;
  }
}


cvec3f Quad::computeEdgePoint(EnoType e, int d) {
  if(isLeaf())
    makeChildren(d);

  if(hasMidPos(e, d))
    return midPos(e,d);
  else {
    cvec3f tmp;
    if(headVert(e)->isSpecial() || tailVert(e)->isSpecial()) {
      SubdivideType subdiv;
      if(!SubdivideType::hdRelevantToMidpoint(this, -e)) {
	subdiv.getHdRuleAndRing(this, e, d);
	tmp = subdiv.computeEdgePoint();
      } else if(!SubdivideType::hdRelevantToMidpoint(this, e)) {
	subdiv.getHdRuleAndRing(this, -e, d);
	tmp = subdiv.computeEdgePoint();
      } else {
	subdiv.getHdRuleAndRing(this, e, d);
	tmp = 0.5 * subdiv.computeEdgePoint();
	subdiv.getHdRuleAndRing(this, -e, d);
	tmp += 0.5 * subdiv.computeEdgePoint();
      }
    } else {
      EnoType ne;
      Face* nf = neighbor(e, ne);
      assert(nf);
      tmp = 1/4.0f * (headPos(e, d) + tailPos(e, d) + 
		      computeFacePoint(d) + nf->computeFacePoint(d));
    }
    setMidPos(e, d, tmp);
    return tmp;    
  }
}


template<class QuadRing>
CreaseQuadRuleTable QuadRuleTableTp<QuadRing>::_creaseRuleTable;
template<class QuadRing>
InteriorQuadRuleTable QuadRuleTableTp<QuadRing>::_interiorRuleTable;
template<class QuadRing>
ConvexQuadRuleTable QuadRuleTableTp<QuadRing>::_convexRuleTable;
template<class QuadRing>
ConcaveQuadRuleTable QuadRuleTableTp<QuadRing>::_concaveRuleTable;


