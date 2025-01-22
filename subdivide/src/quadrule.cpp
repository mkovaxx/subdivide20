// -*- Mode: c++ -*-
// $Id: quadrule.cpp,v 1.2 2000/04/29 09:27:19 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/src/quadrule.cpp,v $

#include "quadrule.hpp"
#include "compat.hpp"
#include <math.h>

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

//------------------------------------------------------------------------------

InteriorQuadRule::InteriorQuadRule(int k) : k(k) {

    computeLambda1();
    computeLambda2();
    computeL0();
    computeL1();
    computeL2();
    computeX1();
    computeX2();
    computeSubCoef();
    computeEdgeSubCoef();
}

void InteriorQuadRule::computeEdgeSubCoef() {
    _edgeSub.cf[0] = 3 / 8.0;
    _edgeSub.cf[3] = 3 / 8.0;

    _edgeSub.cf[1] = 1 / 16.0;
    _edgeSub.cf[2] = 1 / 16.0;
    _edgeSub.cf[4] = 1 / 16.0;
    _edgeSub.cf[5] = 1 / 16.0;
}

void InteriorQuadRule::computeSubCoef() {
    _sub.edgeC = std::vector<float>(k);
    _sub.faceC = std::vector<float>(k);

    float beta = 3 / 2.0 / (float)k;
    float gamma = 1 / 4.0 / (float)k;

    _sub.centerC = 1 - beta - gamma;

    for (int i = 0; i < k; i++) {
        _sub.edgeC[i] = beta / (float)k;
        _sub.faceC[i] = gamma / (float)k;
    }
}

void InteriorQuadRule::computeLambda1() {
    float thetaK = M_PI * 2.0 / (float)k;
    float An = 1 + cos(thetaK) + cos(M_PI / (float)k) * sqrt(2.0 * (9.0 + cos(thetaK)));
    _lambda1 = (An + 4.0) / 16.0;
}

void InteriorQuadRule::computeLambda2() { _lambda2 = lambda1(); }

void InteriorQuadRule::computeL1() {
    _l1.edgeC = std::vector<float>(k);
    _l1.faceC = std::vector<float>(k);
    _l1.centerC = 0.0;

    float lambda = lambda1();
    float thetaK = M_PI * 2.0 / (float)k;
    float f1 = 1 / (4.0 * lambda - 1);

    for (int i = 0; i < k; i++) {
        _l1.edgeC[i] = 4.0 * sin(i * thetaK);
        _l1.faceC[i] = f1 * (sin(i * thetaK) + sin((i + 1) * thetaK));
    }
}

void InteriorQuadRule::computeL0() {
    _l0.edgeC = std::vector<float>(k);
    _l0.faceC = std::vector<float>(k);
    _l0.centerC = float(k) / (float(k) + 5.0f);

    for (int i = 0; i < k; i++) {
        _l0.edgeC[i] = 4.0f / float(k) / (float(k) + 5.0);
        _l0.faceC[i] = 1.0f / float(k) / (float(k) + 5.0);
    }
}

void InteriorQuadRule::computeL2() {
    _l2.edgeC = std::vector<float>(k);
    _l2.faceC = std::vector<float>(k);
    _l2.centerC = 0.0;

    float lambda = lambda1();
    float thetaK = M_PI * 2.0 / (float)k;
    float f1 = 1 / (4.0 * lambda - 1);

    for (int i = 0; i < k; i++) {
        _l2.edgeC[i] = 4.0 * cos(i * thetaK);
        _l2.faceC[i] = f1 * (cos(i * thetaK) + cos((i + 1) * thetaK));
    }
}

void InteriorQuadRule::computeX1() {
    _x1.edgeC = std::vector<float>(k);
    _x1.faceC = std::vector<float>(k);
    _x1.centerC = 0.0;

    float lambda = lambda1();
    float thetaK = M_PI * 2.0 / (float)k;
    float f1 = 1 / (4.0 * lambda - 1);
    float normalizationConst = k * (2.0 + ((cos(thetaK) + 1.0) * f1 * f1));

    for (int i = 0; i < k; i++) {
        _x1.edgeC[i] = sin(i * thetaK) / normalizationConst;
        _x1.faceC[i] = f1 * (sin(i * thetaK) + sin((i + 1) * thetaK)) / normalizationConst;
    }
}

void InteriorQuadRule::computeX2() {
    _x2.faceC = std::vector<float>(k);
    _x2.edgeC = std::vector<float>(k);
    _x2.centerC = 0.0;

    float lambda = lambda1();
    float thetaK = M_PI * 2.0 / (float)k;
    float f1 = 1 / (4.0 * lambda - 1);
    float normalizationConst = k * (2.0 + ((cos(thetaK) + 1.0) * f1 * f1));

    for (int i = 0; i < k; i++) {
        _x2.edgeC[i] = cos(i * thetaK) / normalizationConst;
        _x2.faceC[i] = f1 * (cos(i * thetaK) + cos((i + 1) * thetaK)) / normalizationConst;
    }
}

