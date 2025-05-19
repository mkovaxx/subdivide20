// -*- Mode: c++ -*-
// $Id: viewer.hpp,v 1.2 2000/04/30 06:06:28 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/viewer.hpp,v $

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

#ifndef __VIEWER_H__
#define __VIEWER_H__

#include <GLFW/glfw3.h>
#include "camera.hpp"
#include "compat.hpp"
#include <vector>

class GeoObject;

class Viewer {
  public:
    Viewer(char* name = 0, int w = 512, int h = 512);
    virtual ~Viewer();

    // viewed object
    virtual void setObject(GeoObject* geoObject);
    GeoObject* getObject() const { return _geoObject; }

    // camera
    Camera* getCamera() { return _camera; }

    // window management
    GLFWwindow* getWindow() const { return _window; }
    int getWidth() { return _width; }
    int getHeight() { return _height; }
    void setPos(int x, int y);
    void setWindow();

    static void initGL(int* argc, char** argv);

    virtual void display();

  protected:
    virtual void reshape(int w, int h);
    virtual void mouse(int button, int action, int x, int y, int mods);
    virtual void motion(int x, int y);
    virtual void key(unsigned char k, int x, int y);
    virtual void specialKey(int k, int x, int y);

  private:
    // window data
    GLFWwindow* _window;
    int _width;
    int _height;
    char _title[80];

    // user interface components
    Camera* _camera;

    // the object to view
    GeoObject* _geoObject;

    static Viewer* getCurrentViewer(GLFWwindow* window);
    // static callbacks to be used with GLFW
    static void reshapeWrapper(GLFWwindow* window, int width, int height);
    static void mouseWrapper(GLFWwindow* window, int button, int action, int mods);
    static void motionWrapper(GLFWwindow* window, double xpos, double ypos);
    static void keyWrapper(GLFWwindow* window, unsigned int codepoint);
    static void specialKeyWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void errorWrapper(int error, const char* description);

    // helper function to transform screen coordinates to pixel coordinates, which may differ in GLFW
    void transformScreenToPixelCoords(double x_screen, double y_screen, int& x_pixel, int& y_pixel);

  protected:
    void positionCamera();
};

#endif /* __VIEWER_H__ */
