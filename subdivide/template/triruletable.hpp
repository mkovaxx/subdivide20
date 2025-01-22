// -*- Mode: c++ -*-
// $Id: triruletable.hpp,v 1.3 2000/04/29 09:27:20 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/triruletable.hpp,v $

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

#ifndef __TRIRULETABLE_H__
#define __TRIRULETABLE_H__

#include "compat.hpp"
#include "trirule.hpp"

// All rule tables together + functions to apply rules to rings

template <class FaceRing> class TriRuleTableTp {
  public:
    typedef FaceRing FaceRingType;
    typedef typename FaceRingType::FaceType FaceType;
    typedef TriRule RuleType;

    static cvec3f applyEdgeCoef(const FaceRingType& tr, const RuleType::EdgeCoefType& coef, int d) {
        EnoType ce, ne;
        FaceType* cf = tr.centerFace(ce);
        FaceType* nf = cf->neighbor(ce, ne);
        cvec3f tmp(0);
        if (nf) {
            tmp = nf->headPos(nf->nextEno(-ne), d);
        }
        return coef.c[0] * cf->headPos(ce, d) + coef.c[1] * cf->headPos(cf->nextEno(ce), d) +
               coef.c[2] * cf->tailPos(ce, d) + coef.c[3] * tmp;
    }

    static cvec3f applyCoef(const FaceRingType& tr, const RuleType::CoefType& coef, int d) {
        cvec3f tmp = coef.centerC * tr.centerVert()->getPos(d);
        for (uint i = 0; i < tr.noVtx(); ++i) {
            tmp += coef.edgeC[i] * tr.vert(i)->getPos(d);
        }
        return tmp;
    }

    static CreaseTriRuleTable _creaseRuleTable;
    static InteriorTriRuleTable _interiorRuleTable;
    static ConvexTriRuleTable _convexRuleTable;
    static ConcaveTriRuleTable _concaveRuleTable;
};

#endif /* __TRIRULETABLE_H__ */
