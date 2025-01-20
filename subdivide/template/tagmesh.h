// -*- Mode: c++ -*-
// $Id: tagmesh.h,v 1.15 2001/02/07 06:42:01 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/tagmesh.h,v $
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

#ifndef __TAGMESH_H__
#define __TAGMESH_H__

#include "mesh.h"
#include "sectorinfo.h"
#include "tagflatmesh.h"

template <class Face> class TagMeshTp : public MeshTp<Face> {
  public:
    typedef MeshTp<Face> MeshFace;
    typedef typename MeshFace::FaceIterType FaceIterType;
    typedef typename MeshFace::EdgeType EdgeType;
    typedef typename MeshFace::VertexType VertexType;
    typedef typename MeshFace::FaceType FaceType;
    typedef typename MeshFace::TLFaceType TLFaceType;
    typedef typename MeshFace::FaceRingType FaceRingType;

    TagMeshTp() { ; }

    TagMeshTp(const FlatMesh& flatMesh) : MeshTp<Face>(flatMesh) { ; }

    TagMeshTp(const TagFlatMesh& flatMesh) : MeshTp<Face>(flatMesh) {

        EMapType _eMap;
        EdgeMapType _edgeMap;
        typename set<FaceType*>::const_iterator fi;
        for (fi = faceSet().begin(); fi != faceSet().end(); ++fi)
            for (EnoType e = 1; e < (*fi)->noVtx() + 1; ++e) {
                Vertex* headVert = (*fi)->headVert(e);
                Vertex* tailVert = (*fi)->tailVert(e);
                _edgeMap[VertPairType(tailVert, headVert)] = EdgeType((*fi), e);
                _eMap[headVert] = EdgeType((*fi), e);
            }

        applyCreaseEdge(flatMesh, _edgeMap);
        applyVertTag(flatMesh, flatMesh.creaseVertVec, FaceType::CREASE_VERTEX, _eMap);
        applyVertTag(flatMesh, flatMesh.cornerVertVec, FaceType::CORNER_VERTEX, _eMap);
        fixBoundaryTag();
        applyVertTag(flatMesh, flatMesh.dartVertVec, FaceType::NOTAG_VERTEX, _eMap);
        applySector(flatMesh, _edgeMap);
        fixSector();
    }

    void toTagFlatMesh(TagFlatMesh* flatMesh) {
        toFlatMesh(flatMesh);
        setVertTag(flatMesh, flatMesh->creaseVertVec, FaceType::CREASE_VERTEX);
        setVertTag(flatMesh, flatMesh->cornerVertVec, FaceType::CORNER_VERTEX);
        setVertTag(flatMesh, flatMesh->dartVertVec, FaceType::NOTAG_VERTEX);

        // build vertex map
        VertexMapType _vertexMap;

        typename map<Vertex*, pair<int, VertexType>>::const_iterator it = vertMap().begin();
        for (uint i = 0; i < flatMesh->vert_v.size(); ++i) {
            assert(flatMesh->vert_v[i] == (*it).first);
            _vertexMap[flatMesh->vert_v[i]] = i;
            ++it;
        }
        setCreaseEdge(flatMesh, _vertexMap);

        setSector(flatMesh);
    }

    TagMeshTp* clone() const;
    TagMeshTp* clone(map<Vertex*, Vertex*>& vvMap, map<FaceType*, FaceType*>& ttMap) const;
    void setClone(const TagMeshTp& m, map<Vertex*, Vertex*>& vvMap, map<FaceType*, FaceType*>& ttMap);
    void setClone(const TagMeshTp& m);

    void midsub(int maxl = -1) {
        FaceIterType it;

        if (maxl == -1) {
            FaceType* f = (*faceBegin(0));
            maxl = 0;
            while (f->childCount() != 0) {
                f = f->child(0);
                ++maxl;
            }
        }

        for (it = faceBegin(0); it != faceEnd(0); ++it)
            (*it)->clearFace(0);

        for (int l = 0; l < maxl; ++l) {
            for (it = faceBegin(l); (it != faceEnd(l)); ++it)
                if (it.depth() == l) {
                    (*it)->midSub(l);
                }
        }
    }

    void subdivide(int maxl = -1) {
        FaceIterType it;

        for (it = faceBegin(maxl); it != faceEnd(maxl); ++it) {
            (*it)->clearFace(0);
            (*it)->clearNormal();
        }

        if (maxl == -1) {
            FaceType* f = (*faceBegin(0));
            maxl = 0;
            while (f->childCount() != 0) {
                f = f->child(0);
                ++maxl;
            }
        }

        for (int l = 0; l < maxl; ++l) {
            for (it = faceBegin(l); (it != faceEnd(l)); ++it)
                if (it.depth() == l)
                    (*it)->subdivide(l);
        }

        for (it = faceBegin(maxl); it != faceEnd(maxl); ++it)
            for (EnoType e = 1; e < (*it)->noVtx() + 1; ++e)
                //	if((*it)->isLeaf())
                if (it.depth() == maxl)
                    (*it)->computeNormalAndLimit(it.depth());
    }

  protected:
    void cleanup();

  private:
    typedef map<Vertex*, int> VertexMapType;
    typedef map<Vertex*, EdgeType> EMapType;
    typedef pair<Vertex*, Vertex*> VertPairType;
    typedef map<VertPairType, EdgeType> EdgeMapType;
    typedef typename Face::VertexTagType VertexTagType;

    void buildMaps(const TagFlatMesh* flatMesh);

    void applyCreaseEdge(const TagFlatMesh& flatMesh, const EdgeMapType& _edgeMap);
    void applyVertTag(const TagFlatMesh& flatMesh, const vector<int>& index, VertexTagType tag, const EMapType& _eMap);
    void applySector(const TagFlatMesh& flatMesh, const EdgeMapType& _eMap);
    void fixBoundaryTag();
    void fixSector();

    void setVertTag(TagFlatMesh* flatMesh, vector<int>& index, VertexTagType tag) const;

    void setCreaseEdge(TagFlatMesh* flatMesh, const VertexMapType& _vertexMap) const;
    void setSector(TagFlatMesh* flatMesh) const;

    void cloneTag(map<FaceType*, FaceType*>& ttMap);

    void fixVertexToggle(FaceType* f, EnoType e) {
        typename Face::FaceRingType fr;
        fr.collectRing(f, e);
        int creaseCount = 0;
        int i;
        for (i = 0; i < fr.noVtx(); ++i) {
            EnoType ringEno;
            FaceType* ringFace = fr.face(i, ringEno);
            if (ringFace->edgeTag(ringEno) == Face::CREASE_EDGE)
                ++creaseCount;
        }
        assert(f->headVert(e) == fr.centerVert());
        if (creaseCount < 2)
            ((TLFaceType*)f)->setVertexTag(f->headVno(e), FaceType::NOTAG_VERTEX);
        else if (creaseCount == 2)
            ((TLFaceType*)f)->setVertexTag(f->headVno(e), FaceType::CREASE_VERTEX);
        else
            ((TLFaceType*)f)->setVertexTag(f->headVno(e), FaceType::CORNER_VERTEX);

        for (i = 0; i < fr.noFace(); ++i) {
            EnoType ringEno;
            FaceType* ringFace = fr.face(i, ringEno);
            ((TLFaceType*)ringFace)->setSectorInfo(ringFace->tailVno(ringEno), 0);
        }

        VertexTagType vertexTag = f->vertexTag(f->headVno(e));
        if ((vertexTag == FaceType::CREASE_VERTEX) || (vertexTag == FaceType::CORNER_VERTEX))
            for (i = 0; i < fr.noFace(); ++i) {
                EnoType ringEno;
                FaceType* ringFace = fr.face(i, ringEno);
                SectorInfo* si = 0;
                assert(ringFace->tailVert(ringEno) == fr.centerVert());
                if ((si = ringFace->sectorInfo(ringFace->tailVno(ringEno))) == 0) {
                    si = new SectorInfo();
                    if (vertexTag == FaceType::CORNER_VERTEX)
                        si->setSectorTag(SectorInfo::CONVEX_SECTOR);
                    ((TLFaceType*)ringFace)->setSectorInfo(ringFace->tailVno(ringEno), si);
                }
            }
    }

    void specializeEdge(FaceType* f, EnoType e) {
        EnoType he, te;
        if (!f->isLeaf()) {
            assert(f->headVert(e)->isSpecial());
            assert(f->tailVert(e)->isSpecial());
            f->midVert(e)->makeSpecial();
            FaceType* hf = f->headSubEdge(e, he);
            FaceType* tf = f->tailSubEdge(e, te);
            specializeEdge(tf, te);
            specializeEdge(hf, he);
        }
    }

    EdgeMapType _edgeMap;
    map<SectorInfo*, EdgeType> _sectorMap;
};

