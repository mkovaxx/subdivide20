// -*- Mode: c++ -*-
// $Id: subviewer.cpp,v 1.13 2001/02/07 16:15:04 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/examples/subviewer.cpp,v $

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

#include "compat.hpp"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "general.hpp" // maxdepth

#include "pickviewer.hpp" // viewer class

#include "tagflatmesh.hpp" // flatmesh data structure
#include "tagivgraph.hpp"  // inventor graph

#include "pickablequad.hpp" // quad mesh + rendering + picking
#include "pickabletri.hpp"  // triangle mesh + rendering + picking

#include "quadmanipulator.hpp" // manipulation of quads
#include "trimanipulator.hpp"  // manipulation of triangles

#include "sectorinfo.hpp" // sector information used for call backs

#include <cstring>

#include <GLFW/glfw3.h> // Added for GLFW key constants

int depth = 3;

void init(int argc, char** argv, bool& triMode, TagIvGraph& ivGraph) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " [--mode tri|quad] infile [depth]" << std::endl;
        exit(1);
    }

    // Default mode is triangles
    triMode = true;

    // Parse command-line arguments for --mode and input file
    const char* inputFile = nullptr;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--mode") == 0) {
            if (i + 1 >= argc) {
                std::cerr << "Error: --mode requires an argument (tri|quad)" << std::endl;
                exit(1);
            }
            const char* modeArg = argv[i + 1];
            if (strcmp(modeArg, "tri") == 0) {
                triMode = true;
            } else if (strcmp(modeArg, "quad") == 0) {
                triMode = false;
            } else {
                std::cerr << "Error: invalid mode '" << modeArg << "' (must be 'tri' or 'quad')" << std::endl;
                exit(1);
            }
            i++; // Consumed mode argument value
        } else if (!inputFile) {
            // The first non-option argument is the input file
            inputFile = argv[i];
        } else if (i == argc - 1) {
            // The last argument is the optional depth
            depth = std::min(std::max(atoi(argv[i]), 0), GEN_MAX_DEPTH);
        }
    }

    if (!inputFile) {
        std::cerr << "Error: No input file specified" << std::endl;
        exit(1);
    }

    // Create a modifiable copy of the filename for the read() method
    char* filename = new char[strlen(inputFile) + 1];
    strcpy(filename, inputFile);
    
    bool res = ivGraph.read(filename);
    delete[] filename;
    
    if (!res) {
        std::cerr << "could not read " << inputFile << std::endl;
        exit(1);
    }
}

//----------------------------------------------------------------------------

// we need global variables to remember what we have selected
PickedStuff pickedTriStuff;
PickedStuff pickedQuadStuff;

// call back to toggle ui state
void toggleViewerStateCB(void* o) { ((PickViewer*)o)->toggleState(); }

// flatness higher cb
void triFlatUpCB(void* triObject) {
    TriManipulator tm(pickedTriStuff.face());
    if (pickedTriStuff.type() == PickedStuff::PICK_SECTOR) {
        VnoType vno = tm.headVno(pickedTriStuff.eno());
        if (tm.getVertexTag(vno) == TriManipulator::CORNER_VERTEX) {
            // get the sector information
            SectorInfo* si = tm.getSectorInfo(vno);
            assert(si);
            float f = si->modifiedFlatness();
            f += 0.1;
            f = std::min(f, 1.0f);
            std::cerr << "set flatness: " << f << std::endl;
            f = std::min(f, 1.0f);
            si->setModifiedFlatness(f);
            tm.recomputeNeighbors();
            ((PickableTri*)triObject)->rerender();
        }
    }
}

// flatness higher cb
void quadFlatUpCB(void* quadObject) {
    QuadManipulator tm(pickedQuadStuff.face());
    if (pickedQuadStuff.type() == PickedStuff::PICK_SECTOR) {
        VnoType vno = tm.headVno(pickedQuadStuff.eno());
        if (tm.getVertexTag(vno) == QuadManipulator::CORNER_VERTEX) {
            // get the sector information
            SectorInfo* si = tm.getSectorInfo(vno);
            float f = si->modifiedFlatness();
            f += 0.1;
            f = std::min(f, 1.0f);
            std::cerr << "set flatness: " << f << std::endl;
            f = std::min(f, 1.0f);
            si->setModifiedFlatness(f);
            tm.recomputeNeighbors();
            ((PickableQuad*)quadObject)->rerender();
        }
    }
}

