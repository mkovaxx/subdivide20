// -*- Mode: c++ -*-
// $Id: ballviewer.h,v 1.1 2000/04/29 12:29:26 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/ballviewer.h,v $

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

#ifndef __BALLVIEWER_H__
#define __BALLVIEWER_H__

#include "compat.h"
#include "viewer.h"
#include "arcball.h"

class UiAction;

class BallViewer : public Viewer {

public:
  BallViewer(char* name = 0, int w = 512, int h = 512);
  virtual ~BallViewer();
  virtual void setObject(GeoObject* geoObject);

  ArcBall* getArcBall() { return &_arcball; }
  
protected:
  UiAction* _uiAction;
  ArcBall _arcball;
  virtual void display();
  virtual void renderObject();
  virtual void mouse(int button, int state, int x, int y);
  virtual void motion(int x, int y);
  virtual void key(unsigned char k, int x, int y);
  virtual void specialKey(int k, int x, int y);

private:
  void setLightAndMaterial();
  void centerArcBall();
  void positionArcBall();
};

#endif /* __BALLVIEWER_H__ */
