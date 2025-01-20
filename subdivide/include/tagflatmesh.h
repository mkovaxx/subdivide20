// -*- Mode: c++ -*-
// $Id: tagflatmesh.h,v 1.2 2000/04/29 12:29:27 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/tagflatmesh.h,v $

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

#ifndef __TAGFLATMESH_H__
#define __TAGFLATMESH_H__

#include "compat.h"
#include "flatmesh.h"
#include "ipoly.h"
#include <vector>

class Vert;
class SectorInfo;

class TagFlatMesh : public FlatMesh {
  public:
    typedef pair<pair<int, int>, SectorInfo*> FlatSectorType;
    vector<FlatSectorType> sectorInfoVec;

    vector<int> creaseEdgeVec;
    vector<int> dartVertVec;
    vector<int> creaseVertVec;
    vector<int> cornerVertVec;

    TagFlatMesh() { ; }
    TagFlatMesh(const TagFlatMesh& fm);
    TagFlatMesh& operator=(const TagFlatMesh& fm);
    virtual void Cleanup();
    virtual ~TagFlatMesh() { Cleanup(); }
};

#endif /* __TAGFLATMESH_H__ */
