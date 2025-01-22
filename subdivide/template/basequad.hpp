// -*- Mode: c++ -*-
// $Id: basequad.hpp,v 1.5 2000/04/29 07:50:08 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/basequad.hpp,v $

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

#ifndef __BASEQUAD_H__
#define __BASEQUAD_H__

#include "compat.hpp"
#include "vertex.hpp"

// base template class for top level faces
// in quad-based face hierarchies (e.g. Catmull-Clark)
// just carries the number of vertices around
// only top-level faces have to carry this number around
// for non-top-level faces, noVtx() returns a fixed constant (4)

template <class BaseFace> class TLBQuadTp : public BaseFace {
  public:
    typedef typename BaseFace::Face Face;
    typedef typename BaseFace::TLFace TLFace;

    TLBQuadTp(VnoType nVtx, Vertex** v) : _noVtx(nVtx) {
        this->_v = new Vertex*[nVtx];
        for (VnoType i = 0; i < nVtx; ++i) {
            Vertex::ref(v[i]);
            this->_v[i] = v[i];
        }
    }
    virtual ~TLBQuadTp() {
        if (this->_v != 0) {
            for (VnoType v = 0; v < noVtx(); ++v) {
                Vertex::unref(this->_v[v]);
            }
            delete[] this->_v;
            this->_v = 0;
        }
    }
    VnoType noVtx() const { return _noVtx; }

  private:
    VnoType _noVtx; // number of vertices
};

// Base template class for all faces (both top-level and non-top-level)
// in quad-based meshes.
// Implements quad-specific stuff: neighbor finding, noVtx(),
// children creation, access to edges and vertices up and
// down the hierarchy.
// We also keep the array of vertices here, because for
// quad schemes it can be variable size on the top level

template <class BaseFace> class BaseQuadTp : public BaseFace {
  public:
    typedef typename BaseFace::Face Face;
    typedef typename BaseFace::TLFace TLFace;

    BaseQuadTp() : _v(0) { ; }
    virtual ~BaseQuadTp() {
        if (_v != 0) {
            for (VnoType v = 0; v < 4; ++v) {
                Vertex::unref(_v[v]);
            }
            delete[] _v;
            _v = 0;
        }
        if (this->_c != 0) {
            delete[] this->_c;
        }
    }

  public:
    VnoType childCount() const { return (this->_c == 0) ? 0 : noVtx(); }

    VnoType noVtx() const {
        VnoType v = (this->_p != 0) ? 4 : ((TLBQuadTp<Face>*)this)->noVtx();
        return v;
    }

    void makeChildren(int d = 0);

    Face* headSubEdge(EnoType e, EnoType& se) const {
        assert(((Face*)this)->checkEno(e));
        if (this->_c == 0) {
            se = 0;
            return 0;
        } else {
            CnoType c = (e > 0) ? (e % noVtx()) : ((-e) - 1);
            se = (e > 0) ? 4 : -1;
            return &this->_c[c];
        }
    }

    Face* tailSubEdge(EnoType e, EnoType& se) const {
        assert(((Face*)this)->checkEno(e));
        if (this->_c == 0) {
            se = 0;
            return 0;
        } else {
            CnoType c = (e > 0) ? (e - 1) : ((-e) % noVtx());
            se = (e > 0) ? 1 : -4;
            return &this->_c[c];
        }
    }

    Face* midEdge(EnoType e, EnoType& me) const {
        assert(((Face*)this)->checkEno(e));
        me = 0;
        if (this->_c == 0) {
            return 0;
        } else {
            me = 1;
            return (Face*)&this->_c[abs(e) - 1];
        }
    }

    Face* centerEdge(EnoType& ce) const {
        ce = 0;
        if (this->_c == 0) {
            return 0;
        } else {
            ce = 2;
            return (Face*)this->_c;
        }
    }

    Vertex* centerVert() const { return (this->_c == 0) ? 0 : this->_c[0].v()[2]; }

    Face* parentEdge(EnoType e, EnoType& pe) const {
        assert(((Face*)this)->checkEno(e));
        switch (e) {
        case 1:
            pe = this->_no + 1;
            break;
        case -1:
            pe = -(this->_no + 1);
            break;
        case 4:
            pe = (this->_no == 0) ? ((Face*)this->_p)->noVtx() : this->_no;
            break;
        case -4:
            pe = -((this->_no == 0) ? ((Face*)this->_p)->noVtx() : this->_no);
            break;
        default:
            pe = 0;
        }
        return (Face*)this->_p;
    }

    typedef typename BaseFace::Face BaseFaceType;

    BaseFaceType* neighbor(EnoType e, EnoType& ne) const {
        assert(((Face*)this)->checkEno(e));
        ne = 0;
        if (this->_p == 0) {
            return ((TLFace*)this)->neighbor(e, ne);
        } else {

            CnoType nc = 0;
            switch (e) {
            case 2:
                ne = -3;
                nc = (this->_no + 1) % ((Face*)this->_p)->noVtx();
                break;
            case -2:
                ne = 3;
                nc = (this->_no + 1) % ((Face*)this->_p)->noVtx();
                break;
            case 3:
                ne = -2;
                nc = (this->_no + ((Face*)this->_p)->noVtx() - 1) % ((Face*)this->_p)->noVtx();
                break;
            case -3:
                ne = 2;
                nc = (this->_no + ((Face*)this->_p)->noVtx() - 1) % ((Face*)this->_p)->noVtx();
                break;
            }

            if (ne != 0) {
                return ((Face*)this->_p)->child(nc);
            } else {
                EnoType pe;
                Face* pt = parentEdge(e, pe);
                EnoType npe;
                Face* npt = pt->neighbor(pe, npe);

                if ((npt == 0) || npt->isLeaf()) {
                    return 0;
                } else {
                    if (((Face*)this)->headVert(e) == npt->headVert(npe)) {
                        return npt->headSubEdge(npe, ne);
                    } else {
                        return npt->tailSubEdge(npe, ne);
                    }
                }
            }
        }
    }

    Vertex**& v() { return _v; }

  protected:
    Vertex** _v;
};

#include "basequad.impl.hpp"

#endif /* __BASEQUAD_H__ */
