// -*- Mode: c++ -*-
// $Id: geoface.hpp,v 1.2 2000/04/29 07:50:09 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/geoface.hpp,v $

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

#ifndef __GEOFACE_H__
#define __GEOFACE_H__

#include "compat.hpp"
#include "cvec3t.hpp"

// Vertex accessors for faces

template <class Face> class GeoFaceTp : public Face {
  public:
    virtual ~GeoFaceTp() { ; }
    const cvec3f& pos(VnoType v, int d) const { return this->vert(v)->getPos(d); }

    const cvec3f& midPos(EnoType e, int d) const { return this->midVert(e)->getPos(d + 1); }

    const cvec3f& tailPos(EnoType e, int d) const { return this->tailVert(e)->getPos(d); }

    const cvec3f& headPos(EnoType e, int d) const { return this->headVert(e)->getPos(d); }

    void setPos(VnoType v, int d, const cvec3f& p) {
        this->vert(v)->setPos(d, p);
        this->vert(v)->set(d);
    }

    void setMidPos(EnoType e, int d, const cvec3f& p) {
        this->midVert(e)->setPos(d + 1, p);
        assert(d + 1 > this->midVert(e)->currentDepth());
        this->midVert(e)->set(d + 1);
    }

    void setTailPos(EnoType e, int d, const cvec3f& p) {
        this->tailVert(e)->setPos(d, p);
        assert(d > this->tailVert(e)->currentDepth());
        this->tailVert(e)->set(d);
    }

    void setHeadPos(EnoType e, int d, const cvec3f& p) {
        this->headVert(e)->setPos(d, p);
        assert(d > this->headVert(e)->currentDepth());
        this->headVert(e)->set(d);
    }

    bool hasPos(VnoType v, int d) const { return this->vert(v)->isSet(d); }

    bool hasHeadPos(EnoType e, int d) const { return this->headVert(e)->isSet(d); }

    bool hasTailPos(EnoType e, int d) const { return this->tailVert(e)->isSet(d); }

    bool hasMidPos(EnoType e, int d) const { return this->midVert(e)->isSet(d + 1); }
};

#endif /* __GEOFACE_H__ */
