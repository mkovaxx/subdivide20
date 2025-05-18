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

#include "compat.hpp"
#include "glcheck.hpp"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "geoobject.hpp"
#include "viewer.hpp"

#include <GLFW/glfw3.h>

std::vector<Viewer*> Viewer::_viewer;

static void spositionCamera(Camera* camera, GeoObject* object, int* vp);

Viewer::Viewer(char* t, int w, int h) : _width(w), _height(h), _camera(0), _geoObject(0) {

    if (t == 0) {
        sprintf(_title, "Viewer#%d", _viewer.size());
    } else {
        strcpy(_title, t);
    }

    _viewer.push_back(this);

    // GLFW Window Hints (replaces glutInitDisplayMode)
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE); // Corresponds to GLUT_DOUBLE
    glfwWindowHint(GLFW_DEPTH_BITS, 24);         // Corresponds to GLUT_DEPTH (assuming 24-bit)
                                                 // GLUT_RGBA is default

    _window = glfwCreateWindow(_width, _height, _title, NULL, NULL);
    if (!_window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        // Consider a more robust error handling mechanism, perhaps throwing an exception
        exit(EXIT_FAILURE); 
    }

    glfwMakeContextCurrent(_window);
    // If a library like GLEW were used, glewInit() would go here.

    // TODO: GLFW Migration - Replace these GLUT calls with GLFW equivalents
    // glutSetWindow(getWindow());
    // glutDisplayFunc(displayWrapper);
    // glutMouseFunc(mouseWrapper);
    // glutMotionFunc(motionWrapper);
    // glutReshapeFunc(reshapeWrapper);
    // glutKeyboardFunc(keyWrapper);
    // glutSpecialFunc(specialKeyWrapper);
    glCheck();

    _camera = new Camera();
    glCheck();
}

void Viewer::initGL(int* /*argc*/, char** /*argv*/) { // argc and argv often not directly used by glfwInit
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        // Consider a more robust error handling mechanism
        exit(EXIT_FAILURE);
    }
    // glutInitDisplayMode removed, handled by glfwWindowHint in constructor
}

Viewer::~Viewer() { 
    // If this viewer owned the window, it might be destroyed here, 
    // but typically window destruction and glfwTerminate are handled at global application exit.
    // For now, do nothing specific for GLFW here. 
    // glfwDestroyWindow(_window); // This might be premature if other viewers exist or app isn't closing
}

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
    // TODO: GLFW Migration - Replace with GLFW equivalent
    // glutSetWindow(getWindow());
    // glutPositionWindow(x, y);
    // glutPostRedisplay();
    glfwSetWindowPos(getWindow(), x, y);
    // glfwPostEmptyEvent(); // Or manage redraw flag for GLFW loop
    glCheck();
}

void Viewer::display() {

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCheck();

    if (_geoObject) {
        _geoObject->render();
    }
    glCheck();

    // glutSwapBuffers(); // Will be replaced by glfwSwapBuffers(getWindow()) in main loop
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
    // TODO: GLFW Migration - glutPostRedisplay equivalent needed if not rendering every frame
    // glutPostRedisplay(); 
    glCheck();
}

void Viewer::mouse(int, int, int, int) {
    // TODO: GLFW Migration - glutPostRedisplay equivalent needed
    // glutPostRedisplay();
    glCheck();
}

void Viewer::motion(int, int) {
    // TODO: GLFW Migration - glutPostRedisplay equivalent needed
    // glutPostRedisplay();
    glCheck();
}

void Viewer::key(unsigned char, int, int) {
    // TODO: GLFW Migration - glutPostRedisplay equivalent needed
    // glutPostRedisplay();
    glCheck();
}

void Viewer::specialKey(int, int, int) {
    // TODO: GLFW Migration - glutPostRedisplay equivalent needed
    // glutPostRedisplay();
    glCheck();
}

void Viewer::redisplayAll() {
    for (uint i = 0; i < _viewer.size(); ++i) {
        // TODO: GLFW Migration - Replace with GLFW equivalent or new logic
        // glutSetWindow(_viewer[i]->getWindow());
        // glutPostRedisplay();
        // For GLFW, redisplay logic will likely be a flag checked in the main loop for each window
        // or calling glfwPostEmptyEvent() if a redraw is needed outside direct event handling.
    }
    glCheck();
}

void Viewer::positionCamera() {
    int vp[4] = {0, 0, _width, _height};
    spositionCamera(getCamera(), getObject(), vp);
}

void Viewer::setWindow() {
    // TODO: GLFW Migration - Replace with GLFW equivalent
    // glutSetWindow(getWindow());
    // glutPostRedisplay();
    glfwMakeContextCurrent(getWindow()); // Make this window's context current
    // glfwPostEmptyEvent(); // Or manage redraw flag
}

//--------------------------------------------------------------
// dispatcher

Viewer* Viewer::getCurrentViewer() {
    // TODO: GLFW Migration - This logic is GLUT-specific (glutGetWindow).
    // GLFW's callbacks provide the GLFWwindow* directly. 
    // This function might become obsolete or need a different way to associate GLFWwindow* with Viewer instance.
    // For now, it will likely not work as expected or be needed in a typical GLFW event model.
    /*
    int id = glutGetWindow(); // This is the problematic GLUT call
    for (uint i = 0; i < Viewer::_viewer.size(); ++i) {
        // if (_viewer[i]->getWindow() == id) { // This comparison is also problematic (GLFWwindow* vs int)
        //     return _viewer[i];
        // }
    }
    */
    // Hacky temporary: return first viewer, or null. This needs a proper fix based on how events are handled.
    if (!_viewer.empty()) return _viewer[0];
    return 0;
}

void Viewer::displayWrapper() {
    Viewer* v = getCurrentViewer();
    if (v) {
        v->display();
    }
}

void Viewer::reshapeWrapper(int x, int y) {
    Viewer* v = getCurrentViewer();
    if (v) {
        v->reshape(x, y);
    }
}

void Viewer::mouseWrapper(int button, int state, int x, int y) {
    Viewer* v = getCurrentViewer();
    if (v) {
        v->mouse(button, state, x, y);
    }
}

void Viewer::motionWrapper(int x, int y) {
    Viewer* v = getCurrentViewer();
    if (v) {
        v->motion(x, y);
    }
}

void Viewer::keyWrapper(unsigned char k, int x, int y) {
    Viewer* v = getCurrentViewer();
    if (v) {
        v->key(k, x, y);
    }
}

void Viewer::specialKeyWrapper(int k, int x, int y) {
    Viewer* v = getCurrentViewer();
    if (v) {
        v->specialKey(k, x, y);
    }
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
