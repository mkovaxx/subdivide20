// -*- Mode: c++ -*-
// $Id: subdivide.impl.hpp,v 1.5 2000/04/29 14:56:19 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/subdivide.impl.hpp,v $

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

#include "sectorinfo.hpp"

template <class RuleTable, class FaceRing> void SubdivideTp<RuleTable, FaceRing>::printRelevance() const {
    EnoType e;
    FaceType* t = ring().centerFace(e);
    int vn = t->headVno(e);
    assert(ring().centerVert() == t->headVert(e));
    assert(ring().centerVert() == t->vert(vn));

    SectorInfo* si = t->sectorInfo(vn);
}

template <class RuleTable, class FaceRing>
bool SubdivideTp<RuleTable, FaceRing>::hdRelevantToMidpoint(FaceType* t, EnoType e) {

    int vn = t->headVno(e);

    if (!t->vert(vn)->isSpecial()) {
        return false;
    }

    SectorInfo* si = t->sectorInfo(vn);
    if (si && (si->hasModifiedNormal() || si->hasModifiedFlatness())) {
        return true;
    }

    if ((t->edgeTag(e) == FaceType::NOTAG_EDGE) && (t->vertexTag(vn) == FaceType::NOTAG_VERTEX)) {
        return false;
    }

    if ((t->edgeTag(e) == FaceType::CREASE_EDGE) &&
        ((t->vertexTag(vn) == FaceType::CREASE_VERTEX) || (t->vertexTag(vn) == FaceType::NOTAG_VERTEX))) {
        return false;
    }

    return true;
}

template <class RuleTable, class FaceRing>
void SubdivideTp<RuleTable, FaceRing>::getHdRuleAndRing(FaceType* f, EnoType e, int depth) {

    _depth = depth;

    switch (f->vertexTag(f->headVno(e))) {
    case FaceType::NOTAG_VERTEX:
        _faceRing.collectRing(f, e);
        if (!_faceRing.isClosed()) {
        }
        assert(_faceRing.isClosed());
        _rule = RuleTable::_interiorRuleTable.getRule(_faceRing.noFace());
        break;

    case FaceType::CREASE_VERTEX:
        _faceRing.collectSector(f, e);
        assert(!_faceRing.isClosed());
        _rule = RuleTable::_creaseRuleTable.getRule(_faceRing.noFace());
        break;

    case FaceType::CORNER_VERTEX:

        SectorInfo* si = f->sectorInfo(f->headVno(e));
        assert(si);

        _faceRing.collectSector(f, e);
        assert(!_faceRing.isClosed());
        switch (f->sectorTag(f->headVno(e))) {
        case SectorInfo::CONCAVE_SECTOR:
            _rule = RuleTable::_concaveRuleTable.getRule(_faceRing.noFace(), f->sectorInfo(f->headVno(e))->theta());
            break;
        case SectorInfo::CONVEX_SECTOR:
            _rule = RuleTable::_convexRuleTable.getRule(_faceRing.noFace(), f->sectorInfo(f->headVno(e))->theta());
            break;
        case SectorInfo::NOTAG_SECTOR:
            die();
            break;
        }
        break;
    }
}

template <class RuleTable, class FaceRing> cvec3f SubdivideTp<RuleTable, FaceRing>::computeVertexPoint() const {

    EnoType e;
    FaceType* t = _faceRing.centerFace(e);

    cvec3f res = RuleTable::applyCoef(_faceRing, _rule->subCoef(), _depth);
    SectorInfo* si = t->sectorInfo(t->headVno(e));

    assert(t->headVert(e) == _faceRing.centerVert());

    if (si && si->hasModifiedFlatness()) {
        res = modifyFlatness(res, _rule->x1().centerC, _rule->x2().centerC);
    }

    if (si && si->hasModifiedNormal()) {
        res = ((_faceRing.isClosed()) ? modifyClosedNormal(res, _rule->x1().centerC, _rule->x2().centerC)
                                      : modifyOpenNormal(res, _rule->x1().centerC, _rule->x2().centerC));
    }

    return res;
}

