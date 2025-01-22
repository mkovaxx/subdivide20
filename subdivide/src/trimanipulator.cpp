// -*- Mode: c++ -*-
// $Id: trimanipulator.cpp,v 1.3 2000/04/29 09:27:19 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/src/trimanipulator.cpp,v $

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

#include "trimanipulator.hpp"
#include "compat.hpp"
#include "subtri.hpp"

#include "facemanipulator.hpp"
#include "tagmesh.hpp"

TriManipulator::TriManipulator(void* f) : _t((Tri*)f) { ; }

bool TriManipulator::isBoundaryEdge(EnoType eno) {
    EnoType ne;
    Tri* nf = _t->neighbor(eno, ne);
    return (nf == 0);
}

TriManipulator::VertexTagType TriManipulator::getVertexTag(VnoType vno) {
    assert(_t);
    return VertexTagType(_t->vertexTag(vno));
}

void TriManipulator::setVertexTag(VnoType vno, VertexTagType vertexTag) {
    assert(_t);
    ((TLTri*)_t)->setVertexTag(vno, Tri::VertexTagType(vertexTag));
}

TriManipulator::EdgeTagType TriManipulator::getEdgeTag(EnoType eno) {
    assert(_t);
    return TriManipulator::EdgeTagType(_t->edgeTag(eno));
}

void TriManipulator::setEdgeTag(EnoType eno, EdgeTagType edgeTag) {
    assert(_t);
    ((TLTri*)_t)->setEdgeTag(eno, Tri::EdgeTagType(edgeTag));
}

SectorInfo* TriManipulator::getSectorInfo(VnoType vno) {
    assert(_t);
    return _t->sectorInfo(vno);
}

void TriManipulator::setSectorInfo(VnoType vno, SectorInfo* sectorInfo) {
    assert(_t);
    ((TLTri*)_t)->setSectorInfo(vno, sectorInfo);
}

cvec3f TriManipulator::getNormalDir(VnoType vno) {
    assert(_t);
    return _t->normal(vno);
}

cvec3f TriManipulator::getVertexPos(VnoType vno) {
    assert(_t);
    return _t->pos(vno, 0);
}

void TriManipulator::setVertexPos(VnoType vno, const cvec3f& pos) {
    assert(_t);
    _t->setPos(vno, 0, pos);
}

EnoType TriManipulator::headVno(EnoType eno) { return _t->headVno(eno); }
EnoType TriManipulator::tailVno(EnoType eno) { return _t->tailVno(eno); }
VnoType TriManipulator::noVtx() { return _t->noVtx(); }

bool TriManipulator::toggleEdge(EnoType eno) {
    typedef FaceManipulatorTp<TagMeshTp<Tri>> FaceManiType;
    FaceManiType faceMani(_t);
    return faceMani.toggleEdge(eno);
}

bool TriManipulator::toggleVertex(EnoType eno) {
    typedef FaceManipulatorTp<TagMeshTp<Tri>> FaceManiType;
    FaceManiType faceMani(_t);
    return faceMani.toggleVertex(eno);
}

bool TriManipulator::toggleSector(EnoType eno) {
    typedef FaceManipulatorTp<TagMeshTp<Tri>> FaceManiType;
    FaceManiType faceMani(_t);
    return faceMani.toggleSector(eno);
}

void TriManipulator::recomputeNeighbors() {
    typedef FaceManipulatorTp<TagMeshTp<Tri>> FaceManiType;
    FaceManiType faceMani(_t);
    faceMani.recomputeNeighbors();
}
