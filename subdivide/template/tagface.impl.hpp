// -*- Mode: c++ -*-
// $Id: tagface.impl.hpp,v 1.4 2000/04/30 07:35:48 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/tagface.impl.hpp,v $

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

template <class F> void TagFaceTp<F>::makeChildren(int d) {
    assert(this->isLeaf());
    ((F*)this)->makeChildren(d);
    for (EnoType e = 1; e < this->noVtx() + 1; ++e) {
        if (edgeTag(e) != NOTAG_EDGE) {
            this->midVert(e)->makeSpecial();
        }
    }
}

template <class F> auto TagFaceTp<F>::edgeTag(EnoType e) const -> TagFaceTp<F>::EdgeTagType {

    if (this->headVert(e)->isSpecial()) {

        if (this->isToplevel()) {
            return ((TLFace*)this)->edgeTag(e);
        }

        EnoType te;
        Face* tf = this->toplevelEdge(e, te);
        if (tf == 0) {
            return NOTAG_EDGE;
        } else {
            return ((TLFace*)tf)->edgeTag(te);
        }
    } else {
        return NOTAG_EDGE;
    }
}

template <class F> TagFaceTp<F>::VertexTagType TagFaceTp<F>::vertexTag(VnoType v) const {
    if (this->vert(v)->isSpecial()) {

        VnoType tv;
        Face* tf = this->toplevelVertex(v, tv);
        if (tf) {
            return ((TLFace*)tf)->vertexTag(tv);
        }

        Face* pt = this->parent();
        assert(pt);
        EnoType pe = pt->noVtx() + 1;
        while ((pe == pt->noVtx() + 1) && (pt != 0)) {
            pe = 1;
            while ((pe < pt->noVtx() + 1) && (pt->midVert(pe) != this->vert(v))) {
                ++pe;
            }
            if (pe == pt->noVtx() + 1) {
                pt = pt->parent();
                pe = pt->noVtx() + 1;
            }
        }
        assert(pt);
        assert(pt->midVert(pe) == this->vert(v));
        if (pt->edgeTag(pe) == NOTAG_EDGE) {
            return NOTAG_VERTEX;
        } else {
            return CREASE_VERTEX;
        }
    } else {
        return NOTAG_VERTEX;
    }
}

template <class F> TagFaceTp<F>::SectorTagType TagFaceTp<F>::sectorTag(VnoType v) const {

    SectorInfo* s = sectorInfo(v);
    if (s == 0) {
        return SectorInfo::NOTAG_SECTOR;
    } else {
        return s->sectorTag();
    }
}

template <class F> SectorInfo* TagFaceTp<F>::sectorInfo(VnoType v) const {
    if (this->vert(v)->isSpecial()) {

        if (this->isToplevel()) {
            return ((TLFace*)this)->sectorInfo(v);
        }

        VnoType tv;
        Face* tf = this->toplevelVertex(v, tv);
        if (tf != 0) {
            return ((TLFace*)tf)->sectorInfo(tv);
        } else {
            return 0;
        }

    } else {
        return 0;
    }
}

template <class F> const cvec3f& TagFaceTp<F>::normal(VnoType v) const {
    if (this->vert(v)->isSpecial()) {
        SectorInfo* s = sectorInfo(v);
        if (s != 0) {
            return s->normal();
        } else if (vertexTag(v) == NOTAG_VERTEX) {
            return this->vert(v)->normal0();
        } else {
            assert(vertexTag(v) == CREASE_VERTEX);
            assert(!this->isToplevel());
            // we need to find an edge for this vertex...

            Face* pt = this->parent();
            assert(pt);
            EnoType pe = pt->noVtx() + 1;
            while ((pe == pt->noVtx() + 1) && (pt != 0)) {
                pe = 1;
                while ((pe < pt->noVtx() + 1) && (pt->midVert(pe) != this->vert(v))) {
                    ++pe;
                }
                if (pe == pt->noVtx() + 1) {
                    pt = pt->parent();
                    pe = pt->noVtx() + 1;
                }
            }
            assert(pt);
            assert(pt->midVert(pe) == this->vert(v));

            EnoType ne;
            Face* nf = pt->neighbor(pe, ne);
            if (nf < pt) {
                return this->vert(v)->normal0();
            } else {
                return this->vert(v)->normal1();
            }
        }
    } else {
        return this->vert(v)->normal0();
    }
}

template <class F> void TagFaceTp<F>::setNormal(VnoType v, const cvec3f& n) {
    if (this->vert(v)->isSpecial()) {
        SectorInfo* s = sectorInfo(v);
        if (s != 0) {
            s->setNormal(n);
        } else if (vertexTag(v) == NOTAG_VERTEX) {
            this->vert(v)->setNormal0(n);
        } else {
            assert(vertexTag(v) == CREASE_VERTEX);
            assert(!this->isToplevel());
            // we need to find an edge for this vertex...

            Face* pt = this->parent();
            EnoType pe = pt->noVtx() + 1;
            while ((pe == pt->noVtx() + 1) && (pt != 0)) {
                pe = 1;
                while ((pe < pt->noVtx() + 1) && (pt->midVert(pe) != this->vert(v))) {
                    ++pe;
                }
                if (pe == pt->noVtx() + 1) {
                    pt = pt->parent();
                    pe = pt->noVtx() + 1;
                }
            }
            assert(pt);
            assert(pt->midVert(pe) == this->vert(v));

            EnoType ne;
            Face* nf = pt->neighbor(pe, ne);
            if (nf < pt) {
                this->vert(v)->setNormal0(n);
            } else {
                this->vert(v)->setNormal1(n);
            }
        }
    } else {
        this->vert(v)->setNormal0(n);
    }
}
