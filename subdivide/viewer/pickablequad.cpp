// -*- Mode: c++ -*-
// $Id: pickablequad.cpp,v 1.3 2000/04/30 03:32:38 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/viewer/pickablequad.cpp,v $

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

#include "pickablequad.h"

#include "pickmeshobject.h"
#include "subquad.h"
#include "tagmesh.h"

PickableQuad::PickableQuad() {
    _sobject = new PickMeshObjectType();
    delete _quadMesh._quadTagMesh;                 // remove the mesh
    _quadMesh._quadTagMesh = &_sobject->getMesh(); // and connect it with the mesh from the object
}

PickableQuad::~PickableQuad() {
    delete _sobject;            // here we already remove the mesh
    _quadMesh._quadTagMesh = 0; // we set this to 0 to avoid a second destruction of the mesh
}

const QuadMesh& PickableQuad::getMesh() const { return _quadMesh; }

QuadMesh& PickableQuad::getMesh() { return _quadMesh; }

void PickableQuad::setMesh(const QuadMesh& mesh) { _sobject->setMesh(*(mesh._quadTagMesh)); }

void PickableQuad::rerender() { _sobject->rerender(); }

void PickableQuad::render() { _sobject->render(); }

void PickableQuad::renderPick(unsigned char picks, unsigned int targetCnt, unsigned int shift) {
    _sobject->renderPick(picks, targetCnt, shift);
}

PickedStuff* PickableQuad::doPick(unsigned char r, unsigned char g, unsigned char b, unsigned char picks,
                                  unsigned int shift) {
    return _sobject->doPick(r, g, b, picks, shift);
}

cvec3f PickableQuad::minPoint() const { return _sobject->minPoint(); }

cvec3f PickableQuad::maxPoint() const { return _sobject->maxPoint(); }

unsigned char& PickableQuad::tlRenderMode() { return _sobject->tlRenderMode(); }
