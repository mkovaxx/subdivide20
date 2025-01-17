// -*- Mode: c++ -*-
// $Id: quadmanipulator.cpp,v 1.3 2000/04/29 09:27:19 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/src/quadmanipulator.cpp,v $

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
#include "subquad.h"
#include "quadmanipulator.h"

#include "tagmesh.h"
#include "facemanipulator.h"

QuadManipulator::QuadManipulator(void *f) : _t((Quad*)f) { ; }

bool QuadManipulator::isBoundaryEdge(EnoType eno) {
  EnoType ne;
  Quad* nf = _t->neighbor(eno, ne);
  return (nf == 0);
}

QuadManipulator::VertexTagType QuadManipulator::getVertexTag(VnoType vno) 
{ assert(_t); return VertexTagType(_t->vertexTag(vno)); }

void QuadManipulator::setVertexTag(VnoType vno, VertexTagType vertexTag) 
{ assert(_t); ((TLQuad*)_t)->setVertexTag(vno, Quad::VertexTagType(vertexTag)); }

QuadManipulator::EdgeTagType QuadManipulator::getEdgeTag(EnoType eno) 
{ assert(_t); return QuadManipulator::EdgeTagType(_t->edgeTag(eno)); }

void QuadManipulator::setEdgeTag(EnoType eno, EdgeTagType edgeTag) 
{ assert(_t); ((TLQuad*)_t)->setEdgeTag(eno, Quad::EdgeTagType(edgeTag)); }

SectorInfo* QuadManipulator::getSectorInfo(VnoType vno) 
{ assert(_t); return _t->sectorInfo(vno); }

void QuadManipulator::setSectorInfo(VnoType vno, SectorInfo* sectorInfo)
{ assert(_t); ((TLQuad*)_t)->setSectorInfo(vno, sectorInfo); }

cvec3f QuadManipulator::getNormalDir(VnoType vno) 
{ assert(_t); return _t->normal(vno); }

cvec3f QuadManipulator::getVertexPos(VnoType vno) 
{ assert(_t); return _t->pos(vno, 0); }

void QuadManipulator::setVertexPos(VnoType vno, const cvec3f& pos) 
{ assert(_t); _t->setPos(vno, 0, pos); }

EnoType QuadManipulator::headVno(EnoType eno) { return _t->headVno(eno); }
EnoType QuadManipulator::tailVno(EnoType eno) { return _t->tailVno(eno); }
VnoType QuadManipulator::noVtx() { return _t->noVtx(); }

bool QuadManipulator::toggleEdge(EnoType eno) {
  typedef FaceManipulatorTp<TagMeshTp<Quad> > FaceManiType;
  FaceManiType faceMani(_t);
  return faceMani.toggleEdge(eno);
}

bool QuadManipulator::toggleVertex(EnoType eno) {
  typedef FaceManipulatorTp<TagMeshTp<Quad> > FaceManiType;
  FaceManiType faceMani(_t);
  return faceMani.toggleVertex(eno);
}

bool QuadManipulator::toggleSector(EnoType eno) {
  typedef FaceManipulatorTp<TagMeshTp<Quad> > FaceManiType;
  FaceManiType faceMani(_t);
  return faceMani.toggleSector(eno);
}

void QuadManipulator::recomputeNeighbors() {
  typedef FaceManipulatorTp<TagMeshTp<Quad> > FaceManiType;
  FaceManiType faceMani(_t);
  faceMani.recomputeNeighbors();
}