//------------------------------------------------------------------------------

CreaseQuadRule::CreaseQuadRule(int k) : k(k) {

    computeLambda1();
    computeLambda2();
    computeL0();
    computeL1();
    computeL2();
    computeX1();
    computeX2();
    computeSubCoef();
    computeEdgeSubCoef();
}

void CreaseQuadRule::computeEdgeSubCoef() {
    _edgeSub.cf[1] = 1 / 16.0;
    _edgeSub.cf[2] = 1 / 16.0;
    _edgeSub.cf[4] = 1 / 16.0;
    _edgeSub.cf[5] = 1 / 16.0;

    float thetaK = M_PI / (float)k;
    _edgeSub.cf[3] = 3 / 8.0 - 1 / 4.0 * cos(thetaK);
    _edgeSub.cf[0] = 1.0 - 2 / 8.0 - _edgeSub.cf[3];
}

void CreaseQuadRule::computeSubCoef() {
    int i;

    _sub.edgeC = std::vector<float>(k + 1);
    _sub.faceC = std::vector<float>(k);

    _sub.centerC = 6.0 / 8.0;
    _sub.edgeC[0] = 1.0 / 8.0;
    _sub.edgeC[k] = 1.0 / 8.0;

    for (i = 0; i < k; i++) {
        _sub.faceC[i] = 0.0;
    }

    for (i = 1; i < k; i++) {
        _sub.edgeC[i] = 0.0;
    }
}

void CreaseQuadRule::computeLambda1() { _lambda1 = (k == 1) ? 0.25 : 0.5; }

void CreaseQuadRule::computeLambda2() { _lambda2 = 0.5; }

void CreaseQuadRule::computeL0() {
    _l0.edgeC = std::vector<float>(k + 1);
    _l0.faceC = std::vector<float>(k);
    _l0.centerC = 2.0f / 3.0f;

    for (int i = 0; i < k; i++) {
        _l0.edgeC[i] = 0.0f;
        _l0.faceC[i] = 0.0f;
    }
    _l0.edgeC[0] = 1.0f / 6.0f;
    _l0.edgeC[k] = 1.0f / 6.0f;
}

void CreaseQuadRule::computeL1() {
    _l1.edgeC = std::vector<float>(k + 1);
    _l1.faceC = std::vector<float>(k);

    if (k == 1) {
        _l1.centerC = 6.0;
        _l1.edgeC[0] = -3.0;
        _l1.edgeC[1] = -3.0;
        _l1.faceC[0] = 0.0;
    } else {
        float thetaK = M_PI / (float)k;
        float R = (cos(thetaK) + 1.0) / (sin(thetaK) * (3.0 + cos(thetaK)) * (float)k);

        _l1.edgeC[0] = -R * (1.0 + 2.0 * cos(thetaK));
        _l1.edgeC[k] = -R * (1.0 + 2.0 * cos(thetaK));

        int i;
        for (i = 1; i <= k - 1; i++) {
            _l1.edgeC[i] = 4.0 / ((3.0 + cos(thetaK)) * (float)k) * sin((float)i * thetaK);
        }

        for (i = 0; i <= k - 1; i++) {
            _l1.faceC[i] = 1.0 / ((3.0 + cos(thetaK)) * (float)k) * (sin((float)i * thetaK) + sin((i + 1.0) * thetaK));
        }

        _l1.centerC = 4.0 * R * (-1.0 + cos(thetaK));
    }
}

void CreaseQuadRule::computeL2() {

    _l2.edgeC = std::vector<float>(k + 1);
    _l2.faceC = std::vector<float>(k);

    if (k == 1) {
        _l2.centerC = 0.0;
        _l2.edgeC[0] = -1.0;
        _l2.edgeC[1] = 1.0;
        _l2.faceC[0] = 0.0;
    } else {
        _l2.centerC = 0.0;
        _l2.edgeC[0] = 0.5;
        _l2.edgeC[k] = -0.5;
        int i;
        for (i = 1; i <= k - 1; i++) {
            _l2.edgeC[i] = 0.0;
        }
        for (i = 0; i <= k - 1; i++) {
            _l2.faceC[i] = 0.0;
        }
    }
}

