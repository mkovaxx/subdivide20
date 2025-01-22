// -*- Mode: c++ -*-
// $Id: tagivgraph.hpp,v 1.2 2000/04/29 12:29:27 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/tagivgraph.hpp,v $

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

#ifndef __IV_TAGGRAPH_H__
#define __IV_TAGGRAPH_H__

#include "compat.hpp"
#include "ivgraph.hpp"
#include "tagflatmesh.hpp"

//: Inventor graph conversion
//
// this class converts between flat meshes and inventor files. It
// doesn't use the inventor library, but QvLib which is a shareware
// parser for VRML 1.0. We've slightly modified the qvlib library to
// fit our purpose.
//

class TagIvGraph : public IvGraph {
  public:
    TagIvGraph();
    virtual ~TagIvGraph();

    bool toTagFlatMesh(TagFlatMesh* im, bool split);
    void fromTagFlatMesh(TagFlatMesh* tagFlatMesh);

  private:
};

#endif /* __TAGIVGRAPH_H__ */