template <class Face>
void TagMeshTp<Face>::applyVertTag(const TagFlatMesh& m, const vector<int>& tagVertVec, VertexTagType tag,
                                   const EMapType& _eMap) {

    for (uint i = 0; i < tagVertVec.size(); ++i) {
        Vertex* v = m.vert_v[tagVertVec[i]];

        typename map<Vertex*, EdgeType>::const_iterator it = _eMap.find(v);
        if (it != _eMap.end()) {
            TLFaceType* f = (TLFaceType*)((*it).second.face());
            VnoType vno = f->headVno((*it).second.eno());
            assert(f->vert(vno) == v);
            f->setVertexTag(vno, tag);
        }
    }
}

template <class Face> void TagMeshTp<Face>::applyCreaseEdge(const TagFlatMesh& m, const EdgeMapType& _edgeMap) {

    int lastIndex = -1;
    int currentIndex;
    for (uint i = 0; i < m.creaseEdgeVec.size(); ++i) {
        currentIndex = m.creaseEdgeVec[i];
        if ((currentIndex != -1) && (lastIndex != -1)) {
            assert(lastIndex < int(m.vert_v.size()));
            assert(currentIndex < int(m.vert_v.size()));
            Vertex* v0 = m.vert_v[lastIndex];
            Vertex* v1 = m.vert_v[currentIndex];
            typename EdgeMapType::const_iterator it = _edgeMap.find(VertPairType(v0, v1));
            if (it == _edgeMap.end())
                it = _edgeMap.find(VertPairType(v1, v0));
            if (it != _edgeMap.end()) {
                TLFaceType* f = (TLFaceType*)((*it).second.face());
                f->setEdgeTag((*it).second.eno(), Face::CREASE_EDGE);
            }
        }
        lastIndex = currentIndex;
    }
}