template <class RuleTable, class FaceRing> cvec3f SubdivideTp<RuleTable, FaceRing>::computeEdgePoint() const {

    EnoType e;
    FaceType* t = _faceRing.centerFace(e);

    cvec3f res;

    if (t->edgeTag(e) == FaceType::CREASE_EDGE) {
        res = RuleTable::applyEdgeCoef(_faceRing, _rule->creaseSubCoef(), _depth);
    } else {
        res = RuleTable::applyEdgeCoef(_faceRing, _rule->edgeSubCoef(), _depth);
    }

    float x1 = _rule->x1().edgeC[_faceRing.edgeIndex()];
    float x2 = _rule->x2().edgeC[_faceRing.edgeIndex()];

    SectorInfo* si = 0;
    if ((si = t->sectorInfo(t->headVno(e))) && si->hasModifiedFlatness()) {

        res = modifyFlatness(res, x1, x2);
    }

    if ((si = t->sectorInfo(t->headVno(e))) && si->hasModifiedNormal()) {
        res = (_faceRing.isClosed()) ? modifyClosedNormal(res, x1, x2) : modifyOpenNormal(res, x1, x2);
    }

    return res;
}

template <class RuleTable, class FaceRing> cvec3f SubdivideTp<RuleTable, FaceRing>::computeNormal() const {

    cvec3f a1 = RuleTable::applyCoef(_faceRing, _rule->l1(), _depth);
    cvec3f a2 = RuleTable::applyCoef(_faceRing, _rule->l2(), _depth);

    cvec3f n;
    EnoType ce;
    FaceType* cf = _faceRing.centerFace(ce);
    if (cf->orientation() == CW) {
        n = a2.cross(a1).dir();
    } else {
        n = a1.cross(a2).dir();
    }
    return n.dir();
}

template <class RuleTable, class FaceRing> cvec3f SubdivideTp<RuleTable, FaceRing>::computeLimit() const {
    return RuleTable::applyCoef(_faceRing, _rule->l0(), _depth);
}

template <class RuleTable, class FaceRing>
cvec3f SubdivideTp<RuleTable, FaceRing>::modifyFlatness(const cvec3f& p, float x1, float x2) const {

    EnoType e;
    FaceType* centerFace = _faceRing.centerFace(e);
    int centerVno = centerFace->headVno(e);

    cvec3f res = p;
    cvec3f a0 = RuleTable::applyCoef(_faceRing, _rule->l0(), _depth);
    cvec3f a1 = RuleTable::applyCoef(_faceRing, _rule->l1(), _depth);
    cvec3f a2 = RuleTable::applyCoef(_faceRing, _rule->l2(), _depth);

    SectorInfo* si = centerFace->sectorInfo(centerVno);
    assert(si);
    float s = si->modifiedFlatness();

    res = (1.0 - s) * p + s * (a0 + x1 * _rule->lambda1() * a1 + x2 * _rule->lambda2() * a2);
    return res;
}

template <class RuleTable, class FaceRing>
cvec3f SubdivideTp<RuleTable, FaceRing>::modifyClosedNormal(const cvec3f& p, float x1, float x2) const {
    EnoType e;
    FaceType* centerFace = _faceRing.centerFace(e);
    int centerVno = centerFace->headVno(e);

    assert(_faceRing.isClosed());
    SectorInfo* si = centerFace->sectorInfo(centerVno);
    assert(si && si->hasModifiedNormal());

    cvec3f res(0.0, 0.0, 0.0);
    cvec3f pn = si->modifiedNormal();

    cvec3f a1 = RuleTable::applyCoef(_faceRing, _rule->l1(), _depth);
    cvec3f a2 = RuleTable::applyCoef(_faceRing, _rule->l2(), _depth);

    res = p - si->modifiedNormalT() * pn * (_rule->lambda1() * a1.dot(pn) * x1 + _rule->lambda2() * a2.dot(pn) * x2);

    return res;
}

