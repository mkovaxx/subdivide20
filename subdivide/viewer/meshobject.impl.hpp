// -*- Mode: c++ -*-
// $Id: meshobject.impl.hpp,v 1.1 2000/04/29 12:29:27 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/viewer/meshobject.impl.hpp,v $

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

#include <GLFW/glfw3.h>

template <class Mesh> MeshObjectTp<Mesh>::MeshObjectTp() : _mesh(), _minPoint(0), _maxPoint(0) {}

template <class Mesh> void MeshObjectTp<Mesh>::render() {

    int lastNoVtx = 3;

    glBegin(GL_TRIANGLES);
    typename Mesh::FaceIterType it;
    for (it = _mesh.faceBegin(); it != _mesh.faceEnd(); ++it) {

        if ((*it)->isLeaf()) {

            int novtx = (*it)->noVtx();
            if (novtx != lastNoVtx) {
                glEnd();
                if (novtx == 3) {
                    glBegin(GL_TRIANGLES);
                } else if (novtx == 4) {
                    glBegin(GL_QUADS);
                } else {
                    glBegin(GL_POLYGON);
                }
            }

            EnoType e = (*it)->directEno(1, CCW);
            for (VnoType v = 0; v < (*it)->noVtx(); ++v, e = (*it)->nextEno(e)) {
                glNormal3fv((*it)->normal((*it)->headVno(e)));
                glVertex3fv(((*it)->headVert(e))->getPos(it.depth()));
            }

            if ((novtx == 3) || (novtx == 4)) {
                lastNoVtx = novtx;
            } else {
                lastNoVtx = 0;
            }
        }
    }
    glEnd();
}
