// -*- Mode: c++ -*-
// $Id: pickedstuff.h,v 1.3 2000/04/30 07:35:46 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/pickedstuff.h,v $

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

#ifndef __PICKEDSTUFF_H__
#define __PICKEDSTUFF_H__

#include "compat.h"
#include "general.h"

// a class describing the result of a picking operation

class PickedStuff {
  public:
    typedef enum { PICK_NOTHING = 0, PICK_VERTEX = 1, PICK_EDGE = 2, PICK_SECTOR = 4, PICK_NORMAL = 8 } PickElementType;

    PickedStuff() : _pickType(PICK_NOTHING), _eno(0), _face(0) { ; }
    PickedStuff(PickElementType pickType, void* face, EnoType e) : _pickType(pickType), _eno(e), _face(face) { ; }

    PickElementType type() { return _pickType; }
    EnoType eno() { return _eno; }
    void* face() { return _face; }

  public:
    PickElementType _pickType;
    EnoType _eno;
    // hide internal representation
    void* _face;
};

#endif /* __PICKEDSTUFF_H__ */
