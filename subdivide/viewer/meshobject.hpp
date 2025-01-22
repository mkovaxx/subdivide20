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

#ifndef __MESHOBJECT_H__
#define __MESHOBJECT_H__

#include "compat.h"
#include "geoobject.h"

template <class Mesh> class MeshObjectTp : public GeoObject {
  public:
    MeshObjectTp();
    virtual ~MeshObjectTp() { ; }
    const Mesh& getMesh() const { return _mesh; }
    Mesh& getMesh() { return _mesh; }
    void setMesh(Mesh& mesh) {
        _mesh = mesh;
        // compute min/max points
        typename Mesh::FaceIterType ti = _mesh.faceBegin();
        _maxPoint = _minPoint = (*ti)->vert(0)->getPos();
        for (ti = _mesh.faceBegin(); ti != _mesh.faceEnd(); ++ti) {
            for (VnoType v = 0; v < (*ti)->noVtx(); ++v) {
                if ((*ti)->isLeaf()) {
                    cvec3f p = (*ti)->vert(v)->getPos();
                    _maxPoint = _maxPoint.max(p);
                    _minPoint = _minPoint.min(p);
                }
            }
        }
    }

    virtual void render();
    virtual void rerender() { render(); }
    virtual cvec3f minPoint() const { return _minPoint; }
    virtual cvec3f maxPoint() const { return _maxPoint; }

  protected:
    Mesh _mesh;

  private:
    cvec3f _minPoint, _maxPoint;
};

#include "meshobject.impl.h"

#endif /* __MESHOBJECT_H__ */
