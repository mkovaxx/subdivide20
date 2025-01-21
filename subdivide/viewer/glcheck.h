// -*- Mode: c++ -*-
// $Id: glcheck.h,v 1.2 2000/04/29 12:29:27 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/viewer/glcheck.h,v $

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

#ifndef __GL_CHECK_H__
#define __GL_CHECK_H__
#include "compat.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#define glCheck() __glCheck__(__FILE__, __LINE__)

inline int __glCheck__(char* fileName, int n) {
    int e = glGetError();
    if (e) {
        std::cerr << (char*)gluErrorString(e) << "  " << fileName << ", #" << n << std::endl;
    }
    return (e);
}

#endif /* __GL_CHECK_H__ */
