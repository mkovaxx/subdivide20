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

static void spositionCamera(Camera* camera, GeoObject* object, int* vp);

Viewer::Viewer(char* t, int w, int h) : _camera(0), _geoObject(0) {
    strcpy(_title, t);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // The following does not work on macOS, which is why it is commented out.
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    _window = glfwCreateWindow(w, h, _title, NULL, NULL);
    if (!_window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE); 
    }

    glfwSetWindowUserPointer(_window, this); // Associate this Viewer instance with the window
    glfwMakeContextCurrent(_window);

    // Register GLFW callbacks for this window
    glfwSetFramebufferSizeCallback(_window, Viewer::reshapeWrapper);
    glfwSetMouseButtonCallback(_window, Viewer::mouseWrapper);
    glfwSetCursorPosCallback(_window, Viewer::motionWrapper);
    glfwSetKeyCallback(_window, Viewer::specialKeyWrapper);
    glfwSetCharCallback(_window, Viewer::keyWrapper);

    _camera = new Camera();
    glCheck();

    glfwGetFramebufferSize(_window, &_width, &_height);
    reshape(_width, _height);
}

void Viewer::initGL(int* argc, char** argv) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback(Viewer::errorWrapper);
}

Viewer::~Viewer() { 
    glfwDestroyWindow(_window);
    _window = nullptr;
}

void Viewer::setObject(GeoObject* object) {
    _geoObject = object;
    int vp[4] = {0, 0, _width, _height};
    spositionCamera(_camera, object, vp);
}

void Viewer::setPos(int x, int y) {
    glfwSetWindowPos(getWindow(), x, y);
    glfwPostEmptyEvent();
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
    glCheck();
}

void Viewer::mouse(int button, int state, int x, int y, int mods) {
    glCheck();
}

void Viewer::motion(int x, int y) {
    glCheck();
}

void Viewer::key(unsigned char key, int x, int y) {
    glCheck();
}

void Viewer::specialKey(int key, int x, int y) {
    glCheck();
}

void Viewer::positionCamera() {
    int vp[4] = {0, 0, _width, _height};
    spositionCamera(getCamera(), getObject(), vp);
}

void Viewer::setWindow() {
    glfwMakeContextCurrent(getWindow());
    glfwPostEmptyEvent();
}

Viewer* Viewer::getCurrentViewer(GLFWwindow* window) {
    return static_cast<Viewer*>(glfwGetWindowUserPointer(window));
}

void Viewer::reshapeWrapper(GLFWwindow* window, int width, int height) {
    Viewer* v = getCurrentViewer(window);
    if (v) {
        v->reshape(width, height);
    }
}

void Viewer::mouseWrapper(GLFWwindow* window, int button, int action, int mods) {
    Viewer* v = getCurrentViewer(window);
    if (v) {
        double xpos_screen_double, ypos_screen_double;
        glfwGetCursorPos(window, &xpos_screen_double, &ypos_screen_double);

        int xpos_pixel, ypos_pixel;
        v->transformScreenToPixelCoords(xpos_screen_double, ypos_screen_double, xpos_pixel, ypos_pixel);

        v->mouse(button, action, xpos_pixel, ypos_pixel, mods);
    }
}

void Viewer::motionWrapper(GLFWwindow* window, double xpos_screen_double, double ypos_screen_double) {
    Viewer* v = getCurrentViewer(window);
    if (v) {
        int xpos_pixel, ypos_pixel;
        v->transformScreenToPixelCoords(xpos_screen_double, ypos_screen_double, xpos_pixel, ypos_pixel);
        v->motion(xpos_pixel, ypos_pixel);
    }
}

void Viewer::keyWrapper(GLFWwindow* window, unsigned int codepoint) {
    Viewer* v = getCurrentViewer(window);
    if (v) {
        double xpos_screen_double, ypos_screen_double;
        glfwGetCursorPos(window, &xpos_screen_double, &ypos_screen_double);

        int xpos_pixel, ypos_pixel;
        v->transformScreenToPixelCoords(xpos_screen_double, ypos_screen_double, xpos_pixel, ypos_pixel);

        v->key(tolower(codepoint), xpos_pixel, ypos_pixel);
    }
}

void Viewer::specialKeyWrapper(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Viewer* v = getCurrentViewer(window);
    if (v) {
        double xpos_screen_double, ypos_screen_double;
        glfwGetCursorPos(window, &xpos_screen_double, &ypos_screen_double);

        int xpos_pixel, ypos_pixel;
        v->transformScreenToPixelCoords(xpos_screen_double, ypos_screen_double, xpos_pixel, ypos_pixel);

        v->specialKey(key, xpos_pixel, ypos_pixel);
    }
}

void Viewer::errorWrapper(int error, const char* description) {
    fprintf(stderr, "GLFW Error [%d]: %s\n", error, description);
}

void Viewer::transformScreenToPixelCoords(double x_screen, double y_screen, int& x_pixel, int& y_pixel) {
    int window_width_screen, window_height_screen;
    glfwGetWindowSize(getWindow(), &window_width_screen, &window_height_screen);

    // Initialize pixel coordinates to screen coordinates as a fallback
    double xpos_pixel_double = x_screen;
    double ypos_pixel_double = y_screen;

    // Perform scaling if window dimensions are valid
    if (window_width_screen > 0 && window_height_screen > 0) {
        double scaleX = (double)getWidth() / window_width_screen;
        double scaleY = (double)getHeight() / window_height_screen;
        xpos_pixel_double = x_screen * scaleX;
        ypos_pixel_double = y_screen * scaleY;
    }

    x_pixel = static_cast<int>(xpos_pixel_double);
    y_pixel = static_cast<int>(ypos_pixel_double);
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
