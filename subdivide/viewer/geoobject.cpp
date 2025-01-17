// -*- Mode: c++ -*-
// $Id: geoobject.cpp,v 1.3 2000/04/29 12:29:27 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/viewer/geoobject.cpp,v $

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

#include<GL/glut.h>
#include "geoobject.h"

GeoObject::GeoObject(float d) : _d(d) { ; }

cvec3f GeoObject::centerPoint() const 
{ return 0.5 * (minPoint() + maxPoint()); }

cvec3f GeoObject::minPoint() const 
{ return cvec3f(-_d, -_d, -_d); }

cvec3f GeoObject::maxPoint() const 
{ return cvec3f(_d, _d, _d); }

void GeoObject::render() 
{
  glutSolidTeapot(_d); 
}



