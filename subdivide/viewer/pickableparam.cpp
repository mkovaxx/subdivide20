// -*- Mode: c++ -*-
// $Id: pickableparam.cpp,v 1.3 2000/04/30 12:14:20 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/viewer/pickableparam.cpp,v $

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

#include "pickableparam.h"

const float PickableParam::normalColor[3] = {0.7f, 0.7f, 0.7f};
const float PickableParam::vertexColor[3] = {0.4f, 0.4f, 0.4f};
const float PickableParam::creaseVertexColor[3] = {0.0f, 0.0f, 1.0f};
const float PickableParam::cornerVertexColor[3] = {1.0f, 0.0f, 0.0f};

const float PickableParam::edgeColor[3] = {0.4f, 0.4f, 0.4f};
const float PickableParam::creaseEdgeColor[3] = {0.0f, 0.0f, 1.0f};

const float PickableParam::sectorColor[4] = {0.5f, 0.5f, 0.5f, 0.5f};
const float PickableParam::convexSectorColor[4] = {0.3f, 0.3f, 1.0f, 0.2f};
const float PickableParam::concaveSectorColor[4] = {1.0f, 0.3f, 0.0f, 0.2f};

const float PickableParam::pickColor[3] = {0.0f, 1.0f, 0.0f};
