// -*- Mode: c++ -*-
// $Id: pickableparam.hpp,v 1.1 2000/04/29 12:29:27 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/viewer/pickableparam.hpp,v $

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

#ifndef __PICKABLEPARAM_H__
#define __PICKABLEPARAM_H__

class PickableParam {
  public:
    static const float vertexColor[3];
    static const float creaseVertexColor[3];
    static const float cornerVertexColor[3];

    static const float normalColor[3];
    static const float edgeColor[3];
    static const float creaseEdgeColor[3];

    // sector colors have an alpha value, as they are blended with the
    // background to create a transparent effect.
    static const float sectorColor[4];
    static const float convexSectorColor[4];
    static const float concaveSectorColor[4];

    // color for selected things
    static const float pickColor[3];
};

#endif /* __PICKABLEPARAM_H__ */
