// -*- Mode: c++ -*-
// $Id: ivgraph.hpp,v 1.4 2000/04/30 07:35:46 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/ivgraph.hpp,v $

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

#ifndef __IVGRAPH_H__
#define __IVGRAPH_H__

#include "compat.hpp"
#include <vector>

//: Inventor graph conversion
//
// this class converts between flat meshes and inventor files. It
// doesn't use the inventor library, but QvLib which is a shareware
// parser for VRML 1.0. We've slightly modified the qvlib library to
// fit our purpose.
//

class FlatMesh;
class QvSeparator;
class QvNode;
class Vertex;

class IvGraph {
  public:
    IvGraph();
    virtual ~IvGraph();

    bool read(char* fileName);
    void write(char* fileName);

    bool toFlatMesh(FlatMesh* im, bool split);
    void fromFlatMesh(FlatMesh* flatMesh);

  protected:
    void clear();
    QvSeparator* getRoot() { return root; }
    void setRoot(QvSeparator* r) { root = r; }

  private:
    QvSeparator* root;
    QvNode* createCoordinate3Node(const std::vector<Vertex*>& v, int d);
    QvNode* createIndexedFaceSet(FlatMesh* flatMesh);
};

#endif /* __IVGRAPH_H__ */
