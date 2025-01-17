// -*- Mode: c++ -*-
// $Id: subdivide.h,v 1.5 2000/04/29 09:27:20 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/subdivide.h,v $

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

#ifndef __SUBDIVIDE_H__
#define __SUBDIVIDE_H__

#include "compat.h"



// Subdivider: a set of functions to apply subdivision rules
// to a ring

template<class RuleTable, class FaceRing>
class SubdivideTp {
public:

  typedef typename FaceRing::FaceType FaceType;

  static bool hdRelevantToMidpoint(FaceType* f, EnoType e);
  void getHdRuleAndRing(FaceType* f, EnoType e, int depth);

  cvec3f computeVertexPoint() const;
  cvec3f computeEdgePoint() const;
  cvec3f computeNormal() const;
  cvec3f computeLimit() const;

  const typename RuleTable::RuleType* rule() { return _rule; }
  const FaceRing& ring() const { return _faceRing; }
  cvec3f modifyFlatness(const cvec3f& p, float x1, float x2) const;
  cvec3f modifyClosedNormal(const cvec3f& p, float x1, float x2) const;
  cvec3f modifyOpenNormal(const cvec3f& p, float x1, float x2) const;
  void printRelevance() const;

private:
  int _depth;
  typename RuleTable::RuleType* _rule;
  FaceRing _faceRing;
};

#include "subdivide.hi"

#endif /* __SUBDIVIDE_H__ */
