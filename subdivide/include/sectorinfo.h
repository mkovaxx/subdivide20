#ifndef __SECTORINFO_H__
#define __SECTORINFO_H__

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


#include "compat.h"
#include "cvec3t.h"

// A class to store information about sector tags
// 

class SectorInfo {
public:

  typedef enum { NOTAG_SECTOR, CONVEX_SECTOR, CONCAVE_SECTOR } SectorTagType;

  SectorInfo() : 
    _refCount(0),
    _sectorTag(NOTAG_SECTOR), 
    _normal(0),
    _modifiedNormal(0), 
    _modifiedNormalT(0),
    _modifiedFlatness(0),
    _theta(-1)
    { }

  virtual ~SectorInfo() {  }

  bool isDefault() const {
    return 
      (_sectorTag == NOTAG_SECTOR) &&
      (_modifiedNormalT == 0) && 
      (_modifiedFlatness == 0);
  }

  SectorTagType sectorTag() const { return _sectorTag; }
  void setSectorTag(SectorTagType tag) {
    _sectorTag = tag;
    switch(tag) {
    case CONVEX_SECTOR: 
      setTheta(2.0f* M_PI *1.0f/4.0f);
      setModifiedFlatness(0.0);
      break;
    case CONCAVE_SECTOR:
      setTheta(2.0f*M_PI*3.0f/4.0f);
      setModifiedFlatness(0.5);
      break;
    case NOTAG_SECTOR:
      setTheta(0);
      setModifiedFlatness(0.0);
    }
  }

  void print() const {
    cerr<<"SectorInfo = "<<(void*)this<<endl;
    cerr<<"\t_sectorTag = "<<(int)_sectorTag<<endl;
    cerr<<"\t_modifiedNormal = "<<_modifiedNormal<<endl;
    cerr<<"\t_modifiedNormalT = "<<_modifiedNormalT<<endl;
    cerr<<"\t_modifiedFlatness = "<<_modifiedFlatness<<endl;
    cerr<<"\t_theta = "<<_theta<<endl;
    cerr<<"\t_refCount = "<<_refCount<<endl;
  }

  const cvec3f& modifiedNormal() const { return _modifiedNormal; }
  float modifiedNormalT() const { return _modifiedNormalT; }
  float modifiedFlatness() const { return _modifiedFlatness; }
  float theta() const { return _theta; }

  bool hasModifiedNormal() const { return (_modifiedNormal.l1() != 0.0f); }
  bool hasModifiedFlatness() const { return (_modifiedFlatness != 0.0f); }

  void setModifiedNormal(const cvec3f& pn) { _modifiedNormal = pn; }
  void setModifiedNormalT(float t) { _modifiedNormalT = t; }
  void setModifiedFlatness(float s) { 
    _modifiedFlatness = s; 
  }
  void setTheta(float theta) { _theta = theta; }

  // this methods should be only called from tagtri
  const cvec3f& normal() const { return _normal; }
  void setNormal(const cvec3f& n) { _normal = n; }

  SectorInfo* clone() const {
    SectorInfo* si = new SectorInfo();
    (*si) = (*this);
    si->_refCount = 0;
    return si;
  }

  static void ref(SectorInfo* si) 
    { if(si) ++si->_refCount; }
  static void unref(SectorInfo* si) 
    { if(si) if((0 == (--si->_refCount))) delete(si); }


private:
  unsigned long _refCount;
  SectorTagType _sectorTag;
  cvec3f _normal;
  cvec3f _modifiedNormal;
  float _modifiedNormalT;
  float _modifiedFlatness;
  float _theta;
};

  
#endif /* __SECTORINFO_H__ */


