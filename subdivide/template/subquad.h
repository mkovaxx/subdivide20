// -*- Mode: c++ -*-
// $Id: subquad.h,v 1.3 2000/04/29 07:50:09 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/subquad.h,v $

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

#ifndef __SUBQUAD_H__
#define __SUBQUAD_H__

#include "baseface.h"
#include "basequad.h"
#include "compat.h"
#include "convface.h"
#include "geoface.h"
#include "tagface.h"

#include "tlbaseface.h"
#include "tltagface.h"

#include "quadruletable.h"
#include "subdivide.h"

class TLQuad;
class Quad;

// Build a class from the templates that handle different functionality
// (hierarchy, geometry and tags), add subdivision functions. The innermost
// base class is parametrized by the most derived class so that we can have
// generic accessors defined in the templates that return correct types.

// TODO remove first parameter of BaseFaceTp

class Quad : public TagFaceTp<                        // normals and tags
                 GeoFaceTp<                           // accessors to geom information
                     ConvenientFaceTp<                // accessors to children etc.
                         BaseQuadTp<                  // quad mesh neighbors and children
                             BaseFaceTp<Quad, TLQuad> // base class for all faces
                             >>>> {
  public:
    typedef QuadRuleTableTp<FaceRingType> QuadRuleTableType;
    typedef SubdivideTp<QuadRuleTableType, FaceRingType> SubdivideType;

    Quad() { ; }
    virtual ~Quad() { ; }

    void clearFace(int d = 0);
    void clearNormal() {
        for (VnoType v = 0; v < noVtx(); ++v)
            setNormal(v, 0);
    }
    void midSub(int d = 0);

    // complete subdivision
    void subdivide(int d);

    // compute normal and limit
    void computeNormalAndLimit(int d);

  private:
    bool hasCenterPos(int d) const { return centerVert()->isSet(d + 1); }
    void setCenterPos(int d, const cvec3f& p) {
        centerVert()->setPos(d + 1, p);
        centerVert()->set(d + 1);
    }
    cvec3f centerPos(int d) const { return centerVert()->getPos(d + 1); }

    cvec3f computeVertexPoint(EnoType e, int d);
    void computeFaceNormal(int d);
    void computeEdgeNormal(EnoType e, int d);
    cvec3f computeFacePoint(int d);
    cvec3f computeEdgePoint(EnoType e, int d);
};

// class for top-level faces in quad meshes; adds tag information
// and pointers to neighbors to Quad

class TLQuad : public TLTagFaceTp< // top level tag information
                   TLBaseFaceTp<   // pointers to neighbors etc
                       TLBQuadTp<  // number of vertices for top level
                           Quad>>> {
  public:
    TLQuad(VnoType nVtx, Vertex** v) : TLTagFaceTp<TLBaseFaceTp<TLBQuadTp<Quad>>>(nVtx, v) {}
    virtual ~TLQuad() { ; }

    // needed for cloning
    static TLQuad* createFromFace(Face* f) {
        Vertex** v = new Vertex*[f->noVtx()];
        for (int i = 0; i < f->noVtx(); ++i)
            v[i] = f->vert(i);
        TLQuad* nf = new TLQuad(f->noVtx(), v);
        nf->_orient = f->orientation();
        delete[] v;
        return nf;
    }
};

#endif /* __SUBQUAD_H__ */
