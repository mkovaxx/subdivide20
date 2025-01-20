#ifndef __GENERAL_H__
#define __GENERAL_H__

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

#define GEN_MAX_DEPTH 8 /* maximum subdivision depth */

typedef signed char EnoType;              // edges are signed
typedef unsigned char VnoType;            // vertices
typedef unsigned char CnoType;            // child numbers
typedef enum { CW, CCW } OrientationType; // triangle orientation

inline OrientationType reverseOrientation(OrientationType o) { return (o == CW) ? CCW : CW; }

#endif /* __GENERAL_H__ */
