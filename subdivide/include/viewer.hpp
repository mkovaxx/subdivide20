// -*- Mode: c++ -*-
// $Id: viewer.h,v 1.2 2000/04/30 06:06:28 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/viewer.h,v $

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

#include "camera.h"
#include "compat.h"
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
    int getId() const { return _winId; }
    int getWidth() { return _width; }
    int getHeight() { return _height; }
    void setSize(int w, int h);
    void setPos(int x, int y);
    void setWindow();

    static void initGL(int* argc, char** argv);

  protected:
    virtual void display();
    virtual void reshape(int w, int h);
    virtual void mouse(int button, int state, int x, int y);
    virtual void motion(int x, int y);
    virtual void key(unsigned char k, int x, int y);
    virtual void specialKey(int k, int x, int y);

  private:
    // window data
    int _winId;
    int _width;
    int _height;
    char _title[80];

    // user interface components
    Camera* _camera;

    // the object to view
    GeoObject* _geoObject;

    // some wrapper functions
    static Viewer* getCurrentViewer();
    static void displayWrapper();
    static void reshapeWrapper(int x, int y);
    static void mouseWrapper(int button, int state, int x, int y);
    static void motionWrapper(int x, int y);
    static void keyWrapper(unsigned char k, int x, int y);
    static void specialKeyWrapper(int k, int x, int y);
    static std::vector<Viewer*> _viewer;

  protected:
    void positionCamera();

  public:
    static void redisplayAll();
};

#endif /* __VIEWER_H__ */
