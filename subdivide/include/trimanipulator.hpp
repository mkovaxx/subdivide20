// -*- Mode: c++ -*-
// $Id: trimanipulator.hpp,v 1.4 2000/04/30 12:14:20 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/trimanipulator.hpp,v $
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

#ifndef __TRIMANIPULATOR_H__
#define __TRIMANIPULATOR_H__

#include "compat.hpp"
#include "general.hpp"
#include "sectorinfo.hpp"

#include "cvec3t.hpp"

// internal representation
class Tri;

// interface for changing tags for faces of a triangular mesh

class TriManipulator {
  public:
    typedef enum { NOTAG_VERTEX, CORNER_VERTEX, CREASE_VERTEX } VertexTagType;
    typedef enum { NOTAG_EDGE, CREASE_EDGE } EdgeTagType;

    TriManipulator(void* f);

    bool isNull() { return _t == 0; }

    // numbering
    EnoType headVno(EnoType eno); // head vertex number of eno
    EnoType tailVno(EnoType eno); // tail vertex number of eno
    VnoType noVtx();              // number of vertices

    bool isBoundaryEdge(EnoType eno); // edge eno is a boundary edge

    VertexTagType getVertexTag(VnoType vno);
    void setVertexTag(VnoType vno, VertexTagType vertexTag);

    EdgeTagType getEdgeTag(EnoType eno);
    void setEdgeTag(EnoType eno, EdgeTagType edgeTag);

    SectorInfo* getSectorInfo(VnoType vno);
    void setSectorInfo(VnoType vno, SectorInfo* sectorInfo);

    cvec3f getNormalDir(VnoType vno);
    cvec3f getVertexPos(VnoType vno);
    void setVertexPos(VnoType vno, const cvec3f& pos);

    void recomputeNeighbors();
    bool toggleEdge(EnoType eno);
    bool toggleVertex(EnoType eno);
    bool toggleSector(EnoType eno);

  private:
    Tri* _t;
};

#endif /* __TRIMANIPULATOR_H__ */
