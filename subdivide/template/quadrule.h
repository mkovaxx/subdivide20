// -*- Mode: c++ -*-
// $Id: quadrule.h,v 1.3 2000/04/29 09:27:19 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/quadrule.h,v $

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

#ifndef __QUADRULE_H__
#define __QUADRULE_H__

#include "compat.h"
#include <vector>

class QuadEdgeCoef {
  public:
    float cf[6];
    void print() const {
        std::cerr << "c = { " << cf[0] << ", " << cf[1] << ", " << cf[2] << ", " << cf[3] << ", " << cf[4] << ", "
                  << cf[5] << "} " << std::endl;
    }
};

class QuadCoef {
  public:
    float centerC;
    std::vector<float> edgeC;
    std::vector<float> faceC;
    float dot(const QuadCoef& coef) const;
    float dotOne() const;
    void print() const {
        std::cerr << "{ centerC = " << centerC;
        uint i;
        for (i = 0; i < edgeC.size(); ++i) {
            std::cerr << ", " << edgeC[i];
        }
        std::cerr << ". faceC = ";
        for (i = 0; i < faceC.size(); ++i) {
            std::cerr << ", " << faceC[i];
        }
        std::cerr << "} " << std::endl;
    }
};

// Base class for subdivision rules for quadrilateral schemes.
// Different rules (interior, smooth boundary, concave/convex corner)
// derive from this.
// each rule comes with 3 pairs of left and right eigenvectors
// x0, x1, x2 and l0, l1, l2; x0 is trivial (constant 1) and
// need not be computed explicitly.

class QuadRule {
  public:
    typedef QuadCoef CoefType;
    typedef QuadEdgeCoef EdgeCoefType;

    QuadRule() {
        _creaseSub.cf[0] = 0.5f;
        _creaseSub.cf[1] = 0.0f;
        _creaseSub.cf[2] = 0.0f;
        _creaseSub.cf[3] = 0.5f;
        _creaseSub.cf[4] = 0.0f;
        _creaseSub.cf[5] = 0.0f;
    }
    virtual ~QuadRule() { ; }
    const CoefType& subCoef() const { return _sub; }

    const EdgeCoefType& creaseSubCoef() const { return _creaseSub; }
    const EdgeCoefType& edgeSubCoef() const { return _edgeSub; }

    const CoefType& l0() const { return _l0; }
    const CoefType& l1() const { return _l1; }
    const CoefType& l2() const { return _l2; }

    const CoefType& x1() const { return _x1; }
    const CoefType& x2() const { return _x2; }

    float lambda1() const { return _lambda1; }
    float lambda2() const { return _lambda2; }

    virtual void printName() const { std::cerr << "QuadRule" << std::endl; }

  protected:
    CoefType _sub, _l0, _l1, _l2, _x1, _x2;
    EdgeCoefType _edgeSub, _creaseSub;
    float _lambda1, _lambda2;
};

class InteriorQuadRule : public QuadRule {
  public:
    InteriorQuadRule(int k);
    virtual void printName() const { std::cerr << "InteriorQuadRule" << std::endl; }

  private:
    void computeSubCoef();
    void computeEdgeSubCoef();
    void computeL0();
    void computeL1();
    void computeL2();
    void computeX1();
    void computeX2();
    void computeLambda1();
    void computeLambda2();
    int k;
};

class CreaseQuadRule : public QuadRule {
  public:
    CreaseQuadRule(int k);
    virtual ~CreaseQuadRule() { ; }
    virtual void printName() const { std::cerr << "CreaseQuadRule" << std::endl; }

  private:
    void computeSubCoef();
    void computeEdgeSubCoef();
    void computeL0();
    void computeL1();
    void computeL2();
    void computeX1();
    void computeX2();
    void computeLambda1();
    void computeLambda2();
    int k;
};

class ConvexQuadRule : public QuadRule {
  public:
    ConvexQuadRule(int k, float theta);
    virtual ~ConvexQuadRule() { ; }
    virtual void printName() const { std::cerr << "ConvexQuadRule" << std::endl; }
    float getTheta() { return theta; }

