// -*- Mode: c++ -*-
// $Id: camera.cpp,v 1.3 2001/02/07 15:12:50 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/viewer/camera.cpp,v $

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

#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// storage for current matrices
GLdouble Camera::_proj[16];
GLdouble Camera::_view[16];
GLint Camera::_vp[4];

// ******************** Camera memeber functions ***********************

void Camera::identityModel() { _ballModel.setIdentity(); }

Camera::Camera(const Camera& c)
    : _projectionMatrix(c._projectionMatrix), _modelviewMatrix(c._modelviewMatrix), _trans(c._trans),
      _ballModel(c._ballModel), _znear(c._znear), _zfar(c._zfar), _fovy(c._fovy), _aspect(c._aspect),
      _wincenterx(c._wincenterx), _wincentery(c._wincentery), _winscale(c._winscale) {

    _viewport[0] = c._viewport[0];
    _viewport[1] = c._viewport[1];
    _viewport[2] = c._viewport[2];
    _viewport[3] = c._viewport[3];
}

const Camera& Camera::operator=(const Camera& c) {
    _trans = c._trans;
    _fovy = c._fovy;
    _znear = c._znear;
    _zfar = c._zfar;
    _wincenterx = c._wincenterx;
    _wincentery = c._wincentery;
    _winscale = c._winscale;
    _ballModel = c._ballModel;
    _modelviewMatrix = c._modelviewMatrix;
    _projectionMatrix = c._projectionMatrix;

    _aspect = c._aspect;
    _viewport[0] = c._viewport[0];
    _viewport[1] = c._viewport[1];
    _viewport[2] = c._viewport[2];
    _viewport[3] = c._viewport[3];
    return *this;
}

void Camera::reset() {
    _trans = 0;
    _fovy = 34.f;
    _znear = .1f;
    _zfar = 100.f;
    _winscale = 1.0f;
    _wincenterx = 0.0f;
    _wincentery = 0.0f;
    _aspect = 1.0;
    identityModel();

    computeModelview();
    computeProjection();
}

void Camera::setFovy(float fovy_degrees) {
    _fovy = fovy_degrees;
    computeProjection();
}

void Camera::setPerspectiveParams(float fovy_degrees, float znear, float zfar) {
    _fovy = fovy_degrees;
    _znear = znear;
    _zfar = zfar;
    computeProjection();
}

// ******************** OpenGL matrices ********************