template <class Face> void TagMeshTp<Face>::fixSector() {
    FaceIterType fi;
    for (fi = faceBegin(); fi != faceEnd(); ++fi) {
        for (EnoType e = 1; e < (*fi)->noVtx() + 1; ++e) {
            TLFaceType* f = (TLFaceType*)(*fi);
            if ((f->vertexTag(f->headVno(e)) == Face::CREASE_VERTEX) && (f->sectorInfo(f->headVno(e)) == 0)) {

                SectorInfo* si = new SectorInfo();
                si->setSectorTag(SectorInfo::NOTAG_SECTOR);
                f->setSectorInfo(f->headVno(e), si);
            }
            if ((f->vertexTag(f->headVno(e)) == Face::CORNER_VERTEX) && (f->sectorInfo(f->headVno(e)) == 0)) {

                SectorInfo* si = new SectorInfo();
                si->setSectorTag(SectorInfo::CONVEX_SECTOR);
                f->setSectorInfo(f->headVno(e), si);
            }
            if ((f->sectorTag(f->headVno(e)) != SectorInfo::NOTAG_SECTOR) &&
                (f->vertexTag(f->headVno(e)) == Face::NOTAG_VERTEX)) {
            }
        }
    }
}

template <class Face> void TagMeshTp<Face>::fixBoundaryTag() {
    // fix edges
    FaceIterType fi;
    for (fi = faceBegin(); fi != faceEnd(); ++fi) {
        for (EnoType e = 1; e < (*fi)->noVtx() + 1; ++e) {
            TLFaceType* f = (TLFaceType*)(*fi);
            EnoType ne;
            FaceType* nf = f->neighbor(e, ne);
            if (nf == 0) {
                if (f->edgeTag(e) == Face::NOTAG_EDGE) {
                    f->setEdgeTag(e, Face::CREASE_EDGE);
                }
            }
        }
    }

    for (fi = faceBegin(); fi != faceEnd(); ++fi) {
        for (EnoType e = 1; e < (*fi)->noVtx() + 1; ++e) {
            FaceType* f = (*fi);
            EnoType ne;
            FaceType* nf = f->neighbor(e, ne);
            assert(nf || (f->edgeTag(e) == Face::CREASE_EDGE));
        }
    }

    // fix boundary vertices
    for (fi = faceBegin(); fi != faceEnd(); ++fi) {
        for (EnoType e = 1; e < (*fi)->noVtx() + 1; ++e) {
            TLFaceType* f = (TLFaceType*)(*fi);
            if ((f->edgeTag(e) == Face::CREASE_EDGE) && (f->vertexTag(f->headVno(e)) == Face::NOTAG_VERTEX)) {
                typename Face::FaceRingType fr;
                fr.collectRing(f, e);
                int incedentCreaseCount = 0;
                for (uint i = 0; i < fr.noVtx(); ++i) {
                    EnoType ringE;
                    FaceType* ringF = fr.face(i, ringE);
                    if (ringF->edgeTag(ringE) == Face::CREASE_EDGE)
                        ++incedentCreaseCount;
                }
                if (incedentCreaseCount == 1) {
                    ;
                } else if (incedentCreaseCount == 2) {
                    f->setVertexTag(f->headVno(e), Face::CREASE_VERTEX);
                } else if (incedentCreaseCount > 2) {
                    f->setVertexTag(f->headVno(e), Face::CORNER_VERTEX);
                }
            }
        }
    }
}

