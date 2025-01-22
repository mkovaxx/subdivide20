// -*- Mode: c++ -*-
// $Id: pickabletri.cpp,v 1.3 2000/04/30 03:32:38 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/viewer/pickabletri.cpp,v $

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

#include "pickabletri.hpp"

#include "pickmeshobject.hpp"
#include "subtri.hpp"
#include "tagmesh.hpp"

PickableTri::PickableTri() {
    _sobject = new PickMeshObjectType();
    delete _triMesh._triTagMesh;                 // remove the mesh
    _triMesh._triTagMesh = &_sobject->getMesh(); // and connect it with the mesh from the object
}

PickableTri::~PickableTri() {
    delete _sobject;          // here we already remove the mesh
    _triMesh._triTagMesh = 0; // we set this to 0 to avoid a second destruction of the mesh
}

const TriMesh& PickableTri::getMesh() const { return _triMesh; }

TriMesh& PickableTri::getMesh() { return _triMesh; }

void PickableTri::setMesh(const TriMesh& mesh) { _sobject->setMesh(*(mesh._triTagMesh)); }

void PickableTri::rerender() { _sobject->rerender(); }

void PickableTri::render() { _sobject->render(); }

void PickableTri::renderPick(unsigned char picks, unsigned int targetCnt, unsigned int shift) {
    _sobject->renderPick(picks, targetCnt, shift);
}

PickedStuff* PickableTri::doPick(unsigned char r, unsigned char g, unsigned char b, unsigned char picks,
                                 unsigned int shift) {
    return _sobject->doPick(r, g, b, picks, shift);
}

cvec3f PickableTri::minPoint() const { return _sobject->minPoint(); }

cvec3f PickableTri::maxPoint() const { return _sobject->maxPoint(); }

unsigned char& PickableTri::tlRenderMode() { return _sobject->tlRenderMode(); }
