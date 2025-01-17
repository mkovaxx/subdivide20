// -*- Mode: c++ -*-
// $Id: baseface.h,v 1.3 2000/04/29 07:50:08 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/baseface.h,v $

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


#ifndef __BASEFACE_H__
#define __BASEFACE_H__

#include "compat.h"
#include "general.h"


// least common denominator for all face classes which 
// are nodes in polygon hierarchies
// contains pointers to the parent and children


template<class FaceParam, class TLFaceParam>
class BaseFaceTp {
public:
  typedef FaceParam Face;
  typedef TLFaceParam TLFace;
  
  BaseFaceTp() : _p(0), _no(0), _c(0), _orient(CCW) {}
  virtual ~BaseFaceTp() {}
  
protected:
  BaseFaceTp* _p;          // parent
  CnoType _no;             // my number among the children of my parent
  Face* _c;                // array of children
  OrientationType _orient; // clockwise/counterclockwise orientation

public:
  BaseFaceTp*& parentx() { return _p; }
  CnoType& nox() { return _no; }
  OrientationType& orient() { return _orient; }
};

#endif /* __BASEFACE_H__ */