// flatness higher cb
void triFlatDownCB(void* triObject) {
    TriManipulator tm(pickedTriStuff.face());
    if (pickedTriStuff.type() == PickedStuff::PICK_SECTOR) {
        VnoType vno = tm.headVno(pickedTriStuff.eno());
        if (tm.getVertexTag(vno) == TriManipulator::CORNER_VERTEX) {
            // get the sector information
            SectorInfo* si = tm.getSectorInfo(vno);
            float f = si->modifiedFlatness();
            f -= 0.1;
            f = std::max(f, 0.0f);
            std::cerr << "set flatness: " << f << std::endl;
            si->setModifiedFlatness(f);
            tm.recomputeNeighbors();
            ((PickableTri*)triObject)->rerender();
        }
    }
}

// flatness down cb
void quadFlatDownCB(void* quadObject) {
    QuadManipulator tm(pickedQuadStuff.face());
    if (pickedQuadStuff.type() == PickedStuff::PICK_SECTOR) {
        VnoType vno = tm.headVno(pickedQuadStuff.eno());
        if (tm.getVertexTag(vno) == QuadManipulator::CORNER_VERTEX) {
            // get the sector information
            SectorInfo* si = tm.getSectorInfo(vno);
            float f = si->modifiedFlatness();
            f -= 0.1;
            std::cerr << "set flatness: " << f << std::endl;
            f = std::max(f, 0.0f);
            si->setModifiedFlatness(f);
            tm.recomputeNeighbors();
            ((PickableQuad*)quadObject)->rerender();
        }
    }
}

//----------------------------------------------------------------------

// theta higher cb
void triThetaUpCB(void* triObject) {
    TriManipulator tm(pickedTriStuff.face());
    if (pickedTriStuff.type() == PickedStuff::PICK_SECTOR) {
        VnoType vno = tm.headVno(pickedTriStuff.eno());
        if (tm.getVertexTag(vno) == TriManipulator::CORNER_VERTEX) {
            // get the sector information
            SectorInfo* si = tm.getSectorInfo(vno);
            assert(si);
            float f = si->theta();
            f += 0.05;
            if (si->sectorTag() == SectorInfo::CONVEX_SECTOR) {
                f = std::min(float(M_PI), std::max(f, 0.0f));
            } else {
                f = std::min(2.0f * float(M_PI), std::max(f, float(M_PI)));
            }
            std::cerr << "set theta: " << f << std::endl;
            si->setTheta(f);
            tm.recomputeNeighbors();
            ((PickableTri*)triObject)->rerender();
        }
    }
}

// theta higher cb
void quadThetaUpCB(void* quadObject) {
    QuadManipulator tm(pickedQuadStuff.face());
    if (pickedQuadStuff.type() == PickedStuff::PICK_SECTOR) {
        VnoType vno = tm.headVno(pickedQuadStuff.eno());
        if (tm.getVertexTag(vno) == QuadManipulator::CORNER_VERTEX) {
            // get the sector information
            SectorInfo* si = tm.getSectorInfo(vno);
            float f = si->theta();
            f += 0.05;
            if (si->sectorTag() == SectorInfo::CONVEX_SECTOR) {
                f = std::min(float(M_PI), std::max(f, 0.0f));
            } else {
                f = std::min(2.0f * float(M_PI), std::max(f, float(M_PI)));
            }
            std::cerr << "set theta: " << f << std::endl;
            si->setTheta(f);
            tm.recomputeNeighbors();
            ((PickableQuad*)quadObject)->rerender();
        }
    }
}

// theta higher cb
void triThetaDownCB(void* triObject) {
    TriManipulator tm(pickedTriStuff.face());
    if (pickedTriStuff.type() == PickedStuff::PICK_SECTOR) {
        VnoType vno = tm.headVno(pickedTriStuff.eno());
        if (tm.getVertexTag(vno) == TriManipulator::CORNER_VERTEX) {
            // get the sector information
            SectorInfo* si = tm.getSectorInfo(vno);
            float f = si->theta();
            f -= 0.05;
            if (si->sectorTag() == SectorInfo::CONVEX_SECTOR) {
                f = std::min(float(M_PI), std::max(f, 0.0f));
            } else {
                f = std::min(2.0f * float(M_PI), std::max(f, float(M_PI)));
            }
            std::cerr << "set theta: " << f << std::endl;
            si->setTheta(f);
            tm.recomputeNeighbors();
            ((PickableTri*)triObject)->rerender();
        }
    }
}

