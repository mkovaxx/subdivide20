// -*- Mode: c++ -*-
// $Id: glcheck.hpp,v 1.2 2000/04/29 12:29:27 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/viewer/glcheck.hpp,v $

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

#ifndef __GL_CHECK_H__
#define __GL_CHECK_H__
#include "compat.hpp"

#include <GLFW/glfw3.h>

#define glCheck() __glCheck__(__FILE__, __LINE__)

namespace {
    // Function to get a human-readable string for an OpenGL error code
    const char* glErrorToString(GLenum error) {
        switch (error) {
        case GL_NO_ERROR:                       return "GL_NO_ERROR";
        case GL_INVALID_ENUM:                   return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:                  return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:              return "GL_INVALID_OPERATION";
        // GL_STACK_OVERFLOW and GL_STACK_UNDERFLOW are deprecated in core profiles
        // but might still be reported by some drivers/contexts.
        case GL_STACK_OVERFLOW:                 return "GL_STACK_OVERFLOW (deprecated)";
        case GL_STACK_UNDERFLOW:                return "GL_STACK_UNDERFLOW (deprecated)";
        case GL_OUT_OF_MEMORY:                  return "GL_OUT_OF_MEMORY";
        //case GL_INVALID_FRAMEBUFFER_OPERATION:  return "GL_INVALID_FRAMEBUFFER_OPERATION";
        //case GL_CONTEXT_LOST:                   return "GL_CONTEXT_LOST"; // If using OpenGL 4.5+ or KHR_robustness
        default:                                return "Unknown GL error";
        }
    }
}

inline int __glCheck__(char* fileName, int n) {
    int e = glGetError();
    if (e) {
        std::cerr << glErrorToString(e) << "  " << fileName << ", #" << n << std::endl;
    }
    return (e);
}

#endif /* __GL_CHECK_H__ */
