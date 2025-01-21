// -*- Mode: c++ -*-
// $Id: tlbaseface.h,v 1.4 2000/04/29 07:50:09 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/tlbaseface.h,v $

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

#ifndef __TLBASEFACE_H__
#define __TLBASEFACE_H__

#include "compat.h"

// Top level face functionality common for triangles and quads
// stores pointers to neighbors, edge names from the point of
// view of the neighbors (with orientation indicated by the sign)
// and reference count.

template <class TLBFace> class TLBaseFaceTp : public TLBFace {
  public:
    typedef typename TLBFace::Face Face;
    typedef typename TLBFace::TLFace TLFace;

    TLBaseFaceTp(VnoType nVtx, Vertex** v) : TLBFace(nVtx, v), _refCount(0) {
        _neighborFace = new TLBaseFaceTp*[nVtx];
        _neighborEno = new EnoType[nVtx];
        for (VnoType vno = 0; vno < nVtx; ++vno) {
            _neighborFace[vno] = 0;
            _neighborEno[vno] = 0;
        }
    }
    virtual ~TLBaseFaceTp() {
        if (_neighborFace) {
            delete[] _neighborFace;
        }
        if (_neighborEno) {
            delete[] _neighborEno;
        }
    }

    Face* neighbor(EnoType e, EnoType& ne) const {
        this->checkEno(e);
        if (e > 0) {
            ne = _neighborEno[e - 1];
            return _neighborFace[e - 1];
        } else {
            ne = -_neighborEno[-e - 1];
            return _neighborFace[-e - 1];
        }
    }

    void link(EnoType e, TLBaseFaceTp* nt, EnoType ne) {
        assert(nt);
        assert(ne);
        assert(this->headVert(e) == nt->headVert(ne));
        assert(this->tailVert(e) == nt->tailVert(ne));
        assert(this->headVert(-e) == nt->headVert(-ne));
        assert(this->tailVert(-e) == nt->tailVert(-ne));
        if (e > 0) {
            _neighborFace[e - 1] = nt;
            _neighborEno[e - 1] = ne;
        } else {
            _neighborFace[-e - 1] = nt;
            _neighborEno[-e - 1] = -ne;
        }
    }

    void linkBothWays(EnoType e, TLBaseFaceTp* nt, EnoType ne) {
        assert(this);
        assert(nt);
        assert(ne);
        assert(e);
        link(e, nt, ne);
        nt->link(ne, this, e);
    }

    void unlinkBothWays(EnoType e) {
        EnoType ne;
        Face* nt = neighbor(e, ne);
        if (nt) {
            ((TLBaseFaceTp*)nt)->unlink(ne);
        }
        unlink(e);
    }

    void unlink(EnoType e) {
        assert(this);
        this->_neighborTri[abs(e) - 1] = 0;
        this->_neighborEno[abs(e) - 1] = 0;
    }

    static void ref(TLBaseFaceTp* t) {
        if (t) {
            ++t->_refCount;
        }
    }

    static void unref(TLBaseFaceTp* t) {
        if (t) {
            if ((0 == (--t->_refCount))) {
                delete (t);
            }
        }
    }

  protected:
    unsigned long _refCount;
    TLBaseFaceTp** _neighborFace; // pointers to adjacent faces
    EnoType* _neighborEno;        // edge names from the point of view of
                                  // the adjacent faces
};

#endif /* __TLBASEFACE_H__ */
