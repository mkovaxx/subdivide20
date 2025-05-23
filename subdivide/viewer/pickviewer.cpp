// -*- Mode: c++ -*-
// $Id: pickviewer.cpp,v 1.11 2001/02/07 15:12:50 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/viewer/pickviewer.cpp,v $

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

#include "pickviewer.hpp"
#include "geoobject.hpp"
#include "glcheck.hpp"
#include "math.h"
#include "pickobject.hpp"
#include "stdlib.h"
#include <GLFW/glfw3.h>

void PickViewer::mouse(int button, int action, int x, int y, int mods) {
    if ((_uiState == PICK_STATE) && (action == GLFW_PRESS)) {
        pick(x, getHeight() - y);
        // callback to mesh class
        if (_pickCB) {
            _pickCB(_pickedStuff, _pickData);
        }
    } else {
        BallViewer::mouse(button, action, x, y, mods);
    }
}

void PickViewer::key(unsigned char k, int x, int y) {
    std::map<unsigned char, CBPairType>::iterator it = _cbMap.find(k);
    if (it != _cbMap.end()) {
        ((*it).second.first)((*it).second.second);
    } else {
        BallViewer::key(k, x, y);
    }
}

void PickViewer::specialKey(int k, int x, int y) {
    std::map<int, CBPairType>::iterator it = _specialMap.find(k);
    if (it != _specialMap.end()) {
        ((*it).second.first)((*it).second.second);
    } else {
        BallViewer::specialKey(k, x, y);
    }
}

void PickViewer::pick(GLint x, GLint y) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glMultMatrixd((double*)HMatrix::Translation(_arcball.Position()));
    glMultMatrixd(_arcball.Rotation());
    glMultMatrixd((double*)HMatrix::Translation(-_arcball.Position()));

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    int bbits, rbits, gbits;
    glGetIntegerv(GL_RED_BITS, &rbits);
    glGetIntegerv(GL_GREEN_BITS, &gbits);
    glGetIntegerv(GL_BLUE_BITS, &bbits);
    int shift = 1;
    for (int i = 0; i < 8 - std::min(std::min(rbits, gbits), bbits); ++i) {
        shift *= 2;
    }

    if (getObject()) {
        unsigned char picks =
            PickedStuff::PICK_EDGE | PickedStuff::PICK_VERTEX | PickedStuff::PICK_SECTOR | PickedStuff::PICK_NORMAL;
        PickObject* o = dynamic_cast<PickObject*>(getObject());
        o->renderPick(picks, 0, shift);
        glFlush();

        GLbyte buff[10];
        glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, buff);
        glFlush();

        _pickedStuff = o->doPick(buff[0], buff[1], buff[2], picks, shift);
    }

    glPopMatrix();
    glCheck();
}
