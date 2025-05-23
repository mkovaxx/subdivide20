// -*- Mode: c++ -*-
// $Id: basequad.impl.hpp,v 1.5 2000/04/30 07:35:48 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/basequad.impl.hpp,v $

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

template <class BaseFace> void BaseQuadTp<BaseFace>::makeChildren(int d) {
    assert(this->_c == 0);
    this->_c = new Face[noVtx()];

    Vertex* centerV = new Vertex(d + 1);

    for (CnoType c = 0; c < childCount(); ++c) {
        this->_c[c].nox() = c;
        this->_c[c].parentx() = (Face*)this;
        this->_c[c].orient() = this->_orient;
        this->_c[c].v() = new Vertex*[4];
        this->_c[c].v()[0] = this->_c[c].v()[1] = this->_c[c].v()[2] = this->_c[c].v()[3] = 0;
    }

    EnoType se;
    Face* st;

    // inherit old vertices
    EnoType e;
    for (e = 1; e < noVtx() + 1; ++e) {
        Vertex* midV = 0;
        EnoType ne;
        Face* nt = neighbor(e, ne);
        if (nt && !nt->isLeaf()) {
            midV = nt->midVert(ne);
        }
        if (midV == 0) {
            midV = new Vertex(d + 1);
        }

        assert(((Face*)this)->headVert(e));
        assert(midV);
        assert(((Face*)this)->tailVert(e));

        st = tailSubEdge(e, se);

        st->setVert(st->headVno(se), midV);
        st->setVert(st->tailVno(se), ((Face*)this)->tailVert(e));
        st->setVert(st->headVno(st->nextEno(se)), centerV);

        st = headSubEdge(e, se);
        st->setVert(st->headVno(se), ((Face*)this)->headVert(e));
        st->setVert(st->tailVno(se), midV);
    }
}
