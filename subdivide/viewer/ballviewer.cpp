// -*- Mode: c++ -*-
// $Id: ballviewer.cpp,v 1.6 2001/02/07 15:12:50 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/viewer/ballviewer.cpp,v $

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

#include "ballviewer.h"
#include "compat.h"
#include "glcheck.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "geoobject.h"
#include "uiaction.h"

static void setupLight();

BallViewer::BallViewer(char* name, int w, int h) : Viewer(name, w, h), _uiAction(0) {
    setLightAndMaterial();
    _arcball.ShowResult();
}

BallViewer::~BallViewer() { ; }

void BallViewer::setObject(GeoObject* geoObject) {
    Viewer::setObject(geoObject);
    positionArcBall();
}

void BallViewer::renderObject() {

    // setup gl state
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glColor3f(1.0, 1.0, 0.0);

    // render object
    if (getObject())
        getObject()->render();
    glCheck();
}

void BallViewer::display() {

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //  setupLight();

    glPushMatrix();

    glMultMatrixd((double*)HMatrix::Translation(_arcball.Position()));
    glMultMatrixd(_arcball.Rotation());
    glMultMatrixd((double*)HMatrix::Translation(-_arcball.Position()));

    renderObject();
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    _arcball.DrawCompleteBall(getCamera()->viewWorldPosition());

    glutSwapBuffers();
    glCheck();
}

void BallViewer::mouse(int button, int state, int x, int y) {
    y = getHeight() - y;
    if (state == GLUT_DOWN) {
        if (_uiAction)
            delete _uiAction;

        // map left+shift to right button
        if ((button == GLUT_LEFT_BUTTON) && (glutGetModifiers() & GLUT_ACTIVE_ALT))
            button = GLUT_RIGHT_BUTTON;

        switch (button) {
        case GLUT_LEFT_BUTTON:
            _uiAction = new CameraRotateAction(x, y, &_arcball, getCamera());
            break;
        case GLUT_MIDDLE_BUTTON:
            _uiAction = new CameraTransXYAction(x, y, &_arcball, getCamera());
            break;
        case GLUT_RIGHT_BUTTON:
            _uiAction = new CameraTransZAction(x, y, &_arcball, getCamera());
            break;
        }
    } else {
        delete _uiAction;
        _uiAction = 0;
    }
    glutPostRedisplay();
}

void BallViewer::motion(int x, int y) {
    y = getHeight() - y;
    if (_uiAction)
        _uiAction->update(x, y);
    glutPostRedisplay();
}

void BallViewer::key(unsigned char k, int, int y) {
    y = getHeight() - y;

    switch (k) {
    case 'q':
        // quit the viewer
        exit(0);
        break;
    case 'c':
        // center the arc ball
        if (_uiAction) {
            delete _uiAction;
            _uiAction = 0;
        }
        centerArcBall();
        break;
    }

    glutPostRedisplay();
}

void BallViewer::specialKey(int k, int /* x */, int /* y */) {
    switch (k) {
    case GLUT_KEY_HOME:
        positionCamera();
        positionArcBall();
        break;
    }
    glutPostRedisplay();
}

void BallViewer::centerArcBall() {

    float z = getCamera()->project(getObject()->centerPoint()).z();

    float top = getHeight() * 5.0f / 6.0f;
    float right = getWidth() * 5.0f / 6.0f;
    float cx = getWidth() * 0.5f;
    float cy = getWidth() * 0.5;

    cvec3f pos = getCamera()->unproject(cvec3f(cx, cy, z));
    float rad = min((getCamera()->unproject(cvec3f(right, cy, z)) - pos).l2(),
                    (getCamera()->unproject(cvec3f(cx, top, z)) - pos).l2());

    getArcBall()->Init();
    getArcBall()->SetPosition(pos);
    getArcBall()->SetRadius(rad);
    getArcBall()->Update();
}

void BallViewer::positionArcBall() {
    cvec3f center = getObject()->centerPoint();
    float r = 0.5f * (getObject()->minPoint() - getObject()->maxPoint()).l2();
    getArcBall()->Init();
    getArcBall()->SetPosition(center);
    getArcBall()->SetRadius(r);
    getArcBall()->Update();
}

void setupLight() {
    GLfloat lgt1_diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lgt1_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lgt1_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};

    GLfloat lgt2_diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lgt2_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lgt2_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};

    GLfloat light_pos0[] = {1.0f, 1.0f, 1.0f, 0.0f};
    GLfloat light_pos1[] = {1.0f, 1.0f, 0.0f, 0.0f};
    GLfloat light_pos2[] = {-1.0f, 1.0f, 0.0f, 0.0f};

    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

    glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lgt1_specular);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lgt1_diffuse);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lgt1_ambient);

    glLightfv(GL_LIGHT2, GL_POSITION, light_pos2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lgt2_specular);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lgt2_diffuse);
    glLightfv(GL_LIGHT2, GL_AMBIENT, lgt2_ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);

    glCheck();
}

void BallViewer::setLightAndMaterial() {

    // Light
    setupLight();

    // Material properties
    float matSpecular[] = {0.478814, 0.457627, 0.5};
    float matAmbient[] = {0.75, 0.652647, 0.154303};
    float matDiffuse[] = {0.75, 0.652647, 0.154303};
    float matFrontShininess = 25.0;

    float matBackSpecular[] = {0.1596, 0.1525, 0.1667};
    float matBackAmbient[] = {0.3750, 0.3263, 0.0772};
    float matBackDiffuse[] = {0.3750, 0.3263, 0.0772};
    float matBackShininess = 100.0;

    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);

    glMaterialfv(GL_BACK, GL_DIFFUSE, matBackDiffuse);
    glMaterialfv(GL_BACK, GL_SPECULAR, matBackSpecular);
    glMaterialfv(GL_BACK, GL_AMBIENT, matBackAmbient);

    glMaterialf(GL_FRONT, GL_SHININESS, matFrontShininess);
    glMaterialf(GL_BACK, GL_SHININESS, matBackShininess);

    glCheck();
}
