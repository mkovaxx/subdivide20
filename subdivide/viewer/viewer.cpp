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

Viewer::Viewer(char* t, int w, int h) : _width(w), _height(h), _camera(0), _geoObject(0) {
    strcpy(_title, t);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // The following does not work on macOS, which is why it is commented out.
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    _window = glfwCreateWindow(_width, _height, _title, NULL, NULL);
    if (!_window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE); 
    }

    glfwSetWindowUserPointer(_window, this); // Associate this Viewer instance with the window

    glfwMakeContextCurrent(_window);
    // If a library like GLEW were used, glewInit() would go here.

    // Register GLFW callbacks for this window
    glfwSetFramebufferSizeCallback(_window, Viewer::reshapeWrapper);
    glfwSetMouseButtonCallback(_window, Viewer::mouseWrapper);
    glfwSetCursorPosCallback(_window, Viewer::motionWrapper);
    glfwSetKeyCallback(_window, Viewer::specialKeyWapper);
    glfwSetCharCallback(_window, Viewer::keyWrapper);

    // Enable depth testing by default
    glEnable(GL_DEPTH_TEST);

    glCheck();

    _camera = new Camera();
    glCheck();
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
    glfwSetWindowPos(getWindow(), x, y);
    glfwPostEmptyEvent();
    glCheck();
}

void Viewer::display() {

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCheck();

    if (_camera) { // Ensure camera exists and its matrices are loaded
        _camera->loadMatrices(); 
    }

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
    // TODO: GLFW Migration - Replace with GLFW equivalent
    // glutSetWindow(getWindow());
    // glutPostRedisplay();
    glfwMakeContextCurrent(getWindow()); // Make this window's context current
    // glfwPostEmptyEvent(); // Or manage redraw flag
}

void Viewer::runEventLoop() {
    if (!_window) {
        fprintf(stderr, "Error: Viewer window not initialized before runEventLoop()\n");
        return;
    }

    // Ensure this viewer's context is current for the loop operations
    // Although displayWrapper should also handle this for its specific drawing call.
    glfwMakeContextCurrent(_window);

    while (!glfwWindowShouldClose(_window)) {
        this->display(); // Calls the virtual display() of this viewer instance directly

        glfwSwapBuffers(_window); // Swap front and back buffers
        glfwPollEvents();        // Poll for and process events
    }
}

void Viewer::errorWrapper(int error, const char* description) {
    fprintf(stderr, "GLFW Error [%d]: %s\n", error, description);
}

void Viewer:: reshapeWrapper(GLFWwindow* window, int width, int height) {
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    if (viewer) {
        viewer->reshape(width, height);
    }
}

void Viewer:: mouseWrapper(GLFWwindow* window, int button, int action, int mods) {
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    if (viewer) {
        double xpos_double, ypos_double;
        glfwGetCursorPos(window, &xpos_double, &ypos_double);
        int xpos = static_cast<int>(xpos_double);
        int ypos = static_cast<int>(ypos_double);

        int glut_button = -1;
        if (button == GLFW_MOUSE_BUTTON_LEFT) glut_button = 0; // GLUT_LEFT_BUTTON
        else if (button == GLFW_MOUSE_BUTTON_MIDDLE) glut_button = 1; // GLUT_MIDDLE_BUTTON
        else if (button == GLFW_MOUSE_BUTTON_RIGHT) glut_button = 2; // GLUT_RIGHT_BUTTON

        int glut_state = -1;
        if (action == GLFW_PRESS) glut_state = 0; // GLUT_DOWN
        else if (action == GLFW_RELEASE) glut_state = 1; // GLUT_UP

        if (glut_button != -1 && glut_state != -1) {
            viewer->mouse(glut_button, glut_state, xpos, ypos, mods);
        }
    }
}

void Viewer::glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    if (viewer) {
        viewer->motion(static_cast<int>(xpos), static_cast<int>(ypos));
    }
}

void Viewer::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    if (!viewer) return;

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        double xpos_double, ypos_double;
        glfwGetCursorPos(window, &xpos_double, &ypos_double);
        int x = static_cast<int>(xpos_double);
        int y = static_cast<int>(ypos_double);

        // Special keys (function keys, arrows, navigation, Esc, Enter, Tab, etc.)
        // GLFW key codes for these are typically >= GLFW_KEY_ESCAPE (256)
        if (key >= GLFW_KEY_ESCAPE) {
            viewer->specialKey(key, x, y); // Pass GLFW key code directly
        } 
        // Character-like keys (letters, numbers, symbols, space)
        // GLFW key codes for these are typically < 256 and often map to ASCII values.
        // We filter out GLFW_KEY_UNKNOWN (-1) and other negative values if any.
        else if (key >= 0 && key < GLFW_KEY_ESCAPE) {
            // Note: This will pass GLFW_KEY_A (65) as 'A'. 
            // If Viewer::key expects lowercase unless Shift is pressed, 
            // 'mods' would need to be checked here to adjust the character case.
            // For now, direct mapping is simpler.
            viewer->key(static_cast<unsigned char>(key), x, y);
        }
        // Keys like GLFW_KEY_UNKNOWN (-1) will be ignored by this logic.
    }
}

void Viewer::initGL(int* argc, char** argv) { // argc and argv often not directly used by glfwInit
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback(Viewer::errorWrapper);

    // glutInitDisplayMode removed, handled by glfwWindowHint in constructor
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
