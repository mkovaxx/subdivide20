// -*- Mode: c++ -*-
// $Id: cvec3t.h,v 1.2 2000/04/29 12:29:26 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/cvec3t.h,v $

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

#ifndef __CVEC3T_H__
#define __CVEC3T_H__

#include "compat.h"
#include <assert.h>
#include <math.h>

template <class F> class CVec3T;

// another VC 6.0 problem: would not accept this definition
// after the class

template <class F> std::istream& operator>>(std::istream& is, CVec3T<F>& v) {
    is >> v.x();
    is >> v.y();
    is >> v.z();
    return is;
}

template <class F> std::ostream& operator<<(std::ostream& os, const CVec3T<F>& v) {
    return os << v(0) << " " << v(1) << " " << v(2);
}

//: template for vectors of length 3
template <class F> class CVec3T {
  public:
    enum { X = 0, Y = 1, Z = 2 };
    CVec3T(void) {
        v[X] = 0;
        v[Y] = 0;
        v[Z] = 0;
    }
    CVec3T(const CVec3T& c) { v[X] = c.v[X], v[Y] = c.v[Y], v[Z] = c.v[Z]; }
    CVec3T(const F& a, const F& b, const F& c) {
        v[X] = a, v[Y] = b;
        v[Z] = c;
    }
    //: the argument is required to be zero; assumes that
    // conversion of 0 to the type F works
    CVec3T(int j) {
        assert(j == 0);
        v[X] = F(0);
        v[Y] = F(0);
        v[Z] = F(0);
    }

    CVec3T(const F* a) { v[X] = a[X], v[Y] = a[Y], v[Z] = a[Z]; }
    operator F*(void) { return &v[X]; }
    operator const F*(void) const { return &v[X]; }

    bool read(std::istream& is) { return is >> *this; }
    CVec3T& operator=(const CVec3T& c) {
        v[X] = c.v[X];
        v[Y] = c.v[Y];
        v[Z] = c.v[Z];
        return *this;
    }
    CVec3T operator+(const CVec3T& c) const { return CVec3T(v[X] + c.v[X], v[Y] + c.v[Y], v[Z] + c.v[Z]); }
    CVec3T operator-(const CVec3T& c) const { return CVec3T(v[X] - c.v[X], v[Y] - c.v[Y], v[Z] - c.v[Z]); }
    CVec3T operator*(const F& s) const { return CVec3T(s * v[X], s * v[Y], s * v[Z]); }
    CVec3T operator*(const CVec3T& c) const { return CVec3T(v[X] * c.v[X], v[Y] * c.v[Y], v[Z] * c.v[Z]); }
    friend CVec3T operator*(const F& s, const CVec3T& c) { return CVec3T(s * c.v[X], s * c.v[Y], s * c.v[Z]); }
    CVec3T operator/(const F& s) const { return CVec3T(v[X] / s, v[Y] / s, v[Z] / s); }
    CVec3T& operator+=(const CVec3T& c) {
        v[X] += c.v[X], v[Y] += c.v[Y];
        v[Z] += c.v[Z];
        return *this;
    }
    CVec3T& operator-=(const CVec3T& c) {
        v[X] -= c.v[X], v[Y] -= c.v[Y];
        v[Z] -= c.v[Z];
        return *this;
    }
    CVec3T& operator*=(const F& s) {
        v[X] *= s, v[Y] *= s, v[Z] *= s;
        return *this;
    }
    CVec3T& operator/=(const F& s) {
        v[X] /= s, v[Y] /= s, v[Z] /= s;
        return *this;
    }
    CVec3T operator-(void) const { return CVec3T(-v[X], -v[Y], -v[Z]); }

    void plus_aX(F a, const CVec3T& u) {
        v[X] += a * u[X];
        v[Y] += a * u[X];
        v[Z] += a * u[Z];
    }

    F& x(void) { return v[X]; }
    const F& x(void) const { return v[X]; }
    F& y(void) { return v[Y]; }
    const F& y(void) const { return v[Y]; }
    F& z(void) { return v[Z]; }
    const F& z(void) const { return v[Z]; }
    F& operator()(const int i) { return v[i]; }
    const F& operator()(const int i) const { return v[i]; }
    //: returns v + t(v1 - v), where v is *this
    CVec3T lerp(const CVec3T& v1, const F& t) const {
        return CVec3T(v[X] + t * (v1.v[X] - v[X]), v[Y] + t * (v1.v[Y] - v[Y]), v[Z] + t * (v1.v[Z] - v[Z]));
    }
    //: returns the componentwise minimum
    CVec3T min(const CVec3T& o) const {
        F a = std::min(v[X], o.v[X]);
        F b = std::min(v[Y], o.v[Y]);
        F c = std::min(v[Z], o.v[Z]);
        return CVec3T(a, b, c);
    }
    //: returns the componentwise maximum
    CVec3T max(const CVec3T& o) const {
        F a = std::max(v[X], o.v[X]);
        F b = std::max(v[Y], o.v[Y]);
        F c = std::max(v[Z], o.v[Z]);
        return CVec3T(a, b, c);
    }

    F dot(const CVec3T& c) const { return v[X] * c.v[X] + v[Y] * c.v[Y] + v[Z] * c.v[Z]; }
    F dot(void) const { return v[X] * v[X] + v[Y] * v[Y] + v[Z] * v[Z]; }
    //: l1 norm (sum of abs values of the components)
    F l1(void) const {
        F a = fabs(v[X]);
        a += fabs(v[Y]);
        a += fabs(v[Z]);
        return a;
    }
    //: l infinity norm (max of abs values of the components)
    F linfty(void) const {
        F a = fabs(v[X]);
        a = std::max(a, F(abs(v[Y])));
        a = std::max(a, F(abs(v[Z])));
        return a;
    }
    //: l infinity norm (sqrt of the sum of squares )
    F l2(void) const { return sqrt(dot()); }
    //: for nonzero vectors, returns the normalized vector
    CVec3T dir(void) const {
        F a = l2();
        if ((a == 0))
            return *this;
        else
            return *this / a;
    }

    int normalize(void) {
        F mag = l2();
        return (mag == 0) ? 0 : (*this *= 1 / mag, 1);
    }
    //:  length |*this -c|
    F dist(const CVec3T& c) const { return (*this - c).l2(); }

    CVec3T<F> cross(const CVec3T<F>& b) const {
        return CVec3T<F>(y() * b.z() - z() * b.y(), z() * b.x() - x() * b.z(), x() * b.y() - y() * b.x());
    }

    CVec3T<F> abs() { return CVec3T<F>(fabs(x()), fabs(y()), fabs(z())); }

    //: conversion of different vectors
    // note that it is a template    memeber function (parameterzed by G) of
    // the template  class parameterized by F; not all compilers can handle this
    // so it is removed for now (gcc 2.8.1 does not understand it)
    //  template <class G> CVec3T( const CVec3T<G>& u ) { v[0] = u.x(); v[1] = u.y();
    //  v[2] = u.z();  }

  protected:
  private:
    F v[3];
};

typedef CVec3T<float> cvec3f;

#endif /* __CVEC3T_H__ */
