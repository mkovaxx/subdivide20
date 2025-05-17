// -*- Mode: c++ -*-
// $Id: triangle.cpp,v 1.2 2001/02/07 05:29:40 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/src/triangle.cpp,v $
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

#include "compat.hpp"
#include <set>

#include "subtri.hpp"
#include "tagmesh.hpp"
#include "triangle.hpp"

// numbering
VnoType Triangle::noVtx() const {
    assert(_t);
    return _t->noVtx();
}

VnoType Triangle::headVno(EnoType eno) const {
    assert(_t);
    return _t->headVno(eno);
}

VnoType Triangle::tailVno(EnoType eno) const {
    assert(_t);
    return _t->tailVno(eno);
}

VnoType Triangle::prevVno(VnoType vno) const {
    assert(_t);
    return _t->prevVno(vno);
}

VnoType Triangle::nextVno(VnoType vno) const {
    assert(_t);
    return _t->nextVno(vno);
}

EnoType Triangle::prevEno(EnoType eno) const {
    assert(_t);
    return _t->prevEno(eno);
}

EnoType Triangle::nextEno(EnoType eno) const {
    assert(_t);
    return _t->nextEno(eno);
}

EnoType Triangle::reverseEno(EnoType eno) const {
    assert(_t);
    return _t->reverseEno(eno);
}

// orientation
OrientationType Triangle::orientation() const {
    assert(_t);
    return _t->orientation();
}

OrientationType Triangle::edgeDir(EnoType e) const {
    assert(_t);
    return _t->edgeDir(e);
}

EnoType Triangle::directEno(EnoType e, OrientationType o) const {
    assert(_t);
    return _t->directEno(e, o);
}

// tags
Triangle::VertexTagType Triangle::vertexTag(VnoType vno) const {
    assert(_t);
    return (Triangle::VertexTagType)_t->vertexTag(vno);
}

Triangle::EdgeTagType Triangle::edgeTag(EnoType eno) const {
    assert(_t);
    return (Triangle::EdgeTagType)_t->edgeTag(eno);
}

SectorInfo* Triangle::sectorInfo(VnoType vno) const {
    assert(_t);
    return _t->sectorInfo(vno);
}

void Triangle::setVertexTag(VnoType vno, VertexTagType vertexTag) {
    assert(_t);
    ((TLTri*)_t)->setVertexTag(vno, (Tri::VertexTagType)vertexTag);
}

void Triangle::setEdgeTag(EnoType eno, EdgeTagType edgeTag) {
    assert(_t);
    ((TLTri*)_t)->setEdgeTag(eno, (Tri::EdgeTagType)edgeTag);
}

void Triangle::setSectorInfo(VnoType vno, SectorInfo* sectorInfo) {
    assert(_t);
    ((TLTri*)_t)->setSectorInfo(vno, sectorInfo);
}

// positions
void Triangle::setPos(VnoType vno, const cvec3f& pos, int depth) {
    assert(_t);
    _t->setPos(vno, depth, pos);
}

const cvec3f& Triangle::vtxPos(VnoType vno, int depth) const {
    assert(_t);
    return _t->pos(vno, depth);
}

const cvec3f& Triangle::headPos(EnoType eno, int depth) const {
    assert(_t);
    return _t->headPos(eno, depth);
}

const cvec3f& Triangle::tailPos(EnoType eno, int depth) const {
    assert(_t);
    return _t->tailPos(eno, depth);
}

const cvec3f& Triangle::limitPos(VnoType vno) const {
    assert(_t);
    return _t->vert(vno)->limitPos();
}

const cvec3f& Triangle::normalDir(VnoType vno) const {
    assert(_t);
    return _t->normal(vno);
}

// neighbor, children and parent
Triangle Triangle::neighbor(EnoType e, EnoType& ne) const {
    assert(_t);
    return Triangle(_t->neighbor(e, ne));
}

CnoType Triangle::childCount() const {
    assert(_t);
    return (_t->childCount());
}

Triangle Triangle::child(CnoType c) const {
    assert(_t);
    return Triangle(_t->child(c));
}

Triangle Triangle::parent() const {
    assert(_t);
    return Triangle(_t->parent());
}

bool Triangle::isLeaf() const {
    assert(_t);
    return _t->isLeaf();
}
bool Triangle::isToplevel() const {
    assert(_t);
    return _t->isToplevel();
}

// update neighborhood
void Triangle::recomputeNeighbors() {
    // three ring face set
    std::set<Tri*> s0;
    std::set<Tri*> s1;

    typedef TagMeshTp<Tri> TriMeshType;
    typedef std::set<Tri*>::iterator TriIterType;
    typedef TriMeshType::FaceRingType TriRingType;

    s0.insert(_t);

    // collect affected triangles
    for (int k = 0; k < 2; ++k) {
        TriIterType it;
        for (it = s0.begin(); it != s0.end(); ++it) {
            Tri* f = (*it);
            for (EnoType e = 1; e < f->noVtx() + 1; ++e) {
                TriRingType fr;
                fr.collectRing(f, e);
                for (uint i = 0; i < fr.noFace(); ++i) {
                    EnoType ringEno;
                    s1.insert(fr.face(i, ringEno));
                }
            }
        }
        s1.swap(s0);
    }

    // build local mesh
    TriMeshType mk;

    TriIterType it;

    for (it = s0.begin(); it != s0.end(); ++it) {
        mk.insertFace((*it));
    }

    // subdivide mesh
    mk.subdivide();
}
