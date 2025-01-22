// -*- Mode: c++ -*-
// $Id: tagface.hpp,v 1.3 2000/04/29 07:50:09 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/tagface.hpp,v $

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

#ifndef __TAGFACE_H__
#define __TAGFACE_H__

#include "compat.hpp"
#include "facering.hpp"
#include "sectorinfo.hpp"

// Functions to manipulate tags for a face;
// tags are stored only for top-level faces; for all
// other faces the tags are retrieved from the
// top-level parent.
// This template has no data; all tags are in TLTagFaceTp

template <class ConvenientFace> class TagFaceTp : public ConvenientFace {
  public:
    typedef typename ConvenientFace::Face Face;
    typedef typename ConvenientFace::TLFace TLFace;
    typedef FaceRingTp<Face> FaceRingType;
    typedef enum { NOTAG_VERTEX, CORNER_VERTEX, CREASE_VERTEX } VertexTagType;
    typedef enum { NOTAG_EDGE, CREASE_EDGE } EdgeTagType;
    typedef SectorInfo::SectorTagType SectorTagType;

    TagFaceTp() { ; }
    virtual ~TagFaceTp() { ; }

    void makeChildren(int d = 0);
    EdgeTagType edgeTag(EnoType e) const;
    VertexTagType vertexTag(VnoType v) const;
    SectorTagType sectorTag(VnoType v) const;
    SectorInfo* sectorInfo(VnoType v) const;

    const cvec3f& normal(VnoType v) const;
    void setNormal(VnoType v, const cvec3f& n);
    bool hasNormal(VnoType v) const { return (normal(v).l1() != 0); }
};

#include "tagface.impl.hpp"

#endif /* __TAGFACE_H__ */