template <class Face> void TagMeshTp<Face>::applySector(const TagFlatMesh& flatMesh, const EdgeMapType& _edgeMap) {

    for (uint i = 0; i < flatMesh.sectorInfoVec.size(); ++i) {

        SectorInfo* si = flatMesh.sectorInfoVec[i].second;
        int polyIndex = flatMesh.sectorInfoVec[i].first.first;
        uint vno = flatMesh.sectorInfoVec[i].first.second;

        int faceIndex = flatMesh.triindex_v[polyIndex];
        if (vno < flatMesh.poly_v[faceIndex].novtx()) {
            ;
        }

        else {
            assert(flatMesh.poly_v[faceIndex].novtx() == 3);
            faceIndex = faceIndex + ((int)vno) / 3;
            vno = 2;
        }

        FaceType* f;
        EnoType fe;
        Vertex *ppv, *pv, *v, *sv;
        int start = flatMesh.poly_v[faceIndex].start();
        int novtx = flatMesh.poly_v[faceIndex].novtx();
        v = flatMesh.vert_v[flatMesh.index_v[start + vno % novtx]];
        sv = flatMesh.vert_v[flatMesh.index_v[start + (vno + 1) % novtx]];
        pv = flatMesh.vert_v[flatMesh.index_v[start + (vno + novtx - 1) % novtx]];
        ppv = flatMesh.vert_v[flatMesh.index_v[start + (vno + novtx - 2) % novtx]];

        typename EdgeMapType::const_iterator it = _edgeMap.find(VertPairType(pv, v));
        VnoType fv;

        if (it != _edgeMap.end()) {
            f = (*it).second.face();
            fe = (*it).second.eno();
        } else {
            it = _edgeMap.find(VertPairType(v, pv));
            assert(it != _edgeMap.end());
            f = (*it).second.face();
            fe = (*it).second.eno();
        }

        assert(f->tailVert(fe) == pv);
        assert(f->headVert(fe) == v);

        if ((f->headVert(f->nextEno(fe)) == sv) || (f->tailVert(f->prevEno(fe)) == ppv)) {
            fv = f->headVno(fe);
        } else {
            EnoType ne;
            f = f->neighbor(fe, ne);
            assert((f->headVert(f->nextEno(ne)) == sv) || (f->tailVert(f->prevEno(ne)) == ppv));
            fv = f->headVno(ne);
            fe = ne;
        }

        if (f->vertexTag(fv) == FaceType::NOTAG_VERTEX) {
            cerr << "ERROR: SECTOR FOR UNTAGGED VERTEX!" << endl;
            cerr << "vert: " << f->vert(fv) << endl;
            for (VnoType a = 0; a < f->noVtx(); ++a)
                cerr << "\tvert(a): " << f->vert(a) << "\t" << (int)f->vertexTag(a) << endl;
        }

        ((TLFaceType*)f)->setSectorInfo(fv, si);
    }
}

