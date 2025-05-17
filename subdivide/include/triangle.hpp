// -*- Mode: c++ -*-
// $Id: triangle.hpp,v 1.1 2000/05/23 01:36:21 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/triangle.hpp,v $
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

#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

// this class is a wrapper class around triangles, it contains
// acessors to positions, normals, tags.  Also, it has acessors to
// neighbors and children.

#include "compat.hpp"
#include "cvec3t.hpp"
#include "general.hpp"
#include "sectorinfo.hpp"

// Forward declarations
class Tri;
class TriangleIter;
class TriangleRing;
template <class Face> class FaceIterTp;

// The wrapper for a tri.  This is basically a reference class.  We
// don't do reference counting on triangles as they are taken care for
// in the mesh class.  (Only toplevel triangles are counted...)
class Triangle {
    friend class TriangleIter;
    friend class TriangleRing;

  public:
    typedef enum { NOTAG_VERTEX, CORNER_VERTEX, CREASE_VERTEX } VertexTagType;
    typedef enum { NOTAG_EDGE, CREASE_EDGE } EdgeTagType;

    Triangle() : _t(0) { ; }
    Triangle(const Triangle& tri) { _t = tri._t; }
    Triangle& operator=(const Triangle& tri) {
        _t = tri._t;
        return *this;
    }
    bool operator==(const Triangle& tri) const { return _t == tri._t; }
    bool operator!=(const Triangle& tri) const { return _t != tri._t; }
    bool operator>(const Triangle& tri) const { return _t > tri._t; }
    bool operator<(const Triangle& tri) const { return _t < tri._t; }
    bool isNull() const { return _t == 0; }

    // numbering
    VnoType noVtx() const;              // number of vertices
    VnoType headVno(EnoType eno) const; // head vertex number of eno
    VnoType tailVno(EnoType eno) const; // tail vertex number of eno
    VnoType prevVno(VnoType vno) const;
    VnoType nextVno(VnoType vno) const;
    EnoType prevEno(EnoType eno) const;
    EnoType nextEno(EnoType eno) const;
    EnoType reverseEno(EnoType eno) const;

    // orientation
    OrientationType orientation() const;                         // returns whether vertices in CCW order or in CW order
    OrientationType edgeDir(EnoType e) const;                    // edge in CCW or CW direction
    EnoType directEno(EnoType e, OrientationType o = CCW) const; // orient edge to be in or CCW/CW direction

    // tags
    VertexTagType vertexTag(VnoType vno) const;
    EdgeTagType edgeTag(EnoType eno) const;
    SectorInfo* sectorInfo(VnoType vno) const;
    void setVertexTag(VnoType vno, VertexTagType vertexTag);
    void setEdgeTag(EnoType eno, EdgeTagType edgeTag);
    void setSectorInfo(VnoType vno, SectorInfo* sectorInfo);

    // positions
    void setPos(VnoType vno, const cvec3f& pos, int depth = 0);
    const cvec3f& vtxPos(VnoType vno, int depth) const;
    const cvec3f& headPos(EnoType eno, int depth) const;
    const cvec3f& tailPos(EnoType eno, int depth) const;
    const cvec3f& midPos(EnoType eno, int depth) const;
    const cvec3f& limitPos(VnoType vno) const;
    const cvec3f& normalDir(VnoType vno) const;

    // neighbor, children and parent
    Triangle neighbor(EnoType e, EnoType& ne) const;
    CnoType childCount() const;
    Triangle child(CnoType c) const;
    Triangle parent() const;
    bool isLeaf() const;
    bool isToplevel() const;

    // update neighborhood
    void recomputeNeighbors();

  private:
    Triangle(Tri* t) : _t(t) { ; }
    Tri* _t;
};

// The TriangleIter class provides iteration over triangles in a mesh
class TriangleIter {
    friend class TriMesh;

  public:
    TriangleIter();
    ~TriangleIter();
    TriangleIter(const TriangleIter& i);
    TriangleIter& operator=(const TriangleIter& i);

    Triangle operator*();
    TriangleIter& operator++();
    bool operator==(const TriangleIter& i) const;
    bool operator!=(const TriangleIter& i) const;

    int depth() const;
    int maxDepth() const;

  private:
    typedef FaceIterTp<Tri> TriIter;
    TriIter* _ti;
    TriangleIter(TriIter _ti);
};

#endif /* __TRIANGLE_H__ */
