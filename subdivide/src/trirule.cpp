// -*- Mode: c++ -*-
// $Id: trirule.cpp,v 1.4 2000/04/30 07:35:47 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/src/trirule.cpp,v $

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
#include <math.h>
#include "trirule.h"

static void computeBoundaryParams(float k,
				  float& thetak,
				  float& delta,
				  float& gamma,
				  float& a,
				  float& b,
				  float& theta,
				  float& sigma1, 
				  float& sigma2, 
				  float& sigma3,
				  float th = 0);
  
float Coef::dot(const Coef& coef) const {
  assert(coef.edgeC.size() == edgeC.size());
  float s = centerC * coef.centerC;
  for(uint i = 0; i < edgeC.size(); ++i)
    s += edgeC[i] * coef.edgeC[i];
  return s;
}

float Coef::dotOne() const {
  float s = centerC;
  for(uint i = 0; i < edgeC.size(); ++i)
    s += edgeC[i];
  return s;
}

//----------------------------------------------------------------------------

InteriorTriRule::InteriorTriRule(int k) : k(k) {

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

void InteriorTriRule::computeEdgeSubCoef() {
  _edgeSub.c[0] = 3.0/8.0f; 
  _edgeSub.c[1] = 1.0f/8.0f;
  _edgeSub.c[2] = 3.0/8.0f; 
  _edgeSub.c[3] = 1.0f/8.0f;
}

void InteriorTriRule::computeSubCoef() {

  float beta;

  if(k == 3) 
    beta = 3.0f/16.0f;
  else 
    beta = 3.0f/8.0f/(float)k;  
    
  _sub.edgeC = vector<float>(k);
  _sub.centerC = 1 - (float) k * beta;
  
  for (int i = 0; i < k; ++i)
    _sub.edgeC[i] = beta;
    
}
  
void InteriorTriRule::computeL0() {
  _l0.centerC = 0.5f; 
  _l0.edgeC = vector<float>(k);
  if(k == 3) {
    _l0.centerC = 2/5.0f;
    for(int i = 0; i < k; ++i)
      _l0.edgeC[i] = 1/5.0;
  } else {
    for (int i = 0; i < k; ++i) 
      _l0.edgeC[i] = 0.5/(float)k;
  }
}

void InteriorTriRule::computeL1() {
  _l1.centerC = 0;
  _l1.edgeC = vector<float>(k);
  for (int i = 0; i < k; ++i) 
    _l1.edgeC[i] = 2/(float) k * sin(2.0*M_PI*i/(float)k);
}

void InteriorTriRule::computeL2() {
  _l2.centerC = 0.0;
  _l2.edgeC = vector<float>(k);
  for (int i = 0; i < k; ++i)
    _l2.edgeC[i] = 2/(float) k * cos(2.0*M_PI*i/(float)k);
}

void InteriorTriRule::computeX1() {
  _x1.centerC = 0.0;
  _x1.edgeC = vector<float>(k);
  for (int i = 0; i < k; ++i) 
    _x1.edgeC[i] = sin(2.0*M_PI*i/(float)k);
}

void InteriorTriRule::computeX2() {
  _x2.centerC = 0.0;
  _x2.edgeC = vector<float>(k);
  for (int i = 0; i < k; ++i) 
    _x2.edgeC[i] = cos(2.0*M_PI*i/(float)k);
}

void InteriorTriRule::computeLambda1() 
{ _lambda1 = 3/8.0 + 1/4.0 * cos(2*M_PI/(float)k); }

void InteriorTriRule::computeLambda2()
{  _lambda2 = 3/8.0 + 1/4.0 * cos(2*M_PI/(float)k); }

  
//--------------------------------------------------------------------------------

CreaseTriRule::CreaseTriRule(int k) : k(k) {

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

void CreaseTriRule::computeEdgeSubCoef() {
  float gamma = 1.0f/2.0f - 1.0f/4.0f * cos(M_PI/float(k));
  _edgeSub.c[0] = 3.0/4.0f-gamma; 
  _edgeSub.c[1] = 1.0f/8.0f;
  _edgeSub.c[2] = gamma;
  _edgeSub.c[3] = 1.0/8.0f; 
}

void CreaseTriRule::computeSubCoef() {
  _sub.edgeC = vector<float>(k+1);
  _sub.centerC = 6/8.0;
  _sub.edgeC[0] = 1/8.0;
  _sub.edgeC[k] = 1/8.0;
  for (int i = 1; i < k; ++i)
    _sub.edgeC[i] = 0.0;
}

void CreaseTriRule::computeL0() {
  _l0.edgeC = vector<float>(k+1);
  _l0.centerC = 2/3.0f;
  _l0.edgeC[0] = 1/6.0f;
  _l0.edgeC[k] = 1/6.0f;
  for (int i = 1; i < k; ++i)
    _l0.edgeC[i] = 0.0f;
}

void CreaseTriRule::computeL1() {
  _l1.edgeC = vector<float>(k+1);
  if (k == 1) {
    _l1.centerC = -1.0;
    _l1.edgeC[0] = 0.5;
    _l1.edgeC[1] = 0.5;
  } else {
    float thetak, delta, gamma, theta, a, b, sigma1, sigma2, sigma3;
    computeBoundaryParams(k, thetak, delta,gamma,
			  a, b, theta,
			  sigma1, sigma2,sigma3);
      
    _l1.edgeC[0] = -(2.0f/(float)k)*(( 0.5*a + 1.0f/6.0f)*sigma1 + 0.5*b*sigma3);
    _l1.centerC  = -(2.0f/(float)k)*(( 2.0f/3.0f - a)*sigma1 - b*sigma3);
    _l1.edgeC[k] = _l1.edgeC[0];
      
    for (int i = 1; i < k; i++ )
      _l1.edgeC[i] = (2.0f/(float)k)*sin(thetak*(float)i);
  }
}

void CreaseTriRule::computeL2() {
  _l2.edgeC = vector<float>(k+1);
  _l2.centerC = 0.0;
  _l2.edgeC[0] = 0.5;
  _l2.edgeC[k] = -0.5;
  for (int i = 1; i < k; ++i)
    _l2.edgeC[i] = 0.0;
}

void CreaseTriRule::computeX1() {
  _x1.edgeC = vector<float>(k+1);
    
  if (k == 1) {
    _x1.centerC =  float(-1/3.0);
    _x1.edgeC[0] = float(2.0/3.0);
    _x1.edgeC[1] = float(2.0/3.0);
  } else {
    _x1.centerC = 0.0;
    for (int i = 0; i < k+1; ++i) 
      _x1.edgeC[i] = sin(i * M_PI/(float) k);
  }
}

void CreaseTriRule::computeX2() {
  _x2.edgeC = vector<float>(k+1);
  if (k == 1) {
    _x2.centerC = 0;
    _x2.edgeC[0] = 1.0;
    _x2.edgeC[1] = -1.0;
  } else {
    _x2.centerC = 0.0;
    for (int i = 0; i < k+1; ++i)
      _x2.edgeC[i] = cos(i * M_PI / (float) k);
  }
}

void CreaseTriRule::computeLambda1() {
  if (k == 1) 
    _lambda1 = 0.25;
  else
    _lambda1 = 0.5;
}

void CreaseTriRule::computeLambda2() {
  _lambda2 = 0.5;
}

//--------------------------------------------------------------------------------

ConcaveTriRule::ConcaveTriRule(int k, float theta) : k(k), theta(theta) {

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

void ConcaveTriRule::computeEdgeSubCoef() {
  float gamma = 1.0f/2.0f - 1.0f/4.0f * cos(theta/float(k));
  _edgeSub.c[0] = 3.0/4.0f-gamma; 
  _edgeSub.c[1] = 1.0f/8.0f;
  _edgeSub.c[2] = gamma;
  _edgeSub.c[3] = 1.0/8.0f; 
}

void ConcaveTriRule::computeSubCoef() {
  _sub.edgeC = vector<float>(k+1);
  _sub.centerC = 1.0;
  for(int i = 0; i < k+1; ++i)
    _sub.edgeC[i] = 0.0;
}

void ConcaveTriRule::computeL0() {
  _l0.edgeC = vector<float>(k+1);
  _l0.centerC = 1.0;
  for (int i = 0; i < k+1; ++i) 
    _l0.edgeC[k] = 0.0;
}

void ConcaveTriRule::computeL1() {
  _l1.edgeC = vector<float>(k+1);
  _l1.centerC = -1.0;
  _l1.edgeC[0] = 1.0;
  for (int i = 1; i < k+1; ++i) 
    _l1.edgeC[k] = 0.0;
}

void ConcaveTriRule::computeL2() {
  _l2.edgeC = vector<float>(k+1);
  _l2.centerC = -1.0;
  _l2.edgeC[k] = 1.0;
  for (int i = 0; i < k; ++i) 
    _l2.edgeC[i] = 0.0;
}
  
void ConcaveTriRule::computeX1() {
  float thetak = theta / (float) k;
  _x1.edgeC = vector<float>(k+1);
  _x1.centerC = 0.0;
  for (int i = 0; i < k+1; ++i) 
    _x1.edgeC[i] = sin((k-i)*thetak) / sin(theta);
}

void ConcaveTriRule::computeX2() {
  float thetak = theta / (float) k;
  _x2.edgeC = vector<float>(k+1);
  _x2.centerC = 0.0;
  for (int i = 0; i < k+1; ++i) 
    _x2.edgeC[i] = sin(i * thetak) / sin(theta);
}

void ConcaveTriRule::computeLambda1() {
  _lambda1 = 0.5;
}

void ConcaveTriRule::computeLambda2() {
  _lambda2 = 0.5;
}

//--------------------------------------------------------------------------------

ConvexTriRule::ConvexTriRule(int k, float theta) : k(k), theta(theta) {

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

void ConvexTriRule::computeEdgeSubCoef() {
  float gamma = 1.0f/2.0f - 1.0f/4.0f * cos(theta/float(k));
  _edgeSub.c[0] = 3.0/4.0f-gamma; 
  _edgeSub.c[1] = 1.0f/8.0f;
  _edgeSub.c[2] = gamma;
  _edgeSub.c[3] = 1.0/8.0f; 
}

void ConvexTriRule::computeSubCoef() {
  _sub.edgeC = vector<float>(k+1);
  _sub.centerC = 1.0;
  for(int i = 0; i < k+1; ++i)
    _sub.edgeC[i] = 0.0;
}
  
void ConvexTriRule::computeL0() {
  _l0.edgeC = vector<float>(k+1);
  _l0.centerC = 1.0;
  for (int i = 0; i < k+1; ++i) 
    _l0.edgeC[k] = 0.0;
}

void ConvexTriRule::computeL1() {
  _l1.edgeC = vector<float>(k+1);
  _l1.centerC =  -1.0;
  _l1.edgeC[k] =  1.0;
  for (int i = 0; i < k; ++i)
    _l1.edgeC[i] =  0.0;
}
  
void ConvexTriRule::computeL2() {
  _l2.edgeC = vector<float>(k+1);
  _l2.centerC =  -1.0;
  _l2.edgeC[0] =  1.0;
  for (int i = 1; i < k+1; ++i)
    _l2.edgeC[i] = 0.0;
}

void ConvexTriRule::computeX1() {
  _x1.edgeC = vector<float>(k+1);
  _x1.centerC = 0.0;
  for (int i = 0; i < k+1; ++i) 
    _x1.edgeC[i] = sin(i*theta/(float)k)/sin(theta);
}

void ConvexTriRule::computeX2() {
  _x2.edgeC = vector<float>(k+1);
  _x2.centerC = 0.0;
  for (int i = 0; i < k+1; ++i)
    _x2.edgeC[i] = sin((k-i)*theta/(float)k)/sin(theta);
}

void ConvexTriRule::computeLambda1() 
{ _lambda1 =  0.5; }

void ConvexTriRule::computeLambda2() 
{ _lambda2 = 0.5; }
  

void computeBoundaryParams(float k,
			   float& thetak,
			   float& delta,
			   float& gamma,
			   float& a,
			   float& b,
			   float& theta,
			   float& sigma1, 
			   float& sigma2, 
			   float& sigma3,
			   float th) {
    
    
  if (th == 0) 
    thetak = M_PI / (float) k;
  else
    thetak = th / (float) k;
    
  delta = 1.0f/8.0f;
  gamma = 0.5 - 0.25*cos(thetak);
  theta = acos( 0.5*(0.25 - gamma)/delta);
    
  a = (1.0f - 2.0f*delta - gamma)/(3.0f*(2.0f*delta + gamma - 0.25));
  b = (2.0f/3.0f - a)/cos(k*theta/2.0f);
    
  sigma1 = sin(thetak)/(1.0f - cos(thetak));
  sigma2 = (cos(2.0f*thetak) + 1.0f + 2.0f*cos(thetak))/
    (sin(thetak) + sin(2.0f*thetak));
  sigma3 = cos( k*theta/2.0f)*sin(thetak)/(cos(theta) - cos(thetak));
    
}

