// -*- Mode: c++ -*-
// $Id: triangleiter.cpp,v 1.1 2000/05/23 01:36:31 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/src/triangleiter.cpp,v $
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

#include "subtri.h"
#include "triangle.h"

#include "mesh.h"
#include "trimesh.h"

TriangleIter::TriangleIter(TriIter ti) 
{ _ti = new TriIter(ti); }

TriangleIter::TriangleIter() 
{ _ti = new TriIter(); }

TriangleIter::~TriangleIter() 
{ delete _ti; }


Triangle TriangleIter::operator*() { 
  assert(_ti);  
  return Triangle(**_ti);
}

TriangleIter& TriangleIter::operator++() { 
  assert(_ti);
  ++(*_ti);
  return *this;
}

TriangleIter& TriangleIter::operator=(const TriangleIter& i) {
  assert(_ti); assert(i._ti);
  *_ti = *(i._ti);
  return *this;
}

bool TriangleIter::operator==(const TriangleIter& i) const {
  assert(_ti); assert(i._ti);
  return (*_ti) == (*i._ti);
}

bool TriangleIter::operator!=(const TriangleIter& i) const {
  assert(_ti); assert(i._ti);
  return (*_ti) != (*i._ti);
}

int TriangleIter::depth() const {
  assert(_ti);
  return _ti->depth();
}

int TriangleIter::maxDepth() const {
  assert(_ti);
  return _ti->maxDepth(); 
}

