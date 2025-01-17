// -*- Mode: c++ -*-
// $Id: arcball.h,v 1.2 2000/04/30 07:35:46 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/arcball.h,v $

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

#ifndef __ARCBALL_H__
#define __ARCBALL_H__

#include "quat.h"
#include "hmatrix.h"

#define MAX_SET_SIZE 3 

//: Arcball rotation for UI
class ArcBall { 
public:

  enum AxisSet {NoAxes, WorldAxes, BodyAxes, NSets};

  enum AxesNames { X = 0,Y = 1 ,Z = 2};

  void Update();
  void Init();

  ArcBall() { Init(); }


  void SetRadius( double r ) { radius = r; } 
  double Radius() const { return radius; }
  void SetPosition( const CVec3T<float>& c) { center = c; 
          combined = HMatrix::Translation(center)*mNow;}
  const CVec3T<float>& Position() const { return center;  }

  void SetRotation( const HMatrix& rot ) { mNow = rot; 
          combined = HMatrix::Translation(center)*mNow;} 
  const HMatrix&  Rotation() { return mNow; }

  void PickingRay( const CVec3T<float>& rs, const CVec3T<float>& rd)
  { rayStartNow = rs; rayDirNow = rd; }
  
  void UseSet(AxisSet aset) { 
    if (!dragging) axisSet = aset; 
  }
  
  // appearance
  void SetInactiveColor(const CVec3T<float>& color) { inactiveColor = color;  }

  void SetActiveColor  (const CVec3T<float>& color) { activeColor = color;  }

  void SetDragColor    (const CVec3T<float>& color) { dragColor = color;  }

  void SetOutlineColor (const CVec3T<float>& color) { outlineColor = color;  }

  void SetResultColor  (const CVec3T<float>& color) { resultColor = color;  }


  // Begin drawing arc for all drags combined
  void ShowResult() { showResult = GL_TRUE; }
  // Stop drawing arc for all drags combined. 
  void HideResult() { showResult = GL_FALSE; }

  void BeginDrag();
  void EndDrag();
  void Draw(const CVec3T<float>& camerapos );
  void DrawCompleteBall(const CVec3T<float>& camerapos );

 private:
  CVec3T<float> RayOnSphere(const CVec3T<float>& ray_start, 
                            const CVec3T<float>& ray_dir);

  void DrawOuterRing(const CVec3T<float>& camerapos);
  void DrawConstraints();
  void DrawDragArc();
  void DrawResultArc();

  // data:
  double radius;

  // position  the world coord system
  CVec3T<float> center;
  
  //  combined transformation matrix
  HMatrix combined;

  Quat  qNow, qDown, qDrag;
  // current picking ray and the picking ray when drag was started
  CVec3T<float> rayStartNow, rayDirNow,rayStartDown, rayDirDown;
  CVec3T<float> vFrom, vTo, vrFrom, vrTo;
  HMatrix mNow, mDown;
  bool showResult, dragging;
  CVec3T<float> sets[NSets][MAX_SET_SIZE];
  int setSizes[NSets];
  AxisSet axisSet;
  int axisIndex;

  CVec3T<float> outlineColor;
  CVec3T<float> inactiveColor;
  CVec3T<float> activeColor;
  CVec3T<float> dragColor;
  CVec3T<float> resultColor;

};

#endif /* __ARCBALL_H__ */







