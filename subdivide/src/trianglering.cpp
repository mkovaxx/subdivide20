// -*- Mode: c++ -*-
// $Id: trianglering.cpp,v 1.1 2000/05/23 01:36:31 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/src/trianglering.cpp,v $
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

#include "trianglering.hpp"
#include "facering.hpp"
#include "subtri.hpp"

TriangleRing::TriangleRing() { _tr = new TriRingType(); }

TriangleRing::TriangleRing(const TriangleRing& tr) { _tr = new TriRingType(*(tr._tr)); }

TriangleRing::~TriangleRing() { delete _tr; }

void TriangleRing::collectRing(Triangle t, EnoType eno) { _tr->collectRing(t._t, eno); }

void TriangleRing::collectSector(Triangle t, EnoType eno) { _tr->collectSector(t._t, eno); }

Triangle TriangleRing::triangle(uint i, EnoType& eno) const { return Triangle(_tr->face(i, eno)); }

bool TriangleRing::isClosed() const { return _tr->isClosed(); }

uint TriangleRing::noFace() const { return _tr->noFace(); }

uint TriangleRing::noVtx() const { return _tr->noVtx(); }

int TriangleRing::edgeIndex() const { return _tr->edgeIndex(); }

Triangle TriangleRing::centerEdge(EnoType& e) const { return Triangle(_tr->centerFace(e)); }

Triangle TriangleRing::centerVertex(VnoType& vno) const { return Triangle(_tr->centerVertex(vno)); }

Triangle TriangleRing::startVertex(VnoType& vno) const { return Triangle(_tr->startVertex(vno)); }

Triangle TriangleRing::endVertex(VnoType& vno) const { return Triangle(_tr->endVertex(vno)); }