void CreaseQuadRule::computeX1() {
    _x1.edgeC = std::vector<float>(k + 1);
    _x1.faceC = std::vector<float>(k);

    float thetaK = M_PI / (float)k;

    if (k == 1) {
        _x1.centerC = float(1.0 / 18.0);
        _x1.edgeC[0] = float(-2.0 / 18.0);
        _x1.edgeC[1] = float(-2.0 / 18.0);
        _x1.faceC[0] = float(-5.0 / 18.0);
    } else {
        _x1.centerC = 0.0;

        int i;
        for (i = 0; i <= k; i++) {
            _x1.edgeC[i] = sin(thetaK * i);
        }

        for (i = 0; i <= k - 1; i++) {
            _x1.faceC[i] = sin(thetaK * i) + sin(thetaK * (i + 1));
        }
    }
}

void CreaseQuadRule::computeX2() {
    _x2.edgeC = std::vector<float>(k + 1);
    _x2.faceC = std::vector<float>(k);

    float thetaK = M_PI / (float)k;

    if (k == 1) {
        _x2.centerC = 0.0;
        _x2.edgeC[0] = -0.5;
        _x2.edgeC[1] = 0.5;
        _x2.faceC[0] = 0.0;
    } else {

        _x2.centerC = 0.0;
        int i;
        for (i = 0; i <= k; i++) {
            _x2.edgeC[i] = cos(thetaK * i);
        }

        for (i = 0; i <= k - 1; i++) {
            _x2.faceC[i] = cos(thetaK * i) + cos(thetaK * (i + 1));
        }
    }
}

//------------------------------------------------------------------------------

ConvexQuadRule::ConvexQuadRule(int k, float theta) : k(k), theta(theta) {

    computeL0();
    computeL1();
    computeL2();
    computeX1();
    computeX2();
    computeLambda1();
    computeLambda2();
    computeSubCoef();
    computeEdgeSubCoef();
}

void ConvexQuadRule::computeEdgeSubCoef() {
    _edgeSub.cf[1] = 1 / 16.0;
    _edgeSub.cf[2] = 1 / 16.0;
    _edgeSub.cf[4] = 1 / 16.0;
    _edgeSub.cf[5] = 1 / 16.0;

    float thetaK = theta / (float)k;
    _edgeSub.cf[3] = 3 / 8.0 - 1 / 4.0 * cos(thetaK);
    _edgeSub.cf[0] = 1.0 - 2 / 8.0 - _edgeSub.cf[3];
}

void ConvexQuadRule::computeSubCoef() {
    _sub.edgeC = std::vector<float>(k + 1);
    _sub.faceC = std::vector<float>(k);

    _sub.centerC = 1.0;

    int i;
    for (i = 0; i < k; i++) {
        _sub.faceC[i] = 0.0;
    }

    for (i = 1; i < k + 1; i++) {
        _sub.edgeC[i] = 0.0;
    }
}

void ConvexQuadRule::computeLambda1() { _lambda1 = 0.5; }

void ConvexQuadRule::computeLambda2() { _lambda2 = lambda1(); }

void ConvexQuadRule::computeL2() {
    _l2.edgeC = std::vector<float>(k + 1);
    _l2.faceC = std::vector<float>(k);
    _l2.centerC = -1.0;

    _l2.edgeC[0] = 1.0;
    int i;
    for (i = 1; i < k + 1; i++) {
        _l2.edgeC[i] = 0.0;
    }
    for (i = 0; i < k; i++) {
        _l2.faceC[i] = 0.0;
    }
}

void ConvexQuadRule::computeL0() {
    _l0.edgeC = std::vector<float>(k + 1);
    _l0.faceC = std::vector<float>(k);
    _l0.centerC = 1.0f;

    for (int i = 0; i < k; i++) {
        _l0.edgeC[i] = 0.0f;
        _l0.faceC[i] = 0.0f;
    }
    _l0.edgeC[k] = 0.0f;
}

void ConvexQuadRule::computeL1() {
    _l1.edgeC = std::vector<float>(k + 1);
    _l1.faceC = std::vector<float>(k);
    _l1.centerC = -1.0;

    _l1.edgeC[k] = 1.0;
    int i;
    for (i = 0; i < k; i++) {
        _l1.edgeC[i] = 0.0;
    }
    for (i = 0; i < k; i++) {
        _l1.faceC[i] = 0.0;
    }
}

void ConvexQuadRule::computeX2() {

    float thetak = theta / (float)k;

    _x2.edgeC = std::vector<float>(k + 1);
    _x2.faceC = std::vector<float>(k);
    _x2.centerC = 0.0;

    int i;
    for (i = 0; i < k + 1; ++i) {
        _x2.edgeC[i] = sin(thetak * (float)(k - i)) / sin(theta);
    }
    for (i = 0; i < k; i++) {
        _x2.faceC[i] = (sin(thetak * (float)(k - i)) + sin(thetak * (float)(k - i - 1))) / sin(theta);
    }
}

