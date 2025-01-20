// -*- Mode: c++ -*-
// $Id: quadruletable.h,v 1.4 2000/04/30 17:16:22 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/quadruletable.h,v $

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

#ifndef __QUADRULETABLE_H__
#define __QUADRULETABLE_H__

#include "compat.h"
#include "quadrule.h"

// All rule tables together + functions to apply rules to rings

template <class FaceRing> class QuadRuleTableTp {
  public:
    typedef FaceRing FaceRingType;
    typedef typename FaceRingType::FaceType FaceType;
    typedef QuadRule RuleType;

    static cvec3f applyEdgeCoef(const FaceRingType& tr, const RuleType::EdgeCoefType& coef, int d) {
        EnoType ce, ne;
        FaceType* cf = tr.centerFace(ce);
        FaceType* nf = cf->neighbor(ce, ne);

        if (d == 0) {
            cvec3f a0, a1;
            cvec3f pos[6];
            int i;
            for (i = 0; i < cf->noVtx(); ++i)
                a0 += 1 / float(cf->noVtx()) * cf->pos(i, d);
            pos[0] = cf->headPos(ce, d);
            pos[1] = cf->headPos(cf->nextEno(ce), d);
            pos[3] = cf->tailPos(ce, d);
            pos[2] = 4 * a0 - pos[0] - pos[1] - pos[3];

            if (nf) {
                int i;
                for (i = 0; i < nf->noVtx(); ++i)
                    a1 += 1 / float(nf->noVtx()) * nf->pos(i, d);
                pos[5] = nf->headPos(nf->nextEno(ne), d);
                pos[4] = 4 * a1 - pos[0] - pos[3] - pos[5];
            }

            return coef.cf[0] * pos[0] + coef.cf[1] * pos[1] + coef.cf[2] * pos[2] + coef.cf[3] * pos[3] +
                   coef.cf[4] * pos[4] + coef.cf[5] * pos[5];

        } else {
            cvec3f tmp(0);
            if (nf)
                tmp = coef.cf[4] * nf->headPos(nf->nextEno(-ne), d) +
                      coef.cf[5] * nf->headPos(nf->nextEno(nf->nextEno(-ne)), d);
            return coef.cf[0] * cf->headPos(ce, d) + coef.cf[1] * cf->headPos(cf->nextEno(ce), d) +
                   coef.cf[2] * cf->headPos(cf->nextEno(cf->nextEno(ce)), d) + coef.cf[3] * cf->tailPos(ce, d) + tmp;
        }
    }

    static cvec3f applyCoef(const FaceRingType& tr, const RuleType::CoefType& coef, int d) {

        cvec3f tmp = coef.centerC * tr.centerVert()->getPos(d);
        uint i;
        for (i = 0; i < tr.noVtx(); ++i)
            tmp += coef.edgeC[i] * tr.vert(i)->getPos(d);

        for (i = 0; i < tr.noFace(); ++i) {

            EnoType fe;
            FaceType* f = tr.face(i, fe);

            cvec3f t = f->headVert(f->nextEno(fe))->getPos(d);
            if (d == 0) {
                cvec3f a(0);
                for (VnoType v = 0; v < f->noVtx(); ++v)
                    a += 1 / float(f->noVtx()) * f->pos(v, d);
                t = 4 * a - f->headPos(fe, d) - f->headPos(f->nextEno(fe), d) - f->tailPos(fe, d);
            }

            tmp += coef.faceC[i] * t;
        }

        return tmp;
    }

    static CreaseQuadRuleTable _creaseRuleTable;
    static InteriorQuadRuleTable _interiorRuleTable;
    static ConvexQuadRuleTable _convexRuleTable;
    static ConcaveQuadRuleTable _concaveRuleTable;
};

#endif /* __QUADRULETABLE_H__ */
