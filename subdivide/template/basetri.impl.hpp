// -*- Mode: c++ -*-
// $Id: basetri.impl.hpp,v 1.5 2000/04/30 07:35:48 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/basetri.impl.hpp,v $

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

template <class BFace> void BaseTriTp<BFace>::makeChildren(int d) {
    this->_c = new FaceType[4];

    for (CnoType c = 0; c < childCount(); ++c) {
        this->_c[c].nox() = c;
        this->_c[c].parentx() = (FaceType*)this;
        this->_c[c].orient() = (this->_orient == CCW) ? CW : CCW;
    }
    this->_c[0].orient() = this->_orient;

    EnoType se;
    FaceType* st;

    // inherit old vertices
    EnoType e;
    for (e = 1; e < noVtx() + 1; ++e) {
        Vertex* midV = 0;
        EnoType ne;
        FaceType* nt = neighbor(e, ne);
        if (nt && !nt->isLeaf()) {
            midV = nt->midVert(ne);
        }
        if (midV == 0) {
            midV = new Vertex(d + 1);
        }

        assert(((FaceType*)this)->headVert(e));
        assert(midV);
        assert(((FaceType*)this)->tailVert(e));

        this->_c[0].setVert((e + 1) % 3, midV);

        st = tailSubEdge(e, se);
        st->setVert(st->headVno(se), midV);
        st->setVert(st->tailVno(se), ((FaceType*)this)->tailVert(e));

        st = headSubEdge(e, se);
        st->setVert(st->headVno(se), ((FaceType*)this)->headVert(e));
        st->setVert(st->tailVno(se), midV);
    }
}

template <class T>
const EnoType BaseTriTp<T>::_parentEno[3][7] = {
    {1, 0, 3, 0, -3, 0, -1}, {0, 1, 2, 0, -2, -1, 0}, {2, 3, 0, 0, 0, -3, -2}};

template <class T> const CnoType BaseTriTp<T>::_headSubCno[7] = {3, 2, 1, 0, 2, 3, 1};

template <class T> const EnoType BaseTriTp<T>::_headSubEno[7] = {2, 1, 3, 0, -2, -3, -1};

template <class T> const CnoType BaseTriTp<T>::_tailSubCno[7] = {1, 3, 2, 0, 1, 2, 3};

template <class T> const EnoType BaseTriTp<T>::_tailSubEno[7] = {1, 3, 2, 0, -3, -1, -2};

// 99 is the code for no neighbor (inside the same parent triangle)!
template <class T>
const CnoType BaseTriTp<T>::_neiCno[4][7] = {
    {2, 1, 3, 99, 3, 1, 2},     // triangle 0
    {99, 0, 99, 99, 99, 0, 99}, // triangle 1
    {0, 99, 99, 99, 99, 99, 0}, // triangle 2
    {99, 99, 0, 99, 0, 99, 99}, // triangle 3
};

// 0 is the code for no neighbor (inside the same parent triangle)!
template <class T>
const EnoType BaseTriTp<T>::_neiEno[4][7] = {
    {-3, -2, -1, 0, 1, 2, 3}, // triangle 0
    {0, -2, 0, 0, 0, 2, 0},   // triangle 1
    {-3, 0, 0, 0, 0, 0, 3},   // triangle 2
    {0, 0, -1, 0, 1, 0, 0}    // triangle 3
};
