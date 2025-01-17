// -*- Mode: c++ -*-
// $Id: flatmesh.cpp,v 1.3 2000/04/29 07:50:07 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/src/flatmesh.cpp,v $

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

#include "flatmesh.h"
#include "vertex.h"

FlatMesh::FlatMesh(const FlatMesh& fm) {
  for(uint i = 0; i < fm.vert_v.size(); ++i)
    Vertex::ref(fm.vert_v[i]);
  Cleanup();
  vert_v = fm.vert_v;
  index_v = fm.index_v;
  poly_v = fm.poly_v;
}

FlatMesh& FlatMesh::operator=(const FlatMesh& fm) {
  if(this != &fm) {
    for(uint i = 0; i < fm.vert_v.size(); ++i) {
      Vertex::ref(fm.vert_v[i]);
    }
    Cleanup();
    vert_v = fm.vert_v;
    index_v = fm.index_v;
    poly_v = fm.poly_v;
  }
  return *this;
}

void FlatMesh::Cleanup() {
  vector<Vertex* >::iterator vi;
  for( vi = vert_v.begin(); vi != vert_v.end(); ++vi ) 
    Vertex::unref(*vi);

  vert_v.erase(vert_v.begin(), vert_v.end());
  poly_v.erase(poly_v.begin(), poly_v.end());
  index_v.erase(index_v.begin(), index_v.end());
  triindex_v.erase(triindex_v.begin(), triindex_v.end());
}


