// -*- Mode: c++ -*-
// $Id: pickablequad.hpp,v 1.4 2000/04/30 07:35:46 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/pickablequad.hpp,v $

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

#ifndef __PICKABLEQUAD_H__
#define __PICKABLEQUAD_H__

#include "geoobject.hpp"  // renderable object
#include "pickobject.hpp" // pickable object
#include "quadmesh.hpp"   // quad class wrapper

// internal representation
template <class Mesh> class PickMeshObjectTp;

// interaction and rendering interface for quad meshes
class PickableQuad : public GeoObject, public PickObject {
  public:
    PickableQuad();
    virtual ~PickableQuad();

    const QuadMesh& getMesh() const;
    QuadMesh& getMesh();
    void setMesh(const QuadMesh& mesh);

    virtual void rerender();
    virtual void render();
    virtual void renderPick(unsigned char picks, unsigned int targetCnt, unsigned int shift);
    virtual PickedStuff* doPick(unsigned char r, unsigned char g, unsigned char b, unsigned char picks,
                                unsigned int shift);

    virtual unsigned char& tlRenderMode();

    virtual cvec3f minPoint() const;
    virtual cvec3f maxPoint() const;

  private:
    typedef PickMeshObjectTp<TagMeshTp<Quad>> PickMeshObjectType;
    // internal representation
    PickMeshObjectType* _sobject;
    // associated mesh
    QuadMesh _quadMesh;
};

#endif /* __PICKABLEQUAD_H__ */