  private:
    void computeSubCoef();
    void computeEdgeSubCoef();
    void computeL0();
    void computeL1();
    void computeL2();
    void computeX1();
    void computeX2();
    void computeLambda1();
    void computeLambda2();
    int k;
    float theta;
};

class ConcaveQuadRule : public QuadRule {
  public:
    ConcaveQuadRule(int k, float theta);
    virtual ~ConcaveQuadRule() { ; }
    virtual void printName() const { std::cerr << "ConcaveQuadRule" << std::endl; }
    float getTheta() { return theta; }

  private:
    void computeSubCoef();
    void computeEdgeSubCoef();
    void computeL0();
    void computeL1();
    void computeL2();
    void computeX1();
    void computeX2();
    void computeLambda1();
    void computeLambda2();
    int k;
    float theta;
};

//-------------------------------------------------------
//
// Rule tables: lazy tables, hold a collection of rules

class InteriorQuadRuleTable {
  public:
    InteriorQuadRuleTable() { ; }
    ~InteriorQuadRuleTable() {
        for (uint i = 0; i < ruleVec.size(); ++i) {
            delete ruleVec[i];
        }
    }

    InteriorQuadRule* getRule(uint k) {
        if (k >= ruleVec.size()) {
            for (uint i = ruleVec.size(); i < k + 1; ++i) {
                ruleVec.push_back(0);
            }
        }

        if (ruleVec[k] == 0) {
            ruleVec[k] = new InteriorQuadRule(k);
        }

        return ruleVec[k];
    }

  private:
    std::vector<InteriorQuadRule*> ruleVec;
};

class CreaseQuadRuleTable {
  public:
    CreaseQuadRuleTable() { ; }
    ~CreaseQuadRuleTable() {
        for (uint i = 0; i < ruleVec.size(); ++i) {
            delete ruleVec[i];
        }
    }

    CreaseQuadRule* getRule(uint k) {
        if (k >= ruleVec.size()) {
            for (uint i = ruleVec.size(); i < k + 1; ++i) {
                ruleVec.push_back(0);
            }
        }

        if (ruleVec[k] == 0) {
            ruleVec[k] = new CreaseQuadRule(k);
        }

        return ruleVec[k];
    }

  private:
    std::vector<CreaseQuadRule*> ruleVec;
};

class ConvexQuadRuleTable {
  public:
    ConvexQuadRuleTable() { ; }
    ~ConvexQuadRuleTable() {
        for (uint i = 0; i < ruleVec.size(); ++i) {
            delete ruleVec[i];
        }
    }

    ConvexQuadRule* getRule(uint k, float theta) {
        if (k >= ruleVec.size()) {
            for (uint i = ruleVec.size(); i < k + 1; ++i) {
                ruleVec.push_back(0);
            }
        }

        if (ruleVec[k] == 0) {
            ruleVec[k] = new ConvexQuadRule(k, theta);
        } else if (ruleVec[k]->getTheta() != theta) {
            delete ruleVec[k];
            ruleVec[k] = new ConvexQuadRule(k, theta);
        }
        return ruleVec[k];
    }

  private:
    std::vector<ConvexQuadRule*> ruleVec;
};

class ConcaveQuadRuleTable {
  public:
    ConcaveQuadRuleTable() { ; }
    ~ConcaveQuadRuleTable() {
        for (uint i = 0; i < ruleVec.size(); ++i) {
            delete ruleVec[i];
        }
    }

    ConcaveQuadRule* getRule(uint k, float theta) {
        if (k >= ruleVec.size()) {
            for (uint i = ruleVec.size(); i < k + 1; ++i) {
                ruleVec.push_back(0);
            }
        }

        if (ruleVec[k] == 0) {
            ruleVec[k] = new ConcaveQuadRule(k, theta);
        } else if (ruleVec[k]->getTheta() != theta) {
            delete ruleVec[k];
            ruleVec[k] = new ConcaveQuadRule(k, theta);
        }
        return ruleVec[k];
    }

  private:
    std::vector<ConcaveQuadRule*> ruleVec;
};

#endif /* __QUADRULE_H__ */
