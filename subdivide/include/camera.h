// -*- Mode: c++ -*-
// $Id: camera.h,v 1.1 2000/04/29 12:29:26 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/include/camera.h,v $

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

#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "arcball.h"
#include "compat.h"
#include "cvec3t.h"
#include "hmatrix.h"
#include <GL/glu.h>

class Camera;
ostream& operator<<(ostream& os, const Camera& camera);
istream& operator>>(istream& is, Camera& camera);

//: Camera class for UI
class Camera {

  public:
    Camera() { reset(); }
    Camera(const Camera&);
    const Camera& operator=(const Camera&);

    void reset();
    void identityModel();

    void computeModelview();
    void computeProjection();

    void loadMatrices() const;
    void getCurrentMatrices();

    CVec3T<float> project(const CVec3T<float>& worldPoint) const;
    CVec3T<float> unproject(const CVec3T<float>& imagePoint) const;
    CVec3T<float> unproject(const HMatrix& model, const CVec3T<float>& imagePoint) const;

    CVec3T<float> projectVector(const CVec3T<float>& worldVector, const CVec3T<float>& startPoint) const {
        return project(worldVector + startPoint) - project(startPoint);
    }

    // camera world position
    CVec3T<float> viewWorldPosition() const {
        HMatrix A;
        A.setInv(_ballModel);
        return A * (-_trans);
    }

    // camera position (in camera coordinates)
    CVec3T<float> viewPosition() const { return -_trans; }

    float fovy() const { return _fovy; }
    float aspect() const { return _aspect; }
    float znear() const { return _znear; }
    float zfar() const { return _zfar; }
    const HMatrix& model() const { return _ballModel; }

    const HMatrix& projectionMatrix() const { return _projectionMatrix; }
    const HMatrix& modelviewMatrix() const { return _modelviewMatrix; }
    HMatrix& model() { return _ballModel; }

    const int* viewport(void) const { return _viewport; }

    void setWinCenter(float wincenterx, float wincentery) {
        _wincenterx = wincenterx;
        _wincentery = wincentery;
    }

    void getWinCenter(float& wincenterx, float& wincentery) const {
        wincenterx = _wincenterx;
        wincentery = _wincentery;
    }

    void setWinScale(float winscale) { _winscale = winscale; }

    float getWinScale() const { return _winscale; }

    void setPerspectiveParams(float fovy_degrees, float znear, float zfar);
    void setFovy(float fovy_degrees);
    void setClippingPlanes(float znear, float zfar) {
        _znear = znear;
        _zfar = zfar;
        computeProjection();
    }

    void translateWindow(float dx, float dy);
    void scaleWindow(float sfactor);
    void translate(const CVec3T<float>& t);
    void setViewport(const int vp[4]) {
        _viewport[0] = vp[0];
        _viewport[1] = vp[1];
        _viewport[2] = vp[2];
        _viewport[3] = vp[3];
        _aspect = float(vp[2]) / float(vp[3]);
    }

    friend ostream& operator<<(ostream& os, const Camera& camera);
    friend istream& operator>>(istream& is, Camera& camera);

  private:
    // projection and modelview matricies
    HMatrix _projectionMatrix;
    HMatrix _modelviewMatrix;

    // modelview parameters (used to create modelview matrix)
    CVec3T<float> _trans; //  camera position
    HMatrix _ballModel;   // the camera rotation matrix

    // projection parameters (used to create projection matrix)
    float _znear;  // near clipping plane
    float _zfar;   // far clipping plane
    float _fovy;   // field of view angle, in degrees, in the y direction
    float _aspect; // x:y aspect ratio

    // center and size of the
    // visible part of the window; center is given as offset from
    // the frustum center; the full view corresponds to
    // _wincenter = (0,0), _winscale =1.0
    float _wincenterx, _wincentery, _winscale;

    GLint _viewport[4];

    // second part of matrixes. used for project and unproject with
    // current axes
    static GLdouble _proj[16];
    static GLdouble _view[16];
    static GLint _vp[4];
};

#endif