// TODO: these need not use OpenGL
void Camera::computeModelview() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(_trans.x(), _trans.y(), _trans.z());

    glMultMatrixd((double*)(_ballModel));
    glGetDoublev(GL_MODELVIEW_MATRIX, (double*)(_modelviewMatrix));
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// TODO: these need not use OpenGL
void Camera::computeProjection() {

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    float ysize = tan(0.5 * _fovy * M_PI / 180.0) * _znear;
    float xsize = _aspect * ysize;
    float left = _wincenterx - xsize * _winscale;
    float right = _wincenterx + xsize * _winscale;
    float bottom = _wincentery - ysize * _winscale;
    float top = _wincentery + ysize * _winscale;

    glFrustum(left, right, bottom, top, _znear, _zfar);

    glGetDoublev(GL_PROJECTION_MATRIX, (double*)(_projectionMatrix));
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

void Camera::loadMatrices() const {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd((const double*)(_projectionMatrix));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd((const double*)(_modelviewMatrix));
}

// ******************** project/unproject ********************

// Helper to convert HMatrix (assumed double[16]) to glm::mat4
static inline glm::mat4 hmatrixToGlm(const HMatrix& hMat) {
    // HMatrix seems to be castable to (const double*). Assuming column-major.
    return glm::make_mat4(static_cast<const double*>(hMat));
}

// project a point from world space to screen space
CVec3T<float> Camera::project(const CVec3T<float>& worldPoint) const {
    glm::mat4 modelView = hmatrixToGlm(_modelviewMatrix);
    glm::mat4 projection = hmatrixToGlm(_projectionMatrix);
    glm::vec4 viewport = glm::vec4(_viewport[0], _viewport[1], _viewport[2], _viewport[3]);

    glm::vec3 screenCoords = glm::project(
        glm::vec3(worldPoint.x(), worldPoint.y(), worldPoint.z()),
        modelView,
        projection,
        viewport
    );
    return CVec3T<float>(screenCoords.x, screenCoords.y, screenCoords.z);
}

// compute the world space coordinate for an image coordinate
CVec3T<float> Camera::unproject(const CVec3T<float>& imagePoint) const {
    glm::mat4 modelView = hmatrixToGlm(_modelviewMatrix);
    glm::mat4 projection = hmatrixToGlm(_projectionMatrix);
    glm::vec4 viewport = glm::vec4(_viewport[0], _viewport[1], _viewport[2], _viewport[3]);

    glm::vec3 worldCoords = glm::unProject(
        glm::vec3(imagePoint.x(), imagePoint.y(), imagePoint.z()),
        modelView,
        projection,
        viewport
    );
    return CVec3T<float>(worldCoords.x, worldCoords.y, worldCoords.z);
}

CVec3T<float> Camera::unproject(const HMatrix& model, const CVec3T<float>& imagePoint) const {
    glm::mat4 modelView = hmatrixToGlm(_modelviewMatrix * model);
    glm::mat4 projection = hmatrixToGlm(_projectionMatrix);
    glm::vec4 viewport = glm::vec4(_viewport[0], _viewport[1], _viewport[2], _viewport[3]);

    glm::vec3 worldCoords = glm::unProject(
        glm::vec3(imagePoint.x(), imagePoint.y(), imagePoint.z()),
        modelView,
        projection,
        viewport
    );
    return CVec3T<float>(worldCoords.x, worldCoords.y, worldCoords.z);
}

// ******************** camera modification ********************

void Camera::translate(const CVec3T<float>& t) { _trans += t; }

void Camera::translateWindow(float dx, float dy) {
    _wincentery -= 2.0 * dy * tan(0.5 * _fovy * M_PI / 180.0) * _znear * _winscale;
    _wincenterx -= 2.0 * dx * tan(0.5 * _fovy * M_PI / 180.0) * _aspect * _znear * _winscale;
    computeProjection();
}

void Camera::scaleWindow(float sfactor) {
    _winscale = _winscale * pow(2.0f, sfactor);
    computeProjection();
}

// ******************** input/output  ********************

std::ostream& operator<<(std::ostream& os, const Camera& camera) {
    os << "TRANSFORMATIONS:" << std::endl;
    os << "Modelview matrix: " << camera._modelviewMatrix << std::endl;
    os << "Projection matrix: " << camera._projectionMatrix << std::endl;

    os << "CAMERA PARAMETERS:" << std::endl;
    os << "ballModel : " << camera._ballModel << std::endl;
    os << "trans: " << camera._trans << std::endl;
    os << "aspect: " << camera._aspect << std::endl;
    os << "znear: " << camera._znear << std::endl;
    os << "zfar: " << camera._zfar << std::endl;
    os << "fovy: " << camera._fovy << std::endl;
    return os;
}

std::istream& operator>>(std::istream& is, Camera&) {
    die();
    // TODO
    return is;
}

// ********************** current matrix project *********

/*
void Camera::getMatricesC()const {
  glGetDoublev(GL_MODELVIEW_MATRIX,(double*)_view);
  glGetDoublev(GL_PROJECTION_MATRIX,(double*)_proj);
  glGetIntegerv(GL_VIEWPORT,_vp);
}

CVec3T<float> Camera::projectC(const CVec3T<float>&  worldPoint) const {
  GLdouble winx, winy, winz;
  gluProject(worldPoint.x(),worldPoint.y(),worldPoint.z(),
             _view,_proj,_vp,&winx,&winy,&winz);
  return CVec3T<float>(winx,winy,winz);
}

CVec3T<float> Camera::unprojectC(const CVec3T<float>& screenPoint) const {
  GLdouble wx, wy, wz;
  gluUnProject(screenPoint.x(),screenPoint.y(),screenPoint.z(),
               _view, _proj, _vp, &wx, &wy, &wz);
  return CVec3T<float>(wx,wy,wz);
}
*/
