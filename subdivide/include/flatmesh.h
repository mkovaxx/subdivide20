// -*- Mode: c++ -*-
// $Id: flatmesh.h,v 1.3 2000/04/29 12:29:26 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/flatmesh.h,v $

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

#ifndef __FLATMESH_H__
#define __FLATMESH_H__

#include "compat.h"
#include "ipoly.h"
#include <vector>

//: Generic flat mesh representation
//
// The mesh is stored as an array of vertices and an array of
// polygons, each represented as an array of vertex numbers. This
// class is mainly used as an intermediate convertable mesh
// representation. We want to be able to convert to and from inventor
// graphs and the mesh class.

class Vertex;

class FlatMesh {
  public:
    //: array of vertices
    std::vector<Vertex*> vert_v;
    //: array of indices of vertices;
    std::vector<uint> index_v;

    //: we assume a uniform depth for the mesh. when we write the mesh,
    // we will access the vertex position according to the vertex_depth
    int vertex_depth;

    //: each polygon corresponds to a subarray of indices
    // each poly is represented by a pair

    // (index into index_v, # of vertices)
    std::vector<IPoly> poly_v; // array of polygons;

    //: sometimes we have to triangulate the mesh, however, we want to be able to
    //: find for each face of the original mesh a corresponding triangle
    //: triindex stores the indices of the first triangle corresponding to a face
    std::vector<int> triindex_v;

    FlatMesh() { ; }
    FlatMesh(const FlatMesh& fm);
    FlatMesh& operator=(const FlatMesh& fm);

    //: erase the flat mesh

    // we erase the flat mesh by erasing each vector individually.  this
    // function is virtual since we might have to destroy more objects
    // in derived classes
    virtual void Cleanup();
    virtual ~FlatMesh() { Cleanup(); }
};

#endif /* __FLATMESH_H__ */