template <class Face> void TagMeshTp<Face>::setSector(TagFlatMesh* m) const {
    map<SectorInfo*, pair<int, int>> _sMap;
    int fcnt = 0;
    typename set<FaceType*>::const_iterator it;
    for (it = faceSet().begin(); it != faceSet().end(); ++it) {
        EnoType e = (*it)->directEno(1);
        VnoType i;
        for (i = 0; i < (*it)->noVtx(); ++i, e = (*it)->nextEno(e)) {
            VnoType v = (*it)->headVno(e);
            if (((*it)->sectorInfo((v)) != 0) && !((*it)->sectorInfo(v)->isDefault())) {
                if ((*it)->sectorInfo(v)->sectorTag() != SectorInfo::NOTAG_SECTOR)
                    assert((*it)->vertexTag(v) != FaceType::NOTAG_VERTEX);

                _sMap[(*it)->sectorInfo(v)] = pair<int, int>(fcnt, i);
            }
        }
        ++fcnt;
    }

    map<SectorInfo*, pair<int, int>>::iterator mi;
    for (mi = _sMap.begin(); mi != _sMap.end(); ++mi) {
        m->sectorInfoVec.push_back(TagFlatMesh::FlatSectorType(pair<int, int>((*mi).second), (*mi).first));
        SectorInfo::ref((*mi).first);
    }
}

template <class Face> void TagMeshTp<Face>::setCreaseEdge(TagFlatMesh* m, const VertexMapType& _vertexMap) const {
    typename set<FaceType*>::const_iterator it;
    for (it = faceSet().begin(); it != faceSet().end(); ++it) {
        for (EnoType e = 1; e < (*it)->noVtx() + 1; ++e) {
            if ((*it)->edgeTag(e) == Face::CREASE_EDGE) {
                EnoType ne;
                FaceType* nt = (*it)->neighbor(e, ne);
                if ((nt == 0) || ((*it) < nt)) {
                    VertexMapType::const_iterator vi0, vi1;
                    vi0 = _vertexMap.find((*it)->tailVert(e));
                    vi1 = _vertexMap.find((*it)->headVert(e));
                    assert((vi0 != _vertexMap.end()) && (vi1 != _vertexMap.end()));
                    int v0 = (*vi0).second;
                    int v1 = (*vi1).second;

                    assert(m->vert_v[v0] == (*vi0).first);
                    assert(m->vert_v[v1] == (*vi1).first);

                    m->creaseEdgeVec.push_back(v0);
                    m->creaseEdgeVec.push_back(v1);
                    m->creaseEdgeVec.push_back(-1);
                }
            }
        }
    }
}

