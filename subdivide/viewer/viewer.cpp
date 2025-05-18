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

    // GLFW Window Hints (replaces glutInitDisplayMode)
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // The following does not work on macOS, which is why it is commented out.
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE); // Corresponds to GLUT_DOUBLE
    glfwWindowHint(GLFW_DEPTH_BITS, 24);         // Corresponds to GLUT_DEPTH (assuming 24-bit)
                                                 // GLUT_RGBA is default

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
    glfwSetFramebufferSizeCallback(_window, Viewer::glfw_framebuffer_size_callback);
    glfwSetMouseButtonCallback(_window, Viewer::glfw_mouse_button_callback);
    glfwSetCursorPosCallback(_window, Viewer::glfw_cursor_position_callback);
    glfwSetKeyCallback(_window, Viewer::glfw_key_callback);
    // Optional: glfwSetCharCallback(_window, Viewer::glfw_char_callback);
    // Optional: glfwSetScrollCallback(_window, Viewer::glfw_scroll_callback);

    // Enable depth testing by default
    glEnable(GL_DEPTH_TEST);

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

void Viewer::initGL(int* argc, char** argv) { // argc and argv often not directly used by glfwInit
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback(Viewer::glfw_error_callback); // Set global error callback

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

//--------------------------------------------------------------
// GLFW static callback implementations

void Viewer::glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error [%d]: %s\n", error, description);
}

void Viewer::glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    if (viewer) {
        viewer->reshape(width, height);
    }
}

void Viewer::glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    if (viewer) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        // TODO: Map GLFW button/action/mods to what Viewer::mouse expects if different from GLUT.
        // For now, passing them directly. Viewer::mouse expects 'state' (press/release)
        // GLFW 'action' is GLFW_PRESS or GLFW_RELEASE, which might map directly.
        viewer->mouse(button, action, static_cast<int>(xpos), static_cast<int>(ypos));
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
    if (viewer) {
        // TODO: Reconcile parameters. Viewer::key expects unsigned char, Viewer::specialKey expects int.
        // GLFW 'key' covers both. 'action' is press/release/repeat.
        // Mouse coordinates x, y are not available here. Passing 0,0 as placeholders.
        // This logic will need refinement based on how key/specialKey are used.
        
        // A simple heuristic: if key is an ASCII value, treat as 'char'. This is imperfect.
        // A more robust approach would be to use glfwSetCharCallback for text input.
        if (key >= 0 && key <= 255 && (action == GLFW_PRESS || action == GLFW_REPEAT)) { // Crude check for printable ASCII for 'key'
             // Only pass on press/repeat for regular keys, consistent with typical char input
            viewer->key(static_cast<unsigned char>(key), 0, 0); 
        }
        // For special keys (arrows, F-keys, etc.) or any key action for specialKey handling
        // We can pass all key events to specialKey and let it filter, or add more logic here.
        // For now, let's assume specialKey handles non-char keys or all keys if viewer->key isn't appropriate.
        // This is a placeholder and likely needs more specific logic. 
        // Viewer::specialKey might be intended for non-ASCII keys or when specific GLFW_KEY_ values are used.
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
             viewer->specialKey(key, 0, 0); // Passing GLFW_KEY_XXX values
        }
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
