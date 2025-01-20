// -*- Mode: c++ -*-
// $Id: geoobject.h,v 1.2 2000/04/29 12:29:26 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/geoobject.h,v $

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

#ifndef __GEOOBJECT_H__
#define __GEOOBJECT_H__

#include "compat.h"
#include "cvec3t.h"
#include <GL/gl.h>

class GeoObject {
  public:
    GeoObject(float d = 1.0);
    virtual void render();
    virtual void rerender() { render(); }
    virtual void pick(GLint, GLint) {}
    virtual cvec3f centerPoint() const;
    virtual cvec3f minPoint() const;
    virtual cvec3f maxPoint() const;
    virtual void write(char*) const {}

  private:
    float _d;
};

#endif /* __GEOOBJECT_H__ */
