// -*- Mode: c++ -*-
// $Id: uiaction.h,v 1.3 2000/04/29 12:29:28 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/viewer/uiaction.h,v $

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

#ifndef __UIACTION_H__
#define __UIACTION_H__

#include "arcball.h"
#include "camera.h"

class UiAction {
public:
  virtual void update(int , int ) { ; }
  virtual ~UiAction() { ; }
};

class CameraRotateAction : public UiAction {
public:
  CameraRotateAction(int x, int y, ArcBall* arcball, Camera* camera) :
    _x(x), _y(y), _arcball(arcball), _camera(camera) { 

    cvec3f worldMousePos = _camera->unproject(cvec3f(x,y,0));
    _arcball->PickingRay(_camera->viewWorldPosition(), 
			(worldMousePos - _camera->viewWorldPosition()).dir());
    _arcball->BeginDrag();
    _arcball->Update();
  }

  virtual ~CameraRotateAction() {
    cvec3f cp = _arcball->Position();
    _camera->model() = _camera->model() * HMatrix::Translation(cp)
      * _arcball->Rotation() * HMatrix::Translation(-cp);

    _camera->computeModelview();
    _camera->loadMatrices();
    
    _arcball->SetRotation(HMatrix::Identity());
  }
  
  virtual void update(int x, int y) {

    cvec3f worldMousePos = _camera->unproject(cvec3f(x,y,0));
    _arcball->PickingRay(_camera->viewWorldPosition(), 
			(worldMousePos - _camera->viewWorldPosition()).dir());
    _arcball->Update();
  }

  
private:
  int _x, _y;
  ArcBall* _arcball;
  Camera* _camera;
};


class CameraTransXYAction : public UiAction {
public:
  
  CameraTransXYAction(int x, int y, ArcBall* arcball, Camera* camera) :
    _x(x), _y(y), _arcball(arcball), _camera(camera) { 
  }

  virtual ~CameraTransXYAction() {
  }

  virtual void update(int x, int y) {
    float z = _camera->project(_arcball->Position()).z();
    cvec3f p0 = _camera->unproject(cvec3f(_x, _y, z));
    cvec3f p1 = _camera->unproject(cvec3f(x, y, z));
    _camera->model() = _camera->model() * HMatrix::Translation(p1-p0);
    
    _camera->computeModelview();
    _camera->loadMatrices();

    _x = x;
    _y = y;
  }

private:
  int _x, _y;
  ArcBall* _arcball;
  Camera* _camera;
};


class CameraTransZAction : public UiAction {
public:
  
  CameraTransZAction(int x, int y, ArcBall* arcball, Camera* camera) :
    _x(x), _y(y), _arcball(arcball), _camera(camera) { 
  }

  virtual ~CameraTransZAction() {
  }

  virtual void update(int x, int y) {
    cvec3f p0 = _camera->unproject(cvec3f(x, _y, _camera->project(_arcball->Position()).z()));
    cvec3f p1 = _camera->unproject(cvec3f(x,  y, _camera->project(_arcball->Position()).z()));
    if (y<_y)
      _camera->model() = _camera->model() * 
	HMatrix::Translation((p0-p1).l2() * (_camera->viewWorldPosition()-_arcball->Position()).dir());
    else
      _camera->model() = _camera->model() * 
	HMatrix::Translation(-(p0-p1).l2() * (_camera->viewWorldPosition()-_arcball->Position()).dir());
    _x = x;
    _y = y;
    _camera->computeModelview();
    _camera->loadMatrices();
  }
  
private:
  int _x, _y;
  ArcBall* _arcball;
  Camera* _camera;
};


#endif /* __UIACTION_H__ */
