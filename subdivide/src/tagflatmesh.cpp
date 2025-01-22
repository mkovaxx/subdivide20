// -*- Mode: c++ -*-
// $Id: tagflatmesh.cpp,v 1.5 2000/04/29 09:27:19 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/src/tagflatmesh.cpp,v $

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

#include "tagflatmesh.hpp"
#include "sectorinfo.hpp"
#include "vertex.hpp"

TagFlatMesh::TagFlatMesh(const TagFlatMesh& fm) {
    uint i;
    for (i = 0; i < fm.vert_v.size(); ++i) {
        Vertex::ref(fm.vert_v[i]);
    }
    for (i = 0; i < fm.sectorInfoVec.size(); ++i) {
        SectorInfo::ref(fm.sectorInfoVec[i].second);
    }

    Cleanup();

    vert_v = fm.vert_v;
    index_v = fm.index_v;
    poly_v = fm.poly_v;

    creaseEdgeVec = fm.creaseEdgeVec;
    creaseVertVec = fm.creaseVertVec;
    dartVertVec = fm.dartVertVec;
    cornerVertVec = fm.cornerVertVec;
}

TagFlatMesh& TagFlatMesh::operator=(const TagFlatMesh& fm) {
    if (this != &fm) {
        uint i;
        for (i = 0; i < fm.vert_v.size(); ++i) {
            Vertex::ref(fm.vert_v[i]);
        }
        for (i = 0; i < fm.sectorInfoVec.size(); ++i) {
            SectorInfo::ref(fm.sectorInfoVec[i].second);
        }
        Cleanup();
        vert_v = fm.vert_v;
        index_v = fm.index_v;
        poly_v = fm.poly_v;

        creaseEdgeVec = fm.creaseEdgeVec;
        creaseVertVec = fm.creaseVertVec;
        dartVertVec = fm.dartVertVec;
        cornerVertVec = fm.cornerVertVec;
    }
    return *this;
}

void TagFlatMesh::Cleanup() {
    std::vector<Vertex*>::iterator vi;

    for (vi = vert_v.begin(); vi != vert_v.end(); ++vi) {
        Vertex::unref(*vi);
    }

    for (uint i = 0; i < sectorInfoVec.size(); ++i) {
        SectorInfo::unref(sectorInfoVec[i].second);
    }

    vert_v.erase(vert_v.begin(), vert_v.end());
    poly_v.erase(poly_v.begin(), poly_v.end());
    index_v.erase(index_v.begin(), index_v.end());
    triindex_v.erase(triindex_v.begin(), triindex_v.end());

    creaseEdgeVec.clear();
    creaseVertVec.clear();
    cornerVertVec.clear();
    dartVertVec.clear();
    sectorInfoVec.clear();
}
