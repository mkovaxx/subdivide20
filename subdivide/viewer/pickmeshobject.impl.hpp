// -*- Mode: c++ -*-
// $Id: pickmeshobject.impl.hpp,v 1.7 2001/02/07 15:12:50 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/viewer/pickmeshobject.impl.hpp,v $

#include "glcheck.hpp"
#include "pickableparam.hpp"

#if defined(__APPLE__)
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#endif

#include "sectorinfo.hpp"
#include "tagflatmesh.hpp"
#include "tagivgraph.hpp"

template <class Mesh> PickMeshObjectTp<Mesh>::PickMeshObjectTp() : _tlPicks(PickObject::PICK_ALL), _listNo(-1) { ; }

template <class Mesh> PickMeshObjectTp<Mesh>::~PickMeshObjectTp() {
    if (glIsList(_listNo)) {
        glDeleteLists(_listNo, 2);
    }
}

template <class Mesh> void PickMeshObjectTp<Mesh>::renderTri() {

    if (glIsList(_listNo)) {
        glCallList(_listNo);
        glCheck();
    } else {
        _listNo = glGenLists(2);
        glNewList(_listNo, GL_COMPILE_AND_EXECUTE);
        int lastNoVtx = 3;
        glBegin(GL_TRIANGLES);
        typename Mesh::FaceIterType it;
        for (it = this->getMesh().faceBegin(); it != this->getMesh().faceEnd(); ++it) {

            if ((*it)->isLeaf()) {

                int novtx = (*it)->noVtx();
                if (novtx != lastNoVtx) {
                    glEnd();
                    if (novtx == 3) {
                        glBegin(GL_TRIANGLES);
                    } else if (novtx == 4) {
                        glBegin(GL_QUADS);
                    } else {
                        glBegin(GL_POLYGON);
                    }
                }

                EnoType e = (*it)->directEno(1, CCW);

                for (VnoType v = 0; v < (*it)->noVtx(); ++v, e = (*it)->nextEno(e)) {
                    glNormal3fv((*it)->normal((*it)->headVno(e)));
                    glVertex3fv((*it)->headVert(e)->getPos(it.depth()));
                }
                if ((novtx == 3) || (novtx == 4)) {
                    lastNoVtx = novtx;
                } else {
                    lastNoVtx = 0;
                }
            }
        }
        glEnd();
        glEndList();
    }

    glLineWidth(1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glCheck();
}

template <class Mesh>
void PickMeshObjectTp<Mesh>::renderTLVertex(bool pick, unsigned int targetCnt, unsigned int shift) {
    float alpha = 0.15; // tl edge width

    typename Mesh::FaceIterType it;
    typedef typename Mesh::FaceType FaceType;

    glBegin(GL_TRIANGLES);
    for (it = this->getMesh().faceBegin(0); it != this->getMesh().faceEnd(0); ++it) {
        for (EnoType e = 1; e < (*it)->noVtx() + 1; ++e) {

            EnoType e1 = (*it)->directEno(e);
            cvec3f p0 = (*it)->tailVert((*it)->prevEno(e1))->getPos(0);
            cvec3f p1 = (*it)->tailVert(e1)->getPos(0);
            cvec3f p2 = (*it)->headVert(e1)->getPos(0);

            cvec3f q0 = alpha * p0 + (1.0f - alpha) * p1;
            cvec3f q1 = (1.0f - alpha) * p1 + alpha * p2;

            if (pick) {
                setColor(*it, (*it)->prevEno(e), PickedStuff::PICK_VERTEX, targetCnt, shift);
            } else {
                switch ((*it)->vertexTag((*it)->tailVno(e))) {
                case FaceType::NOTAG_VERTEX:
                    glColor3fv(PickableParam::vertexColor);
                    break;
                case FaceType::CORNER_VERTEX:
                    glColor3fv(PickableParam::cornerVertexColor);
                    break;
                case FaceType::CREASE_VERTEX:
                    glColor3fv(PickableParam::creaseVertexColor);
                    break;
                }
            }
            if (targetCnt == 0) {
                glVertex3fv((float*)q0);
                glVertex3fv((float*)p1);
                glVertex3fv((float*)q1);
            }
        }
    }
    glEnd();
}

template <class Mesh> void PickMeshObjectTp<Mesh>::renderTLEdge(bool pick, unsigned int targetCnt, unsigned int shift) {

    float alpha = 0.15; // vertex size
    float beta = 0.2;   // edge width

    typename Mesh::FaceIterType it;
    typedef typename Mesh::FaceType FaceType;

    glBegin(GL_QUADS);
    for (it = this->getMesh().faceBegin(0); it != this->getMesh().faceEnd(0); ++it) {
        for (EnoType e = 1; e < (*it)->noVtx() + 1; ++e) {

            EnoType e1 = (*it)->directEno(e);
            cvec3f p0 = (*it)->tailVert((*it)->prevEno(e1))->getPos(0);
            cvec3f p1 = (*it)->tailVert(e1)->getPos(0);
            cvec3f p2 = (*it)->headVert(e1)->getPos(0);
            cvec3f p3 = (*it)->headVert((*it)->nextEno(e1))->getPos(0);

            cvec3f q0 = alpha * p0 + (1.0f - alpha) * p1;
            cvec3f q1 = (1.0f - alpha) * p1 + alpha * p2;
            cvec3f q2 = alpha * p1 + (1.0f - alpha) * p2;
            cvec3f q3 = alpha * p3 + (1.0f - alpha) * p2;

            bool f = false;
            if (pick) {
                setColor(*it, e, PickedStuff::PICK_EDGE, targetCnt, shift);
            } else {
                if ((*it)->edgeTag(e) == FaceType::CREASE_EDGE) {
                    glColor3fv(PickableParam::creaseEdgeColor);
                    f = true;
                }
            }
            if (targetCnt == 0) {
                glVertex3fv((float*)(beta * q0 + (1.0f - beta) * q1));
                glVertex3fv((float*)q1);
                glVertex3fv((float*)q2);
                glVertex3fv((float*)(beta * q3 + (1.0f - beta) * q2));
            }
            if (f) {
                f = false;
                glColor3fv(PickableParam::edgeColor);
            }
        }
    }
    glEnd();
}

template <class Mesh>
void PickMeshObjectTp<Mesh>::renderTLSector(bool pick, unsigned int targetCnt, unsigned int shift) {
    float alpha = 0.15; // vertex size
    float beta = 0.2;   // edge width
    float gamma = 0.3;  //
    typename Mesh::FaceIterType it;
    glBegin(GL_QUADS);
    for (it = this->getMesh().faceBegin(0); it != this->getMesh().faceEnd(0); ++it) {
        for (EnoType e = 1; e < (*it)->noVtx() + 1; ++e) {

            EnoType e1 = (*it)->directEno(e);
            cvec3f p0 = (*it)->tailVert((*it)->prevEno(e1))->getPos(0);
            cvec3f p1 = (*it)->tailVert(e1)->getPos(0);
            cvec3f p2 = (*it)->headVert(e1)->getPos(0);

            cvec3f q0 = alpha * p0 + (1.0f - alpha) * p1;
            cvec3f q1 = (1.0f - alpha) * p1 + alpha * p2;

            cvec3f r0 = gamma * p0 + (1.0f - gamma) * p1;
            cvec3f r1 = (1.0f - gamma) * p1 + gamma * p2;

            float s = 0.5 * beta / alpha;
            cvec3f s0 = s * p0 + (1.0f - s) * p1;
            cvec3f s1 = (1.0f - s) * p1 + s * p2;

            if (pick) {
                setColor(*it, (*it)->prevEno(e), PickedStuff::PICK_SECTOR, targetCnt, shift);
            } else {
                switch ((*it)->sectorTag((*it)->tailVno(e1))) {
                case SectorInfo::NOTAG_SECTOR:
                    glColor4fv(PickableParam::sectorColor);
                    break;
                case SectorInfo::CONVEX_SECTOR:
                    glColor4fv(PickableParam::convexSectorColor);
                    break;
                case SectorInfo::CONCAVE_SECTOR:
                    glColor4fv(PickableParam::concaveSectorColor);
                    break;
                }
            }

            if (targetCnt == 0) {
                glVertex3fv((float*)((beta * q0 + (1.0f - beta) * q1)));
                glVertex3fv((float*)((1.0f - beta) * q0 + beta * q1));
                glVertex3fv((float*)((1.0f - beta / gamma * alpha) * r0 + beta / gamma * alpha * r1));
                glVertex3fv((float*)(beta / gamma * alpha * r0 + (1.0f - beta / gamma * alpha) * r1));
            }

            if (pick && (targetCnt == 0)) {
                glColor3f(0.0f, 0.0f, 0.0f);
                glVertex3fv((float*)((1.0f - beta / gamma * alpha) * r0 + beta / gamma * alpha * r1));
                glVertex3fv((float*)(beta / gamma * alpha * r0 + (1.0f - beta / gamma * alpha) * r1));
                glVertex3fv((float*)(beta / s * alpha * s0 + (1.0f - beta / s * alpha) * s1));
                glVertex3fv((float*)((1.0f - beta / s * alpha) * s0 + beta / s * alpha * s1));
            }
        }
    }
    glEnd();
}

template <class Mesh> void PickMeshObjectTp<Mesh>::renderNormal() {
    typename Mesh::FaceIterType it;
    glBegin(GL_LINES);
    for (it = this->getMesh().faceBegin(); it != this->getMesh().faceEnd(); ++it) {
        if ((*it)->isLeaf()) {
            for (EnoType e = 1; e < (*it)->noVtx() + 1; ++e) {

                cvec3f p0 = (*it)->tailVert((*it)->prevEno(e))->getPos(it.depth());
                cvec3f p1 = (*it)->tailVert(e)->getPos(it.depth());
                cvec3f p2 = (*it)->headVert(e)->getPos(it.depth());

                float fac = 0.5f;
                float l = 1.0f / 3.0f * ((p1 - p0).l2() + (p1 - p2).l2() + (p2 - p0).l2());

                glVertex3fv(p2);
                SectorInfo* si = (*it)->sectorInfo((*it)->headVno(e));
                if ((si == 0) || (si->modifiedNormal().l1() == 0)) {
                    glVertex3fv(p2 + fac * l * (*it)->normal((*it)->headVno(e)));
                } else {
                    glVertex3fv(p2 + fac * l * si->modifiedNormal());
                }
            }
        }
    }
    glEnd();
}

template <class Mesh>
void PickMeshObjectTp<Mesh>::renderTLNormal(bool pick, unsigned int targetCnt, unsigned int shift) {
    typename Mesh::FaceIterType it;
    glBegin(GL_LINES);
    for (it = this->getMesh().faceBegin(0); it != this->getMesh().faceEnd(0); ++it) {
        for (EnoType e = 1; e < (*it)->noVtx() + 1; ++e) {

            cvec3f p0 = (*it)->tailVert((*it)->prevEno(e))->getPos(0);
            cvec3f p1 = (*it)->tailVert(e)->getPos(0);
            cvec3f p2 = (*it)->headVert(e)->getPos(0);

            float fac = 0.5f;
            float l = 1.0f / 3.0f * ((p1 - p0).l2() + (p1 - p2).l2() + (p2 - p0).l2());

            if (pick) {
                setColor(*it, e, PickedStuff::PICK_NORMAL, targetCnt, shift);
            }
            glVertex3fv((float*)p2);
            SectorInfo* si = (*it)->sectorInfo((*it)->headVno(e));
            if (targetCnt == 0) {
                if ((si == 0) || (si->modifiedNormal().l1() == 0)) {
                    glVertex3fv((float*)(p2 + fac * l * (*it)->normal((*it)->headVno(e))));
                } else {
                    glVertex3fv((float*)(p2 + fac * l * si->modifiedNormal()));
                }
            }
        }
    }
    glEnd();
}

template <class Mesh>
PickedStuff* PickMeshObjectTp<Mesh>::doPick(unsigned char r, unsigned char g, unsigned char b, unsigned char picks,
                                            unsigned int shift) {

    typename MeshType::EdgeType e;

    pickedElement(picks, r, g, b, shift, e);
    return &_pickedStuff;
}

template <class Mesh>
void PickMeshObjectTp<Mesh>::renderPick(unsigned char picks, unsigned int targetCnt, unsigned int shift) {
    _cnt = 0;

    if (picks & PickedStuff::PICK_VERTEX) {
        renderTLVertex(true, targetCnt, shift);
    }
    if (picks & PickedStuff::PICK_EDGE) {
        renderTLEdge(true, targetCnt, shift);
    }
    if (picks & PickedStuff::PICK_NORMAL) {
        glLineWidth(5.0f);
        renderTLNormal(true, targetCnt, shift);
        glLineWidth(1.0f);
    }
    if (picks & PickedStuff::PICK_SECTOR) {
        renderTLSector(true, targetCnt, shift);
    }
}

template <class Mesh> void PickMeshObjectTp<Mesh>::renderTL(unsigned char picks) {
    typedef typename Mesh::FaceType FaceType;

    glDisable(GL_LIGHTING);

    if (picks & PickedStuff::PICK_VERTEX) {
        glColor3fv(PickableParam::vertexColor);
        renderTLVertex(false, 0, 1); // no pick, initial count = 0 (doesn't matter), shift = 1 (doesn't matter)
    }

    if (picks & PickedStuff::PICK_EDGE) {
        glColor3fv(PickableParam::edgeColor);
        renderTLEdge(false, 0, 1);
    }

    if (picks & PickedStuff::PICK_NORMAL) {
        glLineWidth(2.0);
        glColor3fv(PickableParam::normalColor);
        renderTLNormal(false, 0, 1);
        glLineWidth(1.0);
    }

    if (picks & PickedStuff::PICK_SECTOR) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_SRC_ALPHA);
        glColor4fv(PickableParam::sectorColor);
        renderTLSector(false, 0, 1);
        glDisable(GL_BLEND);
    }

    if (picks != PickedStuff::PICK_NOTHING) {

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glColor3fv(PickableParam::pickColor);
        if (_pickedStuff._face != 0) {
            FaceType* f = (FaceType*)_pickedStuff._face;
            EnoType e = _pickedStuff._eno;
            switch (_pickedStuff.type()) {
            case PickedStuff::PICK_NORMAL:
                renderPickNormal(f, e);
                break;
            case PickedStuff::PICK_SECTOR:
                renderPickSector(f, e);
                break;
            case PickedStuff::PICK_VERTEX:
                renderPickVertex(f, e);
                break;
            case PickedStuff::PICK_EDGE:
                renderPickEdge(f, e);
                break;
            case PickedStuff::PICK_NOTHING:
                break;
            }
        }
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
    }
}

template <class Mesh> void PickMeshObjectTp<Mesh>::rerender() {

    if (glIsList(_listNo)) {
        glDeleteLists(_listNo, 2);
        glCheck();
    }
    glutPostRedisplay();
}

template <class Mesh> void PickMeshObjectTp<Mesh>::render() {
    // the code for hiding the mesh is only showing sectors..
    if (_tlPicks == PickedStuff::PICK_SECTOR) {
        renderTL(PickObject::PICK_ALL);
    } else {
        // this number actually depends on the cameramodel.  Here we fix
        // it to some reasonable imperical level.
        float zOffset = float(0.001 / (2.0 * 100.0));

        // render surface in the back
        glDepthRange(zOffset, 1.0);
        renderTri();
        glDepthRange(0.0, 1.0 - zOffset);

        // render control mesh in the foreground
        renderTL(_tlPicks);
    }
}
