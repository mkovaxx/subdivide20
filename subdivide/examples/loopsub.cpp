// -*- Mode: c++ -*-
// $Id: loopsub.cpp,v 1.10 2001/02/07 05:29:39 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/examples/loopsub.cpp,v $

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

#include "compat.h"
#include <stdlib.h>

#include "general.h"
#include "tagivgraph.h"
#include "tagflatmesh.h"
#include "trimesh.h"

int main(int argc, char** argv) {
  if(argc != 4) {
    cerr<<argv[0]<<" usage: in.wrl out.wrl depth"<<endl;
  } else {

    // read the wrl file
    TagIvGraph tagIvGraph;
    tagIvGraph.read(argv[1]);

    // convert from wrl to flat mesh
    // second parameter (true) indicates to triangulate all faces
    TagFlatMesh tagFlatMesh;
    tagIvGraph.toTagFlatMesh(&tagFlatMesh, true);

    // convert from flat mesh to tri mesh
    TriMesh triMesh(tagFlatMesh);

    // subdivide the tri mesh a number of times
    int d = atoi(argv[3]);
    d = min(max(0, d), GEN_MAX_DEPTH);
    triMesh.subdivide(d);

    // extract the mesh formed by the leaves
    TriMesh* leafMesh = triMesh.leafMesh();

    // convert this new mesh to a tag flat mesh
    leafMesh->toTagFlatMesh(&tagFlatMesh);

    // build an wrl graph and write it
    tagIvGraph.fromTagFlatMesh(&tagFlatMesh);
    tagIvGraph.write(argv[2]);
  }
  return 0;
}
