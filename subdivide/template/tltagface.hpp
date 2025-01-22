// -*- Mode: c++ -*-
// $Id: tltagface.hpp,v 1.6 2000/04/29 07:50:09 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/tltagface.hpp,v $

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

#ifndef __TLTAGFACE_H__
#define __TLTAGFACE_H__

#include "compat.hpp"
#include "sectorinfo.hpp"

template <class TLBaseFace> class TLTagFaceTp : public TLBaseFace {
  public:
    typedef typename TLBaseFace::Face Face;
    typedef typename TLBaseFace::TLFace TLFace;

    typedef FaceRingTp<Face> FaceRingType;
    typedef typename TLBaseFace::VertexTagType VertexTagType;
    typedef typename TLBaseFace::EdgeTagType EdgeTagType;
    typedef SectorInfo::SectorTagType SectorTagType;

    TLTagFaceTp(VnoType nVtx, Vertex** v) : TLBaseFace(nVtx, v) {
        assert(nVtx > 0);
        _vertexTag = new VertexTagType[nVtx];
        _edgeTag = new EdgeTagType[nVtx];
        _sectorInfo = new SectorInfo*[nVtx];
        for (int i = 0; i < nVtx; ++i) {
            _vertexTag[i] = VertexTagType::NOTAG_VERTEX;
            _edgeTag[i] = EdgeTagType::NOTAG_EDGE;
            _sectorInfo[i] = 0;
        }
    }

    virtual ~TLTagFaceTp() {
        for (int i = 0; i < this->noVtx(); ++i) {
            SectorInfo::unref(_sectorInfo[i]);
        }
        delete[] _vertexTag;
        delete[] _edgeTag;
        delete[] _sectorInfo;
    }

    void setVertexTag(VnoType vno, VertexTagType vertexTag) {
        this->vert(vno)->makeSpecial();
        EnoType eno = this->enoTo(vno);
        FaceRingType tr;
        tr.collectRing(this, eno);
        for (uint i = 0; i < tr.noFace(); ++i) {
            EnoType te;
            TLFace* tf = (TLFace*)tr.face(i, te);
            assert(tf->tailVert(te) == this->vert(vno));
            tf->setTLVertexTag(tf->tailVno(te), vertexTag);
        }
    }

    void setEdgeTag(EnoType eno, EdgeTagType edgeTag) {
        EnoType ne;
        Face* nf = this->neighbor(eno, ne);

        setTLEdgeTag(eno, edgeTag);
        if (nf != 0) {
            ((TLFace*)nf)->setTLEdgeTag(ne, edgeTag);
        }

        this->headVert(eno)->makeSpecial();
        this->tailVert(eno)->makeSpecial();
    }

    void setSectorInfo(VnoType vno, SectorInfo* sectorInfo) {
        this->vert(vno)->makeSpecial();
        EnoType eno = this->enoTo(vno);
        FaceRingType tr;
        tr.collectSector(this, eno);
        for (uint i = 0; i < tr.noFace(); ++i) {
            EnoType te;
            TLFace* tf = (TLFace*)tr.face(i, te);
            assert(tf->tailVert(te) == this->vert(vno));
            tf->setTLSectorInfo(tf->tailVno(te), sectorInfo);
        }
    }

  public:
    EdgeTagType edgeTag(EnoType e) const {
        assert(this->checkEno(e));
        return _edgeTag[abs(e) - 1];
    }
    VertexTagType vertexTag(VnoType v) const {
        assert(this->checkVno(v));
        return _vertexTag[v];
    }
    SectorTagType sectorTag(VnoType v) const {
        assert(this->checkVno(v));
        return (_sectorInfo[v] == 0) ? SectorInfo::NOTAG_SECTOR : _sectorInfo[v]->sectorTag();
    }
    SectorInfo* sectorInfo(VnoType v) const {
        this->checkVno(v);
        return _sectorInfo[v];
    }

    void setTLVertexTag(VnoType v, VertexTagType vertexTag) {
        this->checkVno(v);
        _vertexTag[v] = vertexTag;
    }
    void setTLEdgeTag(EnoType e, EdgeTagType edgeTag) {
        this->checkEno(e);
        _edgeTag[abs(e) - 1] = edgeTag;
    }
    void setTLSectorInfo(VnoType v, SectorInfo* sectorInfo) {
        SectorInfo::ref(sectorInfo);
        SectorInfo::unref(_sectorInfo[v]);
        this->checkVno(v);
        _sectorInfo[v] = sectorInfo;
    }

  private:
    VertexTagType* _vertexTag;
    EdgeTagType* _edgeTag;
    SectorInfo** _sectorInfo;
};

#endif /* __TLTAGFACE_H__ */
