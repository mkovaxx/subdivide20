// -*- Mode: c++ -*-
// $Id: subtri.h,v 1.4 2000/04/29 07:50:09 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/subtri.h,v $

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

#ifndef __SUBTRI_H__
#define __SUBTRI_H__

#include "baseface.h"
#include "basetri.h"
#include "compat.h"
#include "convface.h"
#include "geoface.h"
#include "tagface.h"

#include "tlbaseface.h"
#include "tltagface.h"

#include "subdivide.h"
#include "triruletable.h"

class TLTri;
class Tri;

class Tri : public TagFaceTp<GeoFaceTp<ConvenientFaceTp<BaseTriTp<BaseFaceTp<Tri, TLTri>>>>> {
  public:
    typedef TriRuleTableTp<FaceRingType> TriRuleTableType;
    typedef SubdivideTp<TriRuleTableType, FaceRingType> SubdivideType;

    Tri() { ; }
    virtual ~Tri() { ; }

    void clearFace(int d = 0);
    void clearNormal() {
        for (VnoType v = 0; v < noVtx(); ++v) {
            setNormal(v, 0);
        }
    }

    // midpoint subdivision
    void midSub(int d);

    // basic subdivision without tags and modified normal, simple but fast
    void subdivideBasic(int d);

    // compute normals for the triangle, basic as above
    void computeNormalAndLimitBasic(int d);

    // complete subdivision
    void subdivide(int d);

    // compute normal and limit
    void computeNormalAndLimit(int d);

  private:
    // compute vertex positions in the general case -
    // considering tags, flatness and normal modification
    cvec3f computeEven(EnoType e, int d);
    cvec3f computeOdd(EnoType e, int d);

    // compute vertex positions in the absence of tags and
    // any kind of modification
    void computeOddNormal(EnoType e, int d);
};

class TLTri : public TLTagFaceTp<TLBaseFaceTp<TLBTriTp<Tri>>> {
  public:
    TLTri(VnoType nVtx, Vertex** v) : TLTagFaceTp<TLBaseFaceTp<TLBTriTp<Tri>>>(nVtx, v) {}

    virtual ~TLTri() { ; }

    static TLTri* createFromFace(Face* f) {
        Vertex** v = new Vertex*[f->noVtx()];
        for (int i = 0; i < f->noVtx(); ++i) {
            v[i] = f->vert(i);
        }
        TLTri* nf = new TLTri(f->noVtx(), v);
        assert(nf);
        nf->_orient = f->orientation();
        delete[] v;
        return nf;
    }
};

#endif /* __SUBTRI_H__ */
