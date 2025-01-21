// -*- Mode: c++ -*-
// $Id: trirule.h,v 1.3 2000/04/29 09:27:20 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/trirule.h,v $

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

#ifndef __TRIRULE_H__
#define __TRIRULE_H__

#include "compat.h"
#include <vector>

class EdgeCoef {
  public:
    float c[4];
    void print() const {
        std::cerr << "c = { " << c[0] << ", " << c[1] << ", " << c[2] << ", " << c[3] << "}" << std::endl;
    }
};

class Coef {
  public:
    float centerC;
    std::vector<float> edgeC;
    float dot(const Coef& coef) const;
    float dotOne() const;
    void print() const {
        std::cerr << "{ centerC = " << centerC;
        for (uint i = 0; i < edgeC.size(); ++i)
            std::cerr << ", " << edgeC[i];
        std::cerr << "} " << std::endl;
    }
};

// Base class for subdivision rules for triangular schemes.
// Different rules (interior, smooth boundary, concave/convex corner)
// derive from this.
// each rule comes with 3 pairs of left and right eigenvectors
// x0, x1, x2 and l0, l1, l2; x0 is trivial (constant 1) and
// need not be computed explicitly.

class TriRule {
  public:
    typedef Coef CoefType;
    typedef EdgeCoef EdgeCoefType;

    TriRule() {
        _creaseSub.c[0] = 0.5;
        _creaseSub.c[1] = 0.0f;
        _creaseSub.c[2] = 0.5f;
        _creaseSub.c[3] = 0.0f;
    }
    virtual ~TriRule() { ; }

    const Coef& subCoef() const { return _sub; }

    const EdgeCoef& creaseSubCoef() const { return _creaseSub; }
    const EdgeCoef& edgeSubCoef() const { return _edgeSub; }

    const Coef& l0() const { return _l0; }
    const Coef& l1() const { return _l1; }
    const Coef& l2() const { return _l2; }

    const Coef& x1() const { return _x1; }
    const Coef& x2() const { return _x2; }

    float lambda1() const { return _lambda1; }
    float lambda2() const { return _lambda2; }

    virtual void printName() const { std::cerr << "TriRule" << std::endl; }

  protected:
    Coef _sub, _l0, _l1, _l2, _x1, _x2;
    EdgeCoef _edgeSub, _creaseSub;
    float _lambda1, _lambda2;
};

// various rules for interior, crease, convex and concave vertices
//

class InteriorTriRule : public TriRule {
  public:
    InteriorTriRule(int k);
    virtual ~InteriorTriRule() { ; }
    virtual void printName() const { std::cerr << "InteriorTriRule" << std::endl; }

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

class CreaseTriRule : public TriRule {
  public:
    CreaseTriRule(int k);
    ~CreaseTriRule() { ; }
    virtual void printName() const { std::cerr << "CreaseTriRule" << std::endl; }

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

class ConvexTriRule : public TriRule {
  public:
    ConvexTriRule(int k, float theta);
    virtual ~ConvexTriRule() { ; }
    virtual void printName() const { std::cerr << "ConvexTriRule" << std::endl; }
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

class ConcaveTriRule : public TriRule {
  public:
    ConcaveTriRule(int k, float theta);
    virtual ~ConcaveTriRule() { ; }
    virtual void printName() const { std::cerr << "ConcaveTriRule" << std::endl; }
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

class InteriorTriRuleTable {
  public:
    InteriorTriRuleTable() { ; }
    ~InteriorTriRuleTable() {
        for (uint i = 0; i < ruleVec.size(); ++i)
            delete ruleVec[i];
    }

    InteriorTriRule* getRule(uint k) {
        if (k >= ruleVec.size()) {
            for (uint i = ruleVec.size(); i < k + 1; ++i)
                ruleVec.push_back(0);
        }
        if (ruleVec[k] == 0)
            ruleVec[k] = new InteriorTriRule(k);

        return ruleVec[k];
    }

  private:
    std::vector<InteriorTriRule*> ruleVec;
};

class CreaseTriRuleTable {
  public:
    CreaseTriRuleTable() { ; }
    ~CreaseTriRuleTable() {
        for (uint i = 0; i < ruleVec.size(); ++i)
            delete ruleVec[i];
    }

    CreaseTriRule* getRule(uint k) {
        if (k >= ruleVec.size())
            for (uint i = ruleVec.size(); i < k + 1; ++i)
                ruleVec.push_back(0);

        if (ruleVec[k] == 0)
            ruleVec[k] = new CreaseTriRule(k);

        return ruleVec[k];
    }

  private:
    std::vector<CreaseTriRule*> ruleVec;
};

class ConvexTriRuleTable {
  public:
    ConvexTriRuleTable() { ; }
    ~ConvexTriRuleTable() {
        for (uint i = 0; i < ruleVec.size(); ++i)
            delete ruleVec[i];
    }

    ConvexTriRule* getRule(uint k, float theta) {
        if (k >= ruleVec.size())
            for (uint i = ruleVec.size(); i < k + 1; ++i)
                ruleVec.push_back(0);

        if (ruleVec[k] == 0)
            ruleVec[k] = new ConvexTriRule(k, theta);
        else if (ruleVec[k]->getTheta() != theta) {
            delete ruleVec[k];
            ruleVec[k] = new ConvexTriRule(k, theta);
        }
        return ruleVec[k];
    }

  private:
    std::vector<ConvexTriRule*> ruleVec;
};

class ConcaveTriRuleTable {
  public:
    ConcaveTriRuleTable() { ; }
    ~ConcaveTriRuleTable() {
        for (uint i = 0; i < ruleVec.size(); ++i)
            delete ruleVec[i];
    }

    ConcaveTriRule* getRule(uint k, float theta) {
        if (k >= ruleVec.size())
            for (uint i = ruleVec.size(); i < k + 1; ++i)
                ruleVec.push_back(0);

        if (ruleVec[k] == 0)
            ruleVec[k] = new ConcaveTriRule(k, theta);
        else if (ruleVec[k]->getTheta() != theta) {
            delete ruleVec[k];
            ruleVec[k] = new ConcaveTriRule(k, theta);
        }
        return ruleVec[k];
    }

  private:
    std::vector<ConcaveTriRule*> ruleVec;
};

#endif /* __TRIRULE_H__ */
