// -*- Mode: c++ -*-
// $Id: trianglering.hpp,v 1.1 2000/05/23 01:36:21 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/trianglering.hpp,v $
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

#ifndef __TRIANGLERING_H__
#define __TRIANGLERING_H__

#include "compat.hpp"
#include "triangle.hpp"

class Tri;
template <class Face> class FaceRingTp;

class TriangleRing {
  public:
    TriangleRing();
    TriangleRing(const TriangleRing& tr);
    ~TriangleRing();
    TriangleRing& operator=(const TriangleRing& tr);

    // collect the entire ring
    void collectRing(Triangle tr, EnoType eno);
    // collect the part of the ring between two crease edges
    void collectSector(Triangle tr, EnoType eno);

    Triangle triangle(uint i, EnoType& eno) const;

    // ring closed?
    bool isClosed() const;

    // number of faces
    uint noFace() const;

    // number of vertices
    uint noVtx() const;

    // edge index: where in the ring is the triangle we collected from?
    int edgeIndex() const;

    // triangle and edge to access the center of the ring
    Triangle centerEdge(EnoType& e) const;

    // triangle and vertex to access the center of the ring
    Triangle centerVertex(VnoType& vno) const;

    // first vertex in the ring
    Triangle startVertex(VnoType& vno) const;

    // last vertex in the ring
    Triangle endVertex(VnoType& vno) const;

  private:
    typedef FaceRingTp<Tri> TriRingType;
    TriRingType* _tr;
};

#endif /* __TRIANGLERING_H__ */