template <class Face> void TagMeshTp<Face>::setVertTag(TagFlatMesh*, vector<int>& tagVertVec, VertexTagType tag) const {
    typename map<Vertex*, pair<int, VertexType>>::const_iterator it;
    int cnt = 0;
    for (it = vertMap().begin(); it != vertMap().end(); ++it) {
        FaceType* f = (*it).second.second.face();
        VnoType vno = (*it).second.second.vno();
        if (f->vert(vno)->isSpecial() && (f->vertexTag(vno) == tag))
            tagVertVec.push_back(cnt);
        ++cnt;
    }
}

template <class Face> TagMeshTp<Face>* TagMeshTp<Face>::clone() const {
    map<Vertex*, Vertex*> vvMap;
    map<FaceType*, FaceType*> ttMap;
    return clone(vvMap, ttMap);
}

template <class Face>
TagMeshTp<Face>* TagMeshTp<Face>::clone(map<Vertex*, Vertex*>& vvMap, map<FaceType*, FaceType*>& ttMap) const {
    TagMeshTp* m = new TagMeshTp();
    m->setClone(*this, vvMap, ttMap);
    return m;
}

template <class Face>
void TagMeshTp<Face>::setClone(const TagMeshTp& m, map<Vertex*, Vertex*>& vvMap, map<FaceType*, FaceType*>& ttMap) {
    if (this != &m) {
        MeshTp<Face>::setClone(m, vvMap, ttMap);
        cloneTag(ttMap);
    }
}

template <class Face> void TagMeshTp<Face>::setClone(const TagMeshTp& m) {
    if (this != &m) {
        map<Vertex*, Vertex*> vvMap;
        map<FaceType*, FaceType*> ttMap;
        MeshTp<Face>::setClone(m, vvMap, ttMap);
        cloneTag(ttMap);
    }
}

template <class Face> void TagMeshTp<Face>::cloneTag(map<FaceType*, FaceType*>& ttMap) {
    typename map<FaceType*, FaceType*>::iterator it;
    for (it = ttMap.begin(); it != ttMap.end(); ++it) {
        FaceType* fOrig = (*it).first;
        TLFaceType* fClone = (TLFaceType*)((*it).second);
        for (EnoType e = 1; e < fOrig->noVtx() + 1; ++e) {
            VnoType headVno = fOrig->headVno(e);
            if (fOrig->edgeTag(e) != Face::NOTAG_EDGE)
                fClone->setEdgeTag(e, fOrig->edgeTag(e));
            typename Face::VertexTagType tag = fOrig->vertexTag(headVno);
            if (tag != Face::NOTAG_VERTEX)
                fClone->setVertexTag(headVno, tag);
        }
    }

    // apply sectors...
    for (it = ttMap.begin(); it != ttMap.end(); ++it) {
        FaceType* fOrig = (*it).first;
        TLFaceType* fClone = (TLFaceType*)((*it).second);
        for (EnoType e = 1; e < fOrig->noVtx() + 1; ++e) {
            VnoType headVno = fOrig->headVno(e);
            if ((fOrig->sectorInfo(headVno) != 0) && (fClone->sectorInfo(headVno) == 0)) {
                fClone->setSectorInfo(headVno, fOrig->sectorInfo(headVno)->clone());
                if (fClone->sectorInfo(headVno)->sectorTag() != SectorInfo::NOTAG_SECTOR) {
                    assert(fOrig->vertexTag(headVno) != FaceType::NOTAG_VERTEX);
                    assert(fClone->vertexTag(headVno) != FaceType::NOTAG_VERTEX);
                }
            }
        }
    }

    fixBoundaryTag();
    fixSector();
}

#endif /* __TAGMESH_H__ */
