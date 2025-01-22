// -*- Mode: c++ -*-
// $Id: basevertex.hpp,v 1.1 2000/04/29 07:50:08 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/basevertex.hpp,v $

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

#ifndef __BASEVERTEX_H__
#define __BASEVERTEX_H__

#include "compat.hpp"
#include "cvec3t.hpp"
#include "general.hpp"

// class for vertices;
// stores vertex positions at all subdivision levels,
// the limit position, and the normals (possibly two for creases)
// GEN_MAX_DEPTH is a global constant defining the maximal subdivision
// depth

class BaseVertex {
  public:
    BaseVertex(int d = 0) : _creationDepth(d), _currentDepth(0) { _pos = new cvec3f[GEN_MAX_DEPTH - d]; }

    BaseVertex(cvec3f v, int d = 0) : _creationDepth(d), _currentDepth(0) {
        _pos = new cvec3f[GEN_MAX_DEPTH - d];
        _pos[0] = v;
    }

    BaseVertex(const BaseVertex&) { die(); }
    BaseVertex& operator=(const BaseVertex&) {
        die();
        return *this;
    }

    virtual ~BaseVertex() { delete _pos; }
    void copyValue(const BaseVertex& v) { setPos(v.getPos()); }
    void copyValue(const BaseVertex& v, int d) { setPos(v.getPos(d)); }

    void setPos(int d, const cvec3f& pos) {
        assert(d >= _creationDepth);
        _pos[d - _creationDepth] = pos;
    }
    const cvec3f& getPos(int d) const {
        assert(d >= _creationDepth);
        return _pos[d - _creationDepth];
    }
    void setPos(const cvec3f& pos) { _pos[0] = pos; }
    const cvec3f& getPos() const { return _pos[0]; }
    void setLimitPos(const cvec3f& pos) { _limitPos = pos; }
    const cvec3f& limitPos() { return _limitPos; }

    const cvec3f& normal0() const { return _normal0; }
    const cvec3f& normal1() const { return _normal1; }
    void setNormal0(const cvec3f& n) { _normal0 = n; }
    void setNormal1(const cvec3f& n) { _normal1 = n; }

    int creationDepth() { return _creationDepth; }

    bool isSet(uint d) const { return d <= _currentDepth; }

    void set(int d) { _currentDepth = d; }

    void unset() { _currentDepth = 0; }

    int currentDepth() const { return _currentDepth; }

  private:
    int _creationDepth;         // the depth at which the vertex appeared
    unsigned int _currentDepth; // number of computed subdivision steps
    cvec3f _limitPos;           // corresponding point on the limit surface
    cvec3f* _pos;               // vector of positions
    cvec3f _normal0, _normal1;  // possibly two normals (second one used for
                                // creases only)
};

#endif /* __BASEVERTEX_H__ */