template <class RuleTable, class FaceRing>
cvec3f SubdivideTp<RuleTable, FaceRing>::modifyOpenNormal(const cvec3f& p, float x1, float x2) const {

    cvec3f res(0.0, 0.0, 0.0);

    EnoType e;
    FaceType* centerFace = _faceRing.centerFace(e);
    int centerVno = centerFace->headVno(e);

    // real tangents
    //
    cvec3f a1 = RuleTable::applyCoef(_faceRing, _rule->l1(), _depth);
    cvec3f a2 = RuleTable::applyCoef(_faceRing, _rule->l2(), _depth);

    // center
    //
    EnoType e1;
    FaceType* t1 = _faceRing.centerFace(e1);

    SectorInfo* si = centerFace->sectorInfo(centerVno);
    assert(si);
    cvec3f n1 = si->modifiedNormal();

    // modified tangents
    //
    cvec3f tan1 = (a1 - a1.dot(n1) * n1).dir();
    cvec3f tan2 = (a2 - a2.dot(n1) * n1).dir();

    int k = _faceRing.noFace();
    EnoType eFirst, eLast;
    FaceType *tFirst, *tLast;

    tFirst = _faceRing.face(0, eFirst);
    tLast = _faceRing.face(k - 1, eLast);

    EnoType e0, e2;
    FaceType* t0 = tFirst->neighbor(eFirst, e0);
    FaceType* t2 = tLast->neighbor(tLast->prevEno(eLast), e2);

    bool twoSectorCorner = false;

    if (t1->vertexTag(t1->headVno(e1)) != FaceType::NOTAG_VERTEX) {
        if (t0 && t2) {
            if (t0->sectorInfo(t0->tailVno(e0)) == t2->sectorInfo(t2->headVno(e2))) {
                twoSectorCorner = true;
            }
        }
    }

    if (!twoSectorCorner) {

        // startV
        //
        if (t0) {

            SectorInfo* si = t0->sectorInfo(t0->tailVno(e0));
            assert(si && si->hasModifiedNormal());
            cvec3f n0 = si->modifiedNormal();

            if (n0.cross(n1).l1() > 0.001) {
                switch (t1->vertexTag(t1->headVno(e1))) {
                case FaceType::NOTAG_VERTEX:
                    die();
                    break;
                    // this case only happens if the mesh is tagged incorrectly
                case FaceType::CREASE_VERTEX:
                    tan2 = (n0.cross(n1)).dir();
                    break;
                case FaceType::CORNER_VERTEX: {
                    SectorInfo* s1 = t1->sectorInfo(t1->headVno(e1));
                    if (s1->sectorTag() == SectorInfo::CONCAVE_SECTOR) {
                        tan1 = (n0.cross(n1)).dir();
                    } else {
                        tan2 = (n0.cross(n1)).dir();
                    }
                    break;
                }
                }
            }
        }

        if (t2) {
            SectorInfo* si = t2->sectorInfo(t2->headVno(e2));
            assert(si && si->hasModifiedNormal());
            cvec3f n2 = si->modifiedNormal();

            if (n1.cross(n2).l1() > 0.001) {
                switch (t1->sectorTag(t1->headVno(e1))) {
                case SectorInfo::CONCAVE_SECTOR:
                    tan2 = (n1.cross(n2)).dir();
                    break;
                case SectorInfo::CONVEX_SECTOR:
                    tan1 = (n1.cross(n2)).dir();
                    break;
                case SectorInfo::NOTAG_SECTOR:
                    die();
                    break;
                }
            }
        }
    }

    assert(_faceRing.centerVert() == t1->headVert(e1));
    si = t1->sectorInfo(t1->headVno(e1));
    assert(si && si->hasModifiedNormal());
    res = p + si->modifiedNormalT() * (x1 * _rule->lambda1() * (a1.dot(tan1) * tan1 - a1) +
                                       x2 * _rule->lambda2() * (a2.dot(tan2) * tan2 - a2));

    return res;
}
