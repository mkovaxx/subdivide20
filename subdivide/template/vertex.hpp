// -*- Mode: c++ -*-
// $Id: vertex.hpp,v 1.1 2000/04/29 07:50:09 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/vertex.hpp,v $

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

#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "basevertex.hpp"
#include "compat.hpp"

class TagTri;
class TLTagTri;

// Add reference counting to the vertex class

class Vertex : public BaseVertex {
    //  friend class TagTri;
    // friend class TLTagTri;
  public:
    Vertex(int d = 0) : BaseVertex(d), _refCount(0), _special(false) {}
    virtual ~Vertex() {}

    Vertex* clone(int d) const {
        Vertex* v = new Vertex();
        v->_special = _special;
        v->copyValue(*this, d);
        return v;
    }

    Vertex* clone() const {
        Vertex* v = new Vertex();
        v->_special = _special;
        v->copyValue(*this);
        return v;
    }

    bool isSpecial() const { return _special; }
    void makeSpecial() { _special = true; }
    static void ref(Vertex* vert) {
        if (vert) {
            ++vert->_refCount;
        }
    }
    static void unref(Vertex* vert) {
        if (vert) {
            if ((0 == (--vert->_refCount))) {
                delete vert;
            }
        }
    }

  private:
    unsigned long _refCount;
    bool _special;
};

#endif /* __VERTEX_H__ */
