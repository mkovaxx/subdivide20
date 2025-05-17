// -*- Mode: c++ -*-
// $Id: subquad.hpp,v 1.3 2000/04/29 07:50:09 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/subquad.hpp,v $

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

#include "baseface.hpp"
#include "basequad.hpp"
#include "compat.hpp"
#include "convface.hpp"
#include "geoface.hpp"
#include "tagface.hpp"

#include "tlbaseface.hpp"
#include "tltagface.hpp"

#include "quadruletable.hpp"
#include "subdivide.hpp"

class TLQuad;
class Quad;

// Build a class from the templates that handle different functionality
// (hierarchy, geometry and tags), add subdivision functions. The innermost
// base class is parametrized by the most derived class so that we can have
// generic accessors defined in the templates that return correct types.

// TODO remove first parameter of BaseFaceTp

class Quad;

template <class Face> struct QuadFaceRingType {
    typedef FaceRingTp<Face> type;
};

class Quad : public TagFaceTp<                        // normals and tags
                 GeoFaceTp<                           // accessors to geom information
                     ConvenientFaceTp<                // accessors to children etc.
                         BaseQuadTp<                  // quad mesh neighbors and children
                             BaseFaceTp<Quad, TLQuad> // base class for all faces
                             >>>> {
public:
    // Make FaceRingType public for use in templates
    typedef typename QuadFaceRingType<Quad>::type FaceRingType;
    typedef QuadRuleTableTp<FaceRingType> QuadRuleTableType;
    typedef SubdivideTp<QuadRuleTableType, FaceRingType> SubdivideType;

    Quad() : _centerVert(nullptr) {
        for (int i = 0; i < 4; ++i) {
            _normal[i] = cvec3f(0, 0, 0);
        }
    }
    virtual ~Quad() {
        if (_centerVert) {
            delete _centerVert;
        }
    }

    // Forward declarations for member functions
    VnoType noVtx() const;
    void setNormal(VnoType v, const cvec3f& n);
    cvec3f getNormal(VnoType v) const;

    // Center vertex related functions
    Vertex* centerVert() const;
    void setCenterVert(Vertex* v);

    // Vertex position related functions
    cvec3f getPos(VnoType v) const;

    // Tag related functions
    bool isSet(VnoType v) const;
    void set(VnoType v, bool value = true);

    void clearFace(int d = 0);
    void clearNormal() {
        for (VnoType v = 0; v < noVtx(); ++v) {
            setNormal(v, cvec3f(0, 0, 0));
        }
    }
    void midSub(int d = 0);
    
    // Center position related functions
    bool hasCenterPos(int d) const {
        Vertex* cv = centerVert();
        return cv && cv->isSet(d + 1);
    }

    void setCenterPos(int d, const cvec3f& p) {
        if (Vertex* cv = centerVert()) {
            cv->setPos(d + 1, p);
            cv->set(d + 1);
        }
    }

    cvec3f centerPos(int d) const {
        Vertex* cv = centerVert();
        return cv ? cv->getPos(d + 1) : cvec3f(0, 0, 0);
    }

    // complete subdivision
    void subdivide(int d);

    // compute normal and limit
    void computeNormalAndLimit(int d);

  private:
    cvec3f _normal[4];  // Normal vectors for each vertex
    Vertex* _centerVert; // Center vertex for subdivision

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
        for (int i = 0; i < f->noVtx(); ++i) {
            v[i] = f->vert(i);
        }
        TLQuad* nf = new TLQuad(f->noVtx(), v);
        nf->_orient = f->orientation();
        delete[] v;
        return nf;
    }
};

#endif /* __SUBQUAD_H__ */
