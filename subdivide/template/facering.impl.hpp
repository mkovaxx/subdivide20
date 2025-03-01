// -*- Mode: c++ -*-
// $Id: facering.impl.hpp,v 1.2 2000/04/29 07:50:08 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/facering.impl.hpp,v $

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

template <class F> void FaceRingTp<F>::collectRing(F* t, EnoType eno) {

    _enoVec.erase(_enoVec.begin(), _enoVec.end());
    _faceVec.erase(_faceVec.begin(), _faceVec.end());

    _face = t;
    _eno = eno;
    _edge = 0;
    _closed = true;

    EnoType currentEno = -eno;
    F* currentTri = t;

    do {
        // make sure that the tail of the current edge is the center
        assert(currentTri->tailVert(currentEno) == _face->headVert(_eno));
        _enoVec.push_back(currentEno);
        _faceVec.push_back(currentTri);

        // get the name of the current edge from the point of view
        // of the adjacent polygon, and get the other edge of the
        // adjacent polygon which has _center as an endpoint;
        // the direction is chosen so that center is the tail of the other edge

        currentTri = currentTri->neighbor(currentTri->nextEno(-currentEno), currentEno);
        // until we run into the boundary or return to the initial vertex
    } while (currentTri && currentTri != _face);

    // if we hit the boundary, return to the original edge
    // and go in the opppsite direction;
    // as we go, update the edge number edge(), so that it
    // always points to the initial edge

    if (!currentTri) {
        _closed = false;
        currentTri = _face;
        currentEno = -_eno;
        assert(currentTri->tailVert(currentEno) == _face->headVert(_eno));
        do {
            currentTri = currentTri->neighbor(currentEno, currentEno);
            if (currentTri) {
                currentEno = currentTri->nextEno(-currentEno);
                assert(currentTri->tailVert(currentEno) == _face->headVert(_eno));
                _enoVec.push_front(currentEno);
                _faceVec.push_front(currentTri);
                _edge++;
            }
        } while (currentTri);
    }
}

template <class F> void FaceRingTp<F>::collectSector(F* t, EnoType eno) {

    //    cerr<<"collectSector!"<<endl;

    _enoVec.erase(_enoVec.begin(), _enoVec.end());
    _faceVec.erase(_faceVec.begin(), _faceVec.end());

    _face = t;
    _eno = eno;
    _edge = 0;
    _closed = true;

    EnoType currentEno = -eno;
    F* currentTri = t;

    do {
        // make sure that the tail of the current edge is the center
        assert(currentTri->tailVert(currentEno) == _face->headVert(_eno));
        _enoVec.push_back(currentEno);
        _faceVec.push_back(currentTri);

        // get the name of the current edge from the point of view
        // of the adjacent polygon, and get the other edge of the
        // adjacent polygon which has _center as an endpoint;
        // the direction is chosen so that center is the tail of the other edge

        currentTri = currentTri->neighbor(currentTri->nextEno(-currentEno), currentEno);
        // until we run into the boundary or return to the initial vertex
    } while (currentTri && (currentTri != _face) && (currentTri->edgeTag(currentEno) == F::NOTAG_EDGE));

    // if we hit the boundary, return to the original edge
    // and go in the opppsite direction;
    // as we go, update the edge number edge(), so that it
    // always points to the initial edge

    if ((!currentTri) || (currentTri->edgeTag(currentEno) != F::NOTAG_EDGE)) {
        _closed = false;
        currentTri = _face;
        currentEno = -_eno;
        assert(currentTri->tailVert(currentEno) == _face->headVert(_eno));
        do {
            currentTri = currentTri->neighbor(currentEno, currentEno);
            if (currentTri && (currentTri->edgeTag(currentEno) == F::NOTAG_EDGE)) {
                currentEno = currentTri->nextEno(-currentEno);
                assert(currentTri->tailVert(currentEno) == _face->headVert(_eno));
                _enoVec.push_front(currentEno);
                _faceVec.push_front(currentTri);
                _edge++;
            }
        } while (currentTri && (currentTri->edgeTag(currentEno) == F::NOTAG_EDGE));
    }
}
