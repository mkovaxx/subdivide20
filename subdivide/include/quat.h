// -*- Mode: c++ -*-
// $Id: quat.h,v 1.1 2000/04/29 18:10:14 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/quat.h,v $

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

#ifndef __QUAT_H__
#define __QUAT_H__
#include "cvec3t.h"
#include <math.h>
#include <string.h>

class HMatrix;
class Quat;

class Quat {
  private:
    typedef CVec3T<float> CVec3f;

  public:
    friend class HMatrix;
    Quat() {}
    Quat(float xc, float yc, float zc, float wc) : x(xc), y(yc), z(zc), w(wc) {}
    Quat(const Quat& q) : x(q.x), y(q.y), z(q.z), w(q.w) {}

    // Construct a unit quaternion from two points on unit sphere
    Quat(const CVec3f& from, const CVec3f& to) {
        x = from.y() * to.z() - from.z() * to.y();
        y = from.z() * to.x() - from.x() * to.z();
        z = from.x() * to.y() - from.y() * to.x();
        w = from.x() * to.x() + from.y() * to.y() + from.z() * to.z();
    }

    // Convert a unit quaternion to two points on unit sphere
    void ballPoints(CVec3f& arcFrom, CVec3f& arcTo) {
        double s;
        s = sqrt(x * x + y * y);
        arcFrom = (s == 0.0) ? CVec3f(0.0f, 1.0f, 0.0f) : CVec3f(-y / s, x / s, 0.0f);
        if (w < 0.0f) {
            arcFrom = -arcFrom;
        }

        arcTo = CVec3f(w * arcFrom.x() - z * arcFrom.y(), w * arcFrom.y() + z * arcFrom.x(),
                       x * arcFrom.y() - y * arcFrom.x());
    }

    Quat& operator=(const Quat& q) {
        x = q.x;
        y = q.y;
        z = q.z;
        w = q.w;
        return *this;
    }

    Quat operator-() const { return Quat(-x, -y, -z, -w); }

    Quat operator+(const Quat& q) const { return Quat(x + q.x, y + q.y, z + q.z, w + q.w); }

    Quat operator-(const Quat& q) { return Quat(x - q.x, y - q.y, z - q.z, w - q.w); }

    Quat operator*(const float s) const { return Quat(s * x, s * y, s * z, s * w); }

    // To combine rotations, use the product qSecond*qFirst,
    // which gives the effect of rotating by qFirst then qSecond
    Quat operator*(const Quat& q) {
        return Quat(w * q.x + x * q.w + y * q.z - z * q.y, w * q.y + y * q.w + z * q.x - x * q.z,
                    w * q.z + z * q.w + x * q.y - y * q.x, w * q.w - x * q.x - y * q.y - z * q.z);
    }
    Quat conj() { return Quat(-x, -y, -z, w); }

  private:
    float x, y, z, w;
};

inline Quat operator*(const float s, const Quat& q) { return q * s; }

#endif /* __QUAT_H__ */
