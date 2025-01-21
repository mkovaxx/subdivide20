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

#ifndef __PICKMESHOBJECT_H__
#define __PICKMESHOBJECT_H__

#include "meshobject.h"
#include "pickedstuff.h"
#include "pickobject.h"
#include "sectorinfo.h"

// TODO: rename to PickMeshObject

template <class Mesh> class PickMeshObjectTp : public MeshObjectTp<Mesh>, public PickObject {
  public:
    typedef Mesh MeshType;

    PickMeshObjectTp();
    virtual ~PickMeshObjectTp();
    virtual void rerender();
    virtual void render();
    virtual void renderTL(unsigned char picks);
    virtual void renderPick(unsigned char picks, unsigned int targetCnt, unsigned int shift);
    virtual PickedStuff* doPick(unsigned char r, unsigned char g, unsigned char b, unsigned char picks,
                                unsigned int shift);
    virtual unsigned char& tlRenderMode() { return _tlPicks; }

  private:
    void renderTri();

    void renderTLVertex(bool pick, unsigned int targetCnt, unsigned int shift);
    void renderTLEdge(bool pick, unsigned int targetCnt, unsigned int shift);
    void renderTLSector(bool pick, unsigned int targetCnt, unsigned int shift);
    void renderTLNormal(bool pick, unsigned int targetCnt, unsigned int shift);
    void renderNormal();

    void renderPickVertex(typename Mesh::FaceType* f, EnoType e) {
        glPointSize(10.0);
        glBegin(GL_POINTS);
        glVertex3fv(f->headPos(e, 0));
        glEnd();
        glPointSize(1.0);
    }

    void renderPickEdge(typename Mesh::FaceType* f, EnoType e) {
        glLineWidth(5.0);
        glBegin(GL_LINES);
        glVertex3fv(f->tailPos(e, 0));
        glVertex3fv(f->headPos(e, 0));
        glEnd();
        glLineWidth(1.0);
    }

    void renderPickNormal(typename Mesh::FaceType* f, EnoType e) {
        glLineWidth(5.0);
        glBegin(GL_LINES);

        cvec3f p0 = f->tailVert(f->prevEno(e))->getPos(0);
        cvec3f p1 = f->tailVert(e)->getPos(0);
        cvec3f p2 = f->headVert(e)->getPos(0);

        float fac = 0.5f;
        float l = 1.0f / 3.0f * ((p1 - p0).l2() + (p1 - p2).l2() + (p2 - p0).l2());

        glVertex3fv((float*)p2);
        SectorInfo* si = f->sectorInfo(f->headVno(e));
        if ((si == 0) || (si->modifiedNormal().l1() == 0)) {
            glVertex3fv((float*)(p2 + fac * l * f->normal(f->headVno(e))));
        } else {
            glVertex3fv((float*)(p2 + fac * l * si->modifiedNormal()));
        }

        glEnd();
        glLineWidth(1.0);
    }

    void renderPickSector(typename Mesh::FaceType* f, EnoType e) {
        float alpha = 0.15; // vertex size
        float beta = 0.2;   // edge width
        float gamma = 0.3;  //

        e = f->reverseEno(e);
        cvec3f p0 = f->tailVert(f->prevEno(e))->getPos(0);
        cvec3f p1 = f->tailVert(e)->getPos(0);
        cvec3f p2 = f->headVert(e)->getPos(0);

        cvec3f q0 = alpha * p0 + (1.0f - alpha) * p1;
        cvec3f q1 = (1.0f - alpha) * p1 + alpha * p2;

        cvec3f r0 = gamma * p0 + (1.0f - gamma) * p1;
        cvec3f r1 = (1.0f - gamma) * p1 + gamma * p2;

        cvec3f cp =
            1 / 4.0 *
            ((beta * q0 + (1.0f - beta) * q1) + (1.0f - beta) * q0 + beta * q1 + (1.0f - beta / gamma * alpha) * r0 +
             beta / gamma * alpha * r1 + beta / gamma * alpha * r0 + (1.0f - beta / gamma * alpha) * r1);
        glPointSize(10.0);
        glBegin(GL_POINTS);
        glVertex3fv((float*)cp);
        glEnd();
        glPointSize(1.0);
    }

    virtual PickedStuff::PickElementType pickedElement(unsigned char picks, unsigned char r, unsigned char g,
                                                       unsigned char b, unsigned int shift,
                                                       typename MeshType::EdgeType& e) {
        typedef typename Mesh::EdgeType EdgeType;
        _pickedStuff = PickedStuff(PickedStuff::PICK_NOTHING, 0, 0);

        unsigned int base = 256 / shift;
        unsigned int cnt = base * (base * (b / shift) + (g / shift)) + (r / shift);
        renderPick(picks, cnt, shift);
        e = EdgeType((typename Mesh::FaceType*)_pickedStuff._face, _pickedStuff._eno);
        return _pickedStuff.type();
    }

    void setColor(typename MeshType::FaceType* t, EnoType e, PickedStuff::PickElementType pickElement,
                  unsigned int targetCnt, unsigned int shift) {
        ++_cnt;
        if (_cnt == targetCnt) {
            typedef typename Mesh::EdgeType EdgeType;
            _pickedStuff = PickedStuff(pickElement, t, e);
        }
        unsigned int base = 256 / shift;
        if (_cnt > base * base * base) {
            std::cerr << "WARNING: not enough colors for picking!" << std::endl;
        }

        if (targetCnt == 0) {
            glColor3ub(shift * (_cnt % base), shift * ((_cnt / base) % base), shift * ((_cnt / base / base) % base));
        }
    }

    unsigned int _cnt;
    unsigned char _tlPicks;
    int _listNo;

    PickedStuff _pickedStuff;
};

#include "pickmeshobject.impl.h"

#endif /* __SOBJECT_H__ */
