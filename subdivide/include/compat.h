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

#ifndef __COMPAT_H__
#define __COMPAT_H__

#include <assert.h>

// turn out the visual studio warnings (identifier was truncated)

#ifdef _WIN32
#pragma warning(disable : 4786)
#endif

// Windows-specific include for OpenGL and GLUT
#ifdef _WIN32
#include <windows.h>
#endif

// currently, SGI's STL does not have the new-style iostream

#if defined(_WIN32) || defined(__GNUG__) || defined(_STANDARD_C_PLUS_PLUS)
#include <fstream>
#include <iostream>
#include <strstream>
#else
#include <fstream.h>
#include <iostream.h>
#include <strstream.h>
#endif

// 2.8.1 gcc does not support namespaces
// in other cases need this to avoid std:: prefixes
#if (defined(_WIN32) || defined(__sgi)) && !defined(__GNUG__)
namespace std {}
using namespace std;
#endif

// standard C++ does not seem to have bidirectional_iterator
// defined; however, both gcc and SGI CC have it
#ifdef _WIN32
template <class T, class dist> class bidirectional_iterator : public iterator<bidirectional_iterator_tag, T, dist> {};
#endif

// includes min/max declaration

#if (defined(__sgi) || defined(__GNU__))
#include <algobase.h>
#endif

// dealing with Microsoft min/max mess:
// assume that under Windows the code is compiled with NOMINMAX defined
// which disables #define's for min/max.
// however, Microsoft  violates the C++ standard even with
// NOMINMAX on, and defines templates _cpp_min and _cpp_max
// instead of templates min/max
// define the correct templates here

#ifdef _WIN32

template <class _Ty> inline const _Ty& max(const _Ty& _X, const _Ty& _Y) { return (_X < _Y ? _Y : _X); }

template <class _Ty> inline const _Ty& min(const _Ty& _X, const _Ty& _Y) { return (_Y < _X ? _Y : _X); }
#endif

// disable VC6.0 warning "identifier was truncated to 255 characters "
// this is routine whe compiling templates
// unfortunately, it does not seem to work in all cases
// and the messages are still visible when compiling STL

#if defined(_WIN32)
#pragma warning(disable : 4786)
#endif

#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// disable VC6.0 warning truncation from 'double' to 'float'
#ifdef _WIN32
#pragma warning(disable : 4305)
#endif

inline void die() {
#ifndef NDEBUG
    cerr << "unexpected condition, aborting, " << __FILE__ << ":" << __LINE__ << endl;
#endif
    abort();
}

typedef unsigned int uint;

#endif /* __COMPAT_H__ */