void ConvexQuadRule::computeX1() {

    float thetak = theta / (float)k;

    _x1.edgeC = std::vector<float>(k + 1);
    _x1.faceC = std::vector<float>(k);
    _x1.centerC = 0.0;

    int i;
    for (i = 0; i < k + 1; i++) {
        _x1.edgeC[i] = sin(thetak * (float)i) / sin(theta);
    }
    for (i = 0; i < k; i++) {
        _x1.faceC[i] = (sin(thetak * (float)i) + sin(thetak * (float)(i + 1))) / sin(theta);
    }
}

//------------------------------------------------------------------------------

ConcaveQuadRule::ConcaveQuadRule(int k, float theta) : k(k), theta(theta) {

    computeL0();
    computeL1();
    computeL2();
    computeX1();
    computeX2();
    computeLambda1();
    computeLambda2();
    computeSubCoef();
    computeEdgeSubCoef();
}

void ConcaveQuadRule::computeEdgeSubCoef() {
    _edgeSub.cf[1] = 1 / 16.0;
    _edgeSub.cf[2] = 1 / 16.0;
    _edgeSub.cf[4] = 1 / 16.0;
    _edgeSub.cf[5] = 1 / 16.0;

    float thetaK = theta / (float)k;
    _edgeSub.cf[3] = 3 / 8.0 - 1 / 4.0 * cos(thetaK);
    _edgeSub.cf[0] = 1.0 - 2 / 8.0 - _edgeSub.cf[3];
}

void ConcaveQuadRule::computeSubCoef() {
    _sub.edgeC = std::vector<float>(k + 1);
    _sub.faceC = std::vector<float>(k);
    _sub.centerC = 1.0;

    int i;
    for (i = 0; i < k; i++) {
        _sub.faceC[i] = 0.0;
    }

    for (i = 1; i < k + 1; i++) {
        _sub.edgeC[i] = 0.0;
    }
}

void ConcaveQuadRule::computeLambda1() { _lambda1 = 0.5; }

void ConcaveQuadRule::computeLambda2() { _lambda2 = lambda1(); }

void ConcaveQuadRule::computeL0() {
    _l0.edgeC = std::vector<float>(k + 1);
    _l0.faceC = std::vector<float>(k);
    _l0.centerC = 1.0f;

    for (int i = 0; i < k; i++) {
        _l0.edgeC[i] = 0.0f;
        _l0.faceC[i] = 0.0f;
    }
    _l0.edgeC[k] = 0.0f;
}

void ConcaveQuadRule::computeL1() {
    _l1.edgeC = std::vector<float>(k + 1);
    _l1.faceC = std::vector<float>(k);
    _l1.centerC = -1.0;

    _l1.edgeC[0] = 1.0;
    int i;
    for (i = 1; i < k + 1; i++) {
        _l1.edgeC[i] = 0.0;
    }
    for (i = 0; i < k; i++) {
        _l1.faceC[i] = 0.0;
    }
}

void ConcaveQuadRule::computeL2() {
    _l2.edgeC = std::vector<float>(k + 1);
    _l2.faceC = std::vector<float>(k);
    _l2.centerC = -1.0;

    _l2.edgeC[k] = 1.0;
    int i;
    for (i = 0; i < k; i++) {
        _l2.edgeC[i] = 0.0;
    }
    for (i = 0; i < k; i++) {
        _l2.faceC[i] = 0.0;
    }
}

void ConcaveQuadRule::computeX1() {

    float thetak = theta / (float)k;

    _x1.edgeC = std::vector<float>(k + 1);
    _x1.faceC = std::vector<float>(k);
    _x1.centerC = 0.0;

    int i;
    for (i = 0; i < k + 1; ++i) {
        _x1.edgeC[i] = sin(thetak * (float)(k - i)) / sin(theta);
    }
    for (i = 0; i < k; i++) {
        _x1.faceC[i] = (sin(thetak * (float)(k - i)) + sin(thetak * (float)(k - i - 1))) / sin(theta);
    }
}

void ConcaveQuadRule::computeX2() {

    float thetak = theta / (float)k;

    _x2.edgeC = std::vector<float>(k + 1);
    _x2.faceC = std::vector<float>(k);
    _x2.centerC = 0.0;

    int i;
    for (i = 0; i < k + 1; i++) {
        _x2.edgeC[i] = sin(thetak * (float)i) / sin(theta);
    }
    for (i = 0; i < k; i++) {
        _x2.faceC[i] = (sin(thetak * (float)i) + sin(thetak * (float)(i + 1))) / sin(theta);
    }
}
