// -*- Mode: c++ -*-
// $Id: subtri.cpp,v 1.4 2000/04/30 12:59:56 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/src/subtri.cpp,v $

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

#include "subtri.hpp"

void Tri::subdivide(int d) {
    EnoType e;
    for (e = 1; e < noVtx() + 1; ++e) {
        computeEven(e, d);
    }

    for (e = 1; e < noVtx() + 1; ++e) {
        computeOdd(e, d);
    }

    // compute odd normals
    //  for(e = 1; e < noVtx()+1; ++e)
    //    computeOddNormal(e, d);

    // clear children mid points points
    for (CnoType c = 0; c < childCount(); ++c) {
        child(c)->clearFace(d + 1);
    }
}

void Tri::midSub(int d) {
    if (isLeaf()) {
        makeChildren(d);
    }

    for (EnoType e = 1; e < noVtx() + 1; ++e) {
        if (!hasMidPos(e, d)) {
            setMidPos(e, d, 0.5f * (headPos(e, d) + tailPos(e, d)));
        }
        if (!hasHeadPos(e, d + 1)) {
            setHeadPos(e, d + 1, headPos(e, d));
        }
    }

    // clear children mid points points
    for (CnoType c = 0; c < childCount(); ++c) {
        child(c)->clearFace(d + 1);
    }
}

void Tri::clearFace(int d) {
    if (childCount() > 0) {
        for (EnoType e = 1; e < noVtx() + 1; ++e) {
            EnoType me;
            Face* mt = midEdge(e, me);
            if (mt) {
                mt->headVert(e)->unset();
                mt->setNormal(mt->headVno(me), 0);
            }
            headVert(e)->set(d);
        }
    }
}

cvec3f Tri::computeEven(EnoType e, int d) {
    if (isLeaf()) {
        makeChildren(d);
    }

    if (hasHeadPos(e, d + 1)) {
        return headPos(e, d + 1);
    } else {
        cvec3f tmp;
        if (headVert(e)->isSpecial()) {
            SubdivideType subdiv;
            subdiv.getHdRuleAndRing(this, e, d);
            tmp = subdiv.computeVertexPoint();
        } else {
            FaceRingType tr;
            tr.collectRing(this, e);
            assert(tr.isClosed());

            cvec3f tmppos(0);
            for (uint i = 0; i < tr.noVtx(); ++i) {
                EnoType e;
                Face* f = tr.face(i, e);
                tmppos += f->headPos(e, d);
            }
            float k = tr.noVtx();
            float beta = (k == 3) ? (3.0f / 16.0f) : (3.0f / 8.0f / k);
            tmp = (1.0 - k * beta) * headPos(e, d) + beta * tmppos;
        }
        setHeadPos(e, d + 1, tmp);
        return tmp;
    }
}

cvec3f Tri::computeOdd(EnoType e, int d) {
    if (isLeaf()) {
        makeChildren(d);
    }

    if (hasMidPos(e, d)) {
        return midPos(e, d);
    } else {
        cvec3f tmp;
        if (headVert(e)->isSpecial() || tailVert(e)->isSpecial()) {
            SubdivideType subdiv;
            if (!SubdivideType::hdRelevantToMidpoint(this, -e)) {
                subdiv.getHdRuleAndRing(this, e, d);
                tmp = subdiv.computeEdgePoint();
            } else if (!SubdivideType::hdRelevantToMidpoint(this, e)) {
                subdiv.getHdRuleAndRing(this, -e, d);
                tmp = subdiv.computeEdgePoint();
            } else {
                subdiv.getHdRuleAndRing(this, e, d);
                tmp = 0.5 * subdiv.computeEdgePoint();
                subdiv.getHdRuleAndRing(this, -e, d);
                tmp += 0.5 * subdiv.computeEdgePoint();
            }
        } else {
            EnoType ne;
            Tri* nt = neighbor(e, ne);
            assert(nt);
            tmp = 3 / 8.0 * (headPos(e, d) + tailPos(e, d)) +
                  1 / 8.0 * (headPos(nextEno(e), d) + nt->headPos(nt->nextEno(ne), d));
        }
        setMidPos(e, d, tmp);
        return tmp;
    }
}

void Tri::computeOddNormal(EnoType e, int d) {

    e = directEno(e);

    EnoType me;
    Face* mf = midEdge(e, me);
    if (!mf->hasNormal(mf->headVno(me))) {

        cvec3f a0, a1, a2;
        cvec3f rc(computeOdd(e, d));
        cvec3f r0(headPos(e, d + 1)), r1(computeOdd(nextEno(e), d)), r2(computeOdd(nextEno(nextEno(e)), d)),
            r3(tailPos(e, d + 1));

        if (edgeTag(e) == Face::CREASE_EDGE) {

            a0 = 2.0f / 3.0f * rc + 1.0f / 6.0f * (r0 + r3);
            a1 = -0.57735 * rc - 0.288675 * r0 + 0.57735 * r1 + 0.57735 * r2 - 0.288675 * r3;
            a2 = 0.5 * r0 - 0.5 * r3;
        } else {
            EnoType neiE;
            Face* neiT = neighbor(e, neiE);
            cvec3f r4(neiT->computeOdd(neiT->nextEno(neiT->nextEno(neiE)), d)),
                r5(neiT->computeOdd(neiT->nextEno(neiE), d));

            a0 = 0.5 * computeOdd(e, d) + 0.5 / 6.0 * (r0 + r1 + r2 + r3 + r4 + r5);

            const float sin60 = 0.86602540378444;
            a1 = 0.0 * r0 + sin60 * r1 + sin60 * r2 + 0.0 * r3 - sin60 * r4 - sin60 * r5;

            a2 = 1.0 * r0 + 0.5 * r1 - 0.5 * r2 - 1.0 * r3 - 0.5 * r4 + 0.5 * r5;
        }
        mf->headVert(me)->setLimitPos(a0);
        mf->setNormal(mf->headVno(me), a2.cross(a1).dir());
    }
}

//--------------------------------------------------------------------

void Tri::computeNormalAndLimit(int d) {
    SubdivideType subdiv;

    // compute edge points (and vertex points by chance)
    for (EnoType e = 1; e < noVtx() + 1; ++e) {
        if (!hasNormal(headVno(e))) {
            subdiv.getHdRuleAndRing(this, e, d);
            setNormal(headVno(e), subdiv.computeNormal());
            vert(headVno(e))->setLimitPos(subdiv.computeLimit());
        }
    }
}

template <class TriRing> CreaseTriRuleTable TriRuleTableTp<TriRing>::_creaseRuleTable;
template <class TriRing> InteriorTriRuleTable TriRuleTableTp<TriRing>::_interiorRuleTable;
template <class TriRing> ConvexTriRuleTable TriRuleTableTp<TriRing>::_convexRuleTable;
template <class TriRing> ConcaveTriRuleTable TriRuleTableTp<TriRing>::_concaveRuleTable;