// theta down cb
void quadThetaDownCB(void* quadObject) {
    QuadManipulator tm(pickedQuadStuff.face());
    if (pickedQuadStuff.type() == PickedStuff::PICK_SECTOR) {
        VnoType vno = tm.headVno(pickedQuadStuff.eno());
        if (tm.getVertexTag(vno) == QuadManipulator::CORNER_VERTEX) {
            // get the sector information
            SectorInfo* si = tm.getSectorInfo(vno);
            float f = si->theta();
            f -= 0.05;
            if (si->sectorTag() == SectorInfo::CONVEX_SECTOR) {
                f = std::min(float(M_PI), std::max(f, 0.0f));
            } else {
                f = std::min(2.0f * float(M_PI), std::max(f, float(M_PI)));
            }
            std::cerr << "set theta: " << f << std::endl;
            si->setTheta(f);
            tm.recomputeNeighbors();
            ((PickableQuad*)quadObject)->rerender();
        }
    }
}

//----------------------------------------------------------------------

void triPickCB(PickedStuff* pickedStuff, void* triObject) {
    pickedTriStuff = *pickedStuff; // remember the picked stuff
    TriManipulator tm(pickedStuff->face());
    bool res = false;
    switch (pickedStuff->type()) {
    case PickedStuff::PICK_VERTEX:
        res = tm.toggleVertex(pickedStuff->eno());
        break;
    case PickedStuff::PICK_EDGE:
        res = tm.toggleEdge(pickedStuff->eno());
        break;
    case PickedStuff::PICK_SECTOR:
        res = tm.toggleSector(pickedStuff->eno());
        break;
    case PickedStuff::PICK_NORMAL:
        break;
    case PickedStuff::PICK_NOTHING:
        break;
    }
    if (res) {
        tm.recomputeNeighbors();
        ((PickableTri*)triObject)->rerender();
    }
}

void quadPickCB(PickedStuff* pickedStuff, void* quadObject) {
    pickedQuadStuff = *pickedStuff; // remember the picked stuff
    QuadManipulator tm(pickedStuff->face());
    bool res = false;
    switch (pickedStuff->type()) {
    case PickedStuff::PICK_VERTEX:
        res = tm.toggleVertex(pickedStuff->eno());
        break;
    case PickedStuff::PICK_EDGE:
        res = tm.toggleEdge(pickedStuff->eno());
        break;
    case PickedStuff::PICK_SECTOR:
        res = tm.toggleSector(pickedStuff->eno());
        break;
    case PickedStuff::PICK_NORMAL:
        break;
    case PickedStuff::PICK_NOTHING:
        break;
    }
    if (res) {
        tm.recomputeNeighbors();
        ((PickableQuad*)quadObject)->rerender();
    }
}

// subdivide a triangular mesh object
void subTriObjectCB(void* o) {
    PickableTri* triMeshObject = (PickableTri*)o;
    TriMesh* tm = &triMeshObject->getMesh();
    tm->subdivide(tm->meshDepth() + 1);
    // clear display lists
    triMeshObject->rerender();

    std::cerr << "subdivide (Loop). depth = " << tm->meshDepth() << std::endl;
}

// subdivide a quadrialteral mesh object
void subQuadObjectCB(void* o) {
    PickableQuad* quadMeshObject = (PickableQuad*)o;
    quadMeshObject->getMesh().subdivide(quadMeshObject->getMesh().meshDepth() + 1);
    // clear display lists
    quadMeshObject->rerender();

    std::cerr << "subdivide (Catmull-Clark). depth = " << quadMeshObject->getMesh().meshDepth() << std::endl;
}

// write the control mesh
void writeTriCtrlCB(void* o) {
    std::cerr << "write control mesh to trimesh.wrl" << std::endl;

    TriMesh triMesh = ((PickableTri*)o)->getMesh();

    // create a flat mesh
    TagFlatMesh tagFlatMesh;
    triMesh.toTagFlatMesh(&tagFlatMesh);

    // create an ivgraph and write to file "trimesh.wrl"
    TagIvGraph tagIvGraph;
    tagIvGraph.fromTagFlatMesh(&tagFlatMesh);
    tagIvGraph.write("trimesh.wrl");
}

