// -*- Mode: c++ -*-
// $Id: pickabletri.h,v 1.4 2000/04/30 07:35:46 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/pickabletri.h,v $

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

#ifndef __PICKABLETRI_H__
#define __PICKABLETRI_H__

#include "pickedstuff.h" // results from picking
#include "pickobject.h"  // pickable object
#include "geoobject.h"   // renderable object
#include "trimesh.h"     // triangle class wrapper

// internal representation
template<class Mesh> class PickMeshObjectTp;


// interaction and rendering interface for triangle meshes

class PickableTri : public GeoObject, public PickObject {
public:
  PickableTri();
  virtual ~PickableTri();

  const TriMesh& getMesh() const;
  TriMesh& getMesh();
  void setMesh(const TriMesh& mesh);

  virtual void rerender();
  virtual void render();
  virtual void renderPick(unsigned char picks = PICK_ALL, 
                          unsigned int targetCnt = 0, unsigned int shift = 1);
  virtual PickedStuff* doPick(unsigned char r, unsigned char g, 
                              unsigned char b, unsigned char picks = PICK_ALL,
			      unsigned int shift = 1);
  virtual unsigned char& tlRenderMode();
  
  virtual cvec3f minPoint() const;
  virtual cvec3f maxPoint() const;

private:
  typedef PickMeshObjectTp<TagMeshTp<Tri> > PickMeshObjectType;
  // internal representation of the selected object
  PickMeshObjectType* _sobject;
  // associated mesh
  TriMesh _triMesh;
};

#endif /* __PICKABLETRI_H__ */


