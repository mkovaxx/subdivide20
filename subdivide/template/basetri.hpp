// -*- Mode: c++ -*-
// $Id: basetri.hpp,v 1.5 2000/04/29 09:27:19 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/basetri.hpp,v $

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

#ifndef __BASETRI_H__
#define __BASETRI_H__

#include "compat.hpp"
#include "vertex.hpp"

// base template class for top level faces
// in triangle-based face hierarchies (e.g. Loop)
// does not do much, as for triangles top level
// faces are also triangles, unlike quads.

template <class Face> class TLBTriTp : public Face {
  public:
    TLBTriTp(VnoType nVtx, Vertex** v) {
        assert(nVtx == 3);
        for (VnoType i = 0; i < nVtx; ++i) {
            Vertex::ref(v[i]);
            this->_v[i] = v[i];
        }
    }
    virtual ~TLBTriTp() { ; }
};

// Base template class for all faces (both top-level and non-top-level)
// in triangle-based meshes.
// Implements triangle-specific stuff: neighbor finding, noVtx(),
// children creation, access to edges and vertices up and
// down the hierarchy.
// We also keep the array of vertices here, because for
// quad schemes it can be variable size on the top level

template <class BFace> class BaseTriTp : public BFace {
  public:
    typedef typename BFace::Face FaceType;
    typedef typename BFace::TLFace TLFace;

    BaseTriTp() { _v[0] = _v[1] = _v[2] = 0; }

    virtual ~BaseTriTp() {
        for (VnoType v = 0; v < noVtx(); ++v) {
            Vertex::unref(_v[v]);
        }
        if (this->_c != 0) {
            delete[] this->_c;
        }
    }

  public:
    VnoType noVtx() const { return 3; }
    VnoType childCount() const { return (this->_c == 0) ? 0 : 4; }

    void makeChildren(int d = 0);

    FaceType* headSubEdge(EnoType e, EnoType& se) const {
        assert(((FaceType*)this)->checkEno(e));
        if (this->_c == 0) {
            se = 0;
            return 0;
        } else {
            se = _headSubEno[e + 3];
            return &this->_c[_headSubCno[e + 3]];
        }
    }

    FaceType* tailSubEdge(EnoType e, EnoType& se) const {
        assert(((FaceType*)this)->checkEno(e));
        if (this->_c == 0) {
            se = 0;
            return 0;
        } else {
            se = _tailSubEno[e + 3];
            return &this->_c[_tailSubCno[e + 3]];
        }
    }

    FaceType* midEdge(EnoType e, EnoType& me) const {
        if (this->_c == 0) {
            return 0;
        } else {
            me = (abs(e) == 3) ? 1 : (abs(e) + 1);
            return (FaceType*)&this->_c[0];
        }
    }

    FaceType* parentEdge(EnoType e, EnoType& pe) const {
        assert(((FaceType*)this)->checkEno(e));
        pe = (this->_no == 0) ? 0 : _parentEno[this->_no - 1][e + 3];
        return (FaceType*)this->_p;
    }

    FaceType* neighbor(EnoType e, EnoType& ne) const {
        assert(((FaceType*)this)->checkEno(e));
        ne = 0;
        if (this->_p == 0) {
            return ((TLFace*)this)->neighbor(e, ne);
        } else {

            CnoType nc = _neiCno[this->_no][e + 3];
            ne = _neiEno[this->_no][e + 3];

            if (ne != 0) {
                return ((FaceType*)this->_p)->child(nc);
            } else {
                EnoType pe;
                FaceType* pt = parentEdge(e, pe);

                EnoType npe;
                FaceType* npt = pt->neighbor(pe, npe);

                if ((npt == 0) || npt->isLeaf()) {
                    return 0;
                } else {
                    if (((FaceType*)this)->headVert(e) == npt->headVert(npe)) {
                        return npt->headSubEdge(npe, ne);
                    } else {
                        return npt->tailSubEdge(npe, ne);
                    }
                }
            }
        }
    }

  protected:
    Vertex* _v[3];

  private:
    static const EnoType _parentEno[3][7];
    static const CnoType _neiCno[4][7];
    static const EnoType _neiEno[4][7];
    static const CnoType _headSubCno[7];
    static const EnoType _headSubEno[7];
    static const CnoType _tailSubCno[7];
    static const EnoType _tailSubEno[7];
};

#include "basetri.impl.hpp"

#endif /* _BASETRI_H__ */