// write the control mesh
void writeQuadCtrlCB(void* o) {
    std::cerr << "write control mesh to quadmesh.wrl" << std::endl;

    QuadMesh quadMesh = ((PickableQuad*)o)->getMesh();

    // create a flat mesh
    TagFlatMesh tagFlatMesh;
    quadMesh.toTagFlatMesh(&tagFlatMesh);

    // create an ivgraph and write to file "quadmesh.wrl"
    TagIvGraph tagIvGraph;
    tagIvGraph.fromTagFlatMesh(&tagFlatMesh);
    tagIvGraph.write("quadmesh.wrl");
}

// write the control mesh
void writeTriSubCB(void* o) {
    std::cerr << "write subdivided mesh to trimeshs.wrl" << std::endl;

    TriMesh* leafMesh = ((PickableTri*)o)->getMesh().leafMesh();
    TriMesh leafClone;
    leafClone.setClone(*leafMesh);
    // create a flat mesh
    TagFlatMesh tagFlatMesh;
    leafClone.toTagFlatMesh(&tagFlatMesh);

    // create an ivgraph and write to file "trimeshs.wrl"
    TagIvGraph tagIvGraph;
    tagIvGraph.fromTagFlatMesh(&tagFlatMesh);
    tagIvGraph.write("trimeshs.wrl");

    delete leafMesh;
}

// write the control mesh
void writeQuadSubCB(void* o) {
    std::cerr << "write subdivided mesh to quadmeshs.wrl" << std::endl;

    QuadMesh* leafMesh = ((PickableQuad*)o)->getMesh().leafMesh();
    QuadMesh leafClone;
    leafClone.setClone(*leafMesh);

    // create a flat mesh
    TagFlatMesh tagFlatMesh;
    leafClone.toTagFlatMesh(&tagFlatMesh);

    // create an ivgraph and write to file "tmesh.wrl"
    TagIvGraph tagIvGraph;
    tagIvGraph.fromTagFlatMesh(&tagFlatMesh);
    tagIvGraph.write("quadmeshs.wrl");

    delete leafMesh;
}

void toggleTriViewerStateCB(void* o) {
    unsigned char pickall = PickedStuff::PICK_NOTHING | PickedStuff::PICK_VERTEX | PickedStuff::PICK_EDGE |
                            PickedStuff::PICK_SECTOR | PickedStuff::PICK_NORMAL;

    unsigned char& c = ((PickableTri*)o)->tlRenderMode();
    if (c == pickall) {
        c = PickedStuff::PICK_NOTHING; // show mesh
    } else if (c == PickedStuff::PICK_NOTHING) {
        c = PickedStuff::PICK_SECTOR; // show tl mesh
    } else if (c == PickedStuff::PICK_SECTOR) {
        c = pickall; // show both
    }

    ((PickableTri*)o)->rerender();
}

void toggleQuadViewerStateCB(void* o) {
    unsigned char pickall = PickedStuff::PICK_NOTHING | PickedStuff::PICK_VERTEX | PickedStuff::PICK_EDGE |
                            PickedStuff::PICK_SECTOR | PickedStuff::PICK_NORMAL;

    unsigned char& c = ((PickableQuad*)o)->tlRenderMode();
    if (c == pickall) {
        c = PickedStuff::PICK_NOTHING; // show mesh
    } else if (c == PickedStuff::PICK_NOTHING) {
        c = PickedStuff::PICK_SECTOR; // show tl mesh
    } else if (c == PickedStuff::PICK_SECTOR) {
        c = pickall; // show both
    }

    ((PickableQuad*)o)->rerender();
}

//----------------------------------------------------------------------------

