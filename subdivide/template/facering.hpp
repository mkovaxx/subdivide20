// -*- Mode: c++ -*-
// $Id: facering.hpp,v 1.3 2000/04/29 07:50:08 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/facering.hpp,v $

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

#ifndef __FACERING_H__
#define __FACERING_H__

#include "compat.hpp"
#include "optdeque.hpp"
#include <vector>

class Vertex;

// A class to store sequences of triangles adjacent to a vertex

template <class Face> class FaceRingTp {
  public:
    typedef Face FaceType;

    FaceRingTp() : _edge(0), _closed(false){};

    FaceRingTp(const FaceRingTp& tr) {
        _edge = tr._edge;
        _face = tr._face;
        _eno = tr._eno;
        _enoVec = tr._enoVec;
        _faceVec = tr._faceVec;
        _closed = tr._closed;
    }

    FaceRingTp* operator=(const FaceRingTp& tr) {
        if (&tr != this) {
            _edge = tr._edge;
            _face = tr._face;
            _eno = tr._eno;
            _enoVec = tr._enoVec;
            _faceVec = tr._faceVec;
            _closed = tr._closed;
        }
        return this;
    }

    // collect the entire vertex ring
    //
    void collectRing(FaceType* face, EnoType edge);

    // collect the part of a vertex ring between two crease edges
    //
    void collectSector(FaceType* face, EnoType edge);

    FaceType* centerFace(EnoType& eno) const {
        eno = _eno;
        return _face;
    }

    FaceType* face(uint i, EnoType& eno) const {
        assert(i < noVtx());
        if (i < _enoVec.size()) {
            eno = _enoVec[i];
            return _faceVec[i];
        } else {
            assert(!_closed);
            eno = -_faceVec[i - 1]->prevEno(_enoVec[i - 1]);
            assert(_faceVec[i - 1]->headVert(eno) != _faceVec[i - 1]->tailVert(_enoVec[i - 1]));
            return _faceVec[i - 1];
        }
    }

    bool isClosed() const { return _closed; }
    uint noFace() const { return _enoVec.size(); }
    uint noVtx() const { return (_closed) ? (_enoVec.size()) : (_enoVec.size() + 1); }

    int edgeIndex() const { return _edge; }
    Vertex* vert(uint i) const {
        assert(i < noVtx());
        if (i < _enoVec.size()) {
            return _faceVec[i]->headVert(_enoVec[i]);
        } else {
            assert(!_closed);
            return _faceVec[i - 1]->tailVert(_faceVec[i - 1]->prevEno(_enoVec[i - 1]));
        }
    }
    Vertex* centerVert() const {
        if (_face == 0) {
            return 0;
        } else {
            return _face->headVert(_eno);
        }
    }
    FaceType* centerEdge(EnoType& e) const;
    FaceType* centerVertex(VnoType& vno) const {
        vno = _face->headVno(_eno);
        return _face;
    }
    FaceType* edgeVertex(int i, VnoType& v) const {
        EnoType e;
        FaceType* f = face(i, e);
        v = f->headVno(e);
        return f;
    }

    Vertex* startVert() {
        VnoType sv;
        FaceType* sf = startVertex(sv);
        return sf->vert(sv);
    }

    Vertex* endVert() {
        VnoType sv;
        FaceType* sf = endVertex(sv);
        return sf->vert(sv);
    }

    FaceType* startVertex(VnoType& vno) const {
        assert(!_closed);
        return edgeVertex(0, vno);
    }

    FaceType* endVertex(VnoType& vno) const {
        assert(!_closed);
        return edgeVertex(noVtx() - 1, vno);
    }

  private:
    int _edge;                    // starting edge number (0 for closed rings)
    FaceType* _face;              // face where we started to collect the ring
    EnoType _eno;                 // edge where we started
    optdeque<EnoType> _enoVec;    // edges
    optdeque<FaceType*> _faceVec; // faces
    bool _closed;                 // is it closed?
};

#include "facering.impl.hpp"

#endif /* __FACERING_H__ */
