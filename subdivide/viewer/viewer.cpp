// -*- Mode: c++ -*-
// $Id: viewer.cpp,v 1.4 2000/04/30 06:06:28 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/viewer/viewer.cpp,v $

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

#include "compat.h"
#include "glcheck.h"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "geoobject.h"
#include "viewer.h"

std::vector<Viewer*> Viewer::_viewer;

static void spositionCamera(Camera* camera, GeoObject* object, int* vp);

Viewer::Viewer(char* t, int w, int h) : _width(w), _height(h), _camera(0), _geoObject(0) {

    if (t == 0)
        sprintf(_title, "Viewer#%d", _viewer.size());
    else
        strcpy(_title, t);

    _viewer.push_back(this);

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(_width, _height);

    _winId = glutCreateWindow(_title);

    glutSetWindow(getId());
    glutDisplayFunc(displayWrapper);
    glutMouseFunc(mouseWrapper);
    glutMotionFunc(motionWrapper);
    glutReshapeFunc(reshapeWrapper);
    glutKeyboardFunc(keyWrapper);
    glutSpecialFunc(specialKeyWrapper);
    glCheck();

    _camera = new Camera();
    glCheck();
}

void Viewer::initGL(int* argc, char** argv) {
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
}

Viewer::~Viewer() { ; }

void Viewer::setObject(GeoObject* object) {
    _geoObject = object;
    int vp[4] = {0, 0, _width, _height};
    spositionCamera(_camera, object, vp);
}

void Viewer::setSize(int w, int h) {
    reshape(w, h);
    glCheck();
}

void Viewer::setPos(int x, int y) {
    glutSetWindow(getId());
    glutPositionWindow(x, y);
    glutPostRedisplay();
    glCheck();
}

void Viewer::display() {

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCheck();

    if (_geoObject)
        _geoObject->render();
    glCheck();

    glutSwapBuffers();
    glCheck();
}

void Viewer::reshape(int w, int h) {
    _width = w;
    _height = h;
    int vp[4] = {0, 0, w, h};
    glViewport(0, 0, w, h);
    _camera->setViewport(vp);
    _camera->computeModelview();
    _camera->computeProjection();
    _camera->loadMatrices();
    glutPostRedisplay();
    glCheck();
}

void Viewer::mouse(int, int, int, int) {
    glutPostRedisplay();
    glCheck();
}

void Viewer::motion(int, int) {
    glutPostRedisplay();
    glCheck();
}

void Viewer::key(unsigned char, int, int) {
    glutPostRedisplay();
    glCheck();
}

void Viewer::specialKey(int, int, int) {
    glutPostRedisplay();
    glCheck();
}

void Viewer::redisplayAll() {
    for (uint i = 0; i < _viewer.size(); ++i) {
        glutSetWindow(_viewer[i]->getId());
        glutPostRedisplay();
    }
    glCheck();
}

void Viewer::positionCamera() {
    int vp[4] = {0, 0, _width, _height};
    spositionCamera(getCamera(), getObject(), vp);
}

void Viewer::setWindow() {
    glutSetWindow(getId());
    glutPostRedisplay();
}

//--------------------------------------------------------------
// dispatcher

Viewer* Viewer::getCurrentViewer() {
    int id = glutGetWindow();
    for (uint i = 0; i < Viewer::_viewer.size(); ++i)
        if (_viewer[i]->getId() == id)
            return _viewer[i];
    return 0;
}

void Viewer::displayWrapper() {
    Viewer* v = getCurrentViewer();
    if (v)
        v->display();
}

void Viewer::reshapeWrapper(int x, int y) {
    Viewer* v = getCurrentViewer();
    if (v)
        v->reshape(x, y);
}

void Viewer::mouseWrapper(int button, int state, int x, int y) {
    Viewer* v = getCurrentViewer();
    if (v)
        v->mouse(button, state, x, y);
}

void Viewer::motionWrapper(int x, int y) {
    Viewer* v = getCurrentViewer();
    if (v)
        v->motion(x, y);
}

void Viewer::keyWrapper(unsigned char k, int x, int y) {
    Viewer* v = getCurrentViewer();
    if (v)
        v->key(k, x, y);
}

void Viewer::specialKeyWrapper(int k, int x, int y) {
    Viewer* v = getCurrentViewer();
    if (v)
        v->specialKey(k, x, y);
}

void spositionCamera(Camera* camera, GeoObject* object, int* vp) {
    glCheck();
    cvec3f _mi = object->minPoint();
    cvec3f _ma = object->maxPoint();

    glCheck();
    camera->reset();

    glCheck();
    camera->setViewport(vp);
    cvec3f mid = 0.5f * (_mi + _ma);

    float f = std::max(_ma.x() - _mi.x(), _ma.y() - _mi.y()) / 2.0f / tan(0.5f * camera->fovy() * M_PI / 180.0f);

    cvec3f trans(mid.x(), mid.y(), _ma.z() + 2 * f);

    glCheck();
    camera->setPerspectiveParams(camera->fovy(),
                                 0.1f, // 0.25f * (trans.z() - _mi.z()),
                                 1.5f * (trans.z() - _ma.z()));

    glCheck();
    camera->translate(-trans);
    camera->computeModelview();
    camera->computeProjection();
    camera->loadMatrices();
    glCheck();
}