// register Triangle viewer callbacks
void registerTriCB(PickViewer* triViewer, PickableTri* triObject) {
    // toggle call back on space
    triViewer->addKeyCallback(' ', PickViewer::CBPairType(&toggleViewerStateCB, triViewer));
    // subdivide call back on s
    triViewer->addKeyCallback('s', PickViewer::CBPairType(&subTriObjectCB, triObject));
    // write ctrl mesh on w
    triViewer->addKeyCallback('w', PickViewer::CBPairType(&writeTriCtrlCB, triObject));
    // dump subdivided mesh on d
    triViewer->addKeyCallback('d', PickViewer::CBPairType(&writeTriSubCB, triObject));
    // toggle rendered view with o (for 'o'ther view)
    triViewer->addKeyCallback('o', PickViewer::CBPairType(&toggleTriViewerStateCB, triObject));
    triViewer->addPickCallback(triPickCB, triObject);
    // TODO: GLFW Migration - Verify these key mappings are correct for GLFW
    triViewer->addSpecialCallback(GLFW_KEY_UP, PickViewer::CBPairType(&triFlatUpCB, triObject));
    triViewer->addSpecialCallback(GLFW_KEY_DOWN, PickViewer::CBPairType(&triFlatDownCB, triObject));
    triViewer->addSpecialCallback(GLFW_KEY_RIGHT, PickViewer::CBPairType(&triThetaUpCB, triObject));
    triViewer->addSpecialCallback(GLFW_KEY_LEFT, PickViewer::CBPairType(&triThetaDownCB, triObject));
}

// register Quad viewer callbacks
void registerQuadCB(PickViewer* quadViewer, PickableQuad* quadObject) {
    // toggle call back on space
    quadViewer->addKeyCallback(' ', PickViewer::CBPairType(&toggleViewerStateCB, quadViewer));
    // subdivide call back on s
    quadViewer->addKeyCallback('s', PickViewer::CBPairType(&subQuadObjectCB, quadObject));
    // write ctrl mesh on w
    quadViewer->addKeyCallback('w', PickViewer::CBPairType(&writeQuadCtrlCB, quadObject));
    // dump subdivided mesh on d
    quadViewer->addKeyCallback('d', PickViewer::CBPairType(&writeQuadSubCB, quadObject));
    // toggle rendered view with o (for 'o'ther view)
    quadViewer->addKeyCallback('o', PickViewer::CBPairType(&toggleQuadViewerStateCB, quadObject));
    quadViewer->addPickCallback(quadPickCB, quadObject);
    // TODO: GLFW Migration - Verify these key mappings are correct for GLFW
    quadViewer->addSpecialCallback(GLFW_KEY_UP, PickViewer::CBPairType(&quadFlatUpCB, quadObject));
    quadViewer->addSpecialCallback(GLFW_KEY_DOWN, PickViewer::CBPairType(&quadFlatDownCB, quadObject));
    quadViewer->addSpecialCallback(GLFW_KEY_RIGHT, PickViewer::CBPairType(&quadThetaUpCB, quadObject));
    quadViewer->addSpecialCallback(GLFW_KEY_LEFT, PickViewer::CBPairType(&quadThetaDownCB, quadObject));
}

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
    PickViewer::initGL(&argc, argv);

    // process command line arguments and read input file
    bool triMode;
    TagIvGraph ivGraph;
    init(argc, argv, triMode, ivGraph);

    TagFlatMesh tagFlatMesh;
    PickableTri triObject;
    PickableQuad quadObject;

    PickViewer* viewer = nullptr;

    if (triMode) {
        ivGraph.toTagFlatMesh(&tagFlatMesh, true);
        TriMesh triMesh(tagFlatMesh);
        triObject.setMesh(triMesh);

        // render everything but not normals
        triObject.tlRenderMode() = PickedStuff::PICK_VERTEX | PickedStuff::PICK_EDGE | PickedStuff::PICK_SECTOR;

        // compute 0 subdivision level
        triObject.getMesh().subdivide(0);

        viewer = new PickViewer("Subdivide 2.0 :: Triangles");
        viewer->setObject(&triObject);
        viewer->setPos(100, 100);
        registerTriCB(viewer, &triObject);
    } else {
        ivGraph.toTagFlatMesh(&tagFlatMesh, false);
        QuadMesh quadMesh(tagFlatMesh);
        quadObject.setMesh(quadMesh);

        // render everything but not normals
        quadObject.tlRenderMode() = PickedStuff::PICK_VERTEX | PickedStuff::PICK_EDGE | PickedStuff::PICK_SECTOR;

        // compute 0 subdivision level
        quadObject.getMesh().subdivide(0);

        viewer = new PickViewer("Subdivide 2.0 :: Quads");
        viewer->setObject(&quadObject);
        viewer->setPos(100, 100);
        registerQuadCB(viewer, &quadObject);
    }

    // GLFW Main Loop
    if (viewer) {
        viewer->runEventLoop(); // Call the Viewer's main loop method
        delete viewer; // Clean up the viewer object
        viewer = nullptr;
    }

    glfwTerminate(); // Terminate GLFW
    return 0;
}
