// -*- Mode: c++ -*-
// $Id: pickobject.h,v 1.3 2000/04/30 03:32:38 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/pickobject.h,v $

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

#ifndef __PICKOBJECT_H__
#define __PICKOBJECT_H__

#include "compat.h"
#include "pickedstuff.h"

class PickObject {
  public:
    // prepare screen buffer for picking
    virtual void renderPick(unsigned char picks, unsigned int targetCnt, unsigned int shift) = 0;

    // determine hit
    virtual PickedStuff* doPick(unsigned char r, unsigned char g, unsigned char b, unsigned char picks,
                                unsigned int shift) = 0;
    static const unsigned char PICK_ALL;
};

#endif /* __PICKOBJECT_H__ */
