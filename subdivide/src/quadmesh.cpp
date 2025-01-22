// -*- Mode: c++ -*-
// $Id: quadmesh.cpp,v 1.11 2000/04/30 07:35:47 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/src/quadmesh.cpp,v $

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

#include "quadmesh.hpp"
#include "subquad.hpp"
#include "tagmesh.hpp"

QuadMesh::QuadMesh() { _quadTagMesh = new TagMeshTp<Quad>(); }

QuadMesh::~QuadMesh() {
    if (_quadTagMesh) {
        delete _quadTagMesh;
    }
}

QuadMesh::QuadMesh(const QuadMesh& m) {
    _quadTagMesh = new TagMeshTp<Quad>();
    _quadTagMesh->operator=(*(m._quadTagMesh));
}

QuadMesh::QuadMesh(const TagFlatMesh& flatMesh) { _quadTagMesh = new TagMeshTp<Quad>(flatMesh); }

QuadMesh& QuadMesh::operator=(const QuadMesh& m) {
    _quadTagMesh->operator=(*(m._quadTagMesh));
    return (*this);
}

QuadMesh* QuadMesh::clone() const {
    QuadMesh* nm = new QuadMesh();
    nm->setClone(*this);
    return nm;
}

void QuadMesh::setClone(const QuadMesh& m) { _quadTagMesh->setClone(*(m._quadTagMesh)); }

int QuadMesh::meshDepth() const { return _quadTagMesh->meshDepth(); }

void QuadMesh::toTagFlatMesh(TagFlatMesh* flatMesh) const { _quadTagMesh->toTagFlatMesh(flatMesh); }

void QuadMesh::subdivide(int maxl) { _quadTagMesh->subdivide(maxl); }

void QuadMesh::midsub(int maxl) { _quadTagMesh->midsub(maxl); }

QuadMesh* QuadMesh::leafMesh() const {

    TagMeshTp<Quad>* tm = new TagMeshTp<Quad>();

    TagMeshTp<Quad>::FaceIterType fi;
    for (fi = _quadTagMesh->faceBegin(); fi != _quadTagMesh->faceEnd(); ++fi) {
        if ((*fi)->isLeaf()) {
            tm->insertFace(*fi);
        }
    }

    QuadMesh* m = new QuadMesh();
    m->_quadTagMesh = tm;
    return m;
}

int QuadMesh::numberOfVertices() const { return _quadTagMesh->vertVector().size(); }

const cvec3f& QuadMesh::getVertexPos(uint index) const {
    assert(index < _quadTagMesh->vertVector().size());
    return _quadTagMesh->vertVector()[index]->getPos(0);
}

void QuadMesh::setVertexPos(uint index, const cvec3f& p) {
    assert(index < _quadTagMesh->vertVector().size());
    _quadTagMesh->vertVector()[index]->setPos(0, p);
    _quadTagMesh->vertVector()[index]->set(0);
}
