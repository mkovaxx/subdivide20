// -*- Mode: c++ -*-
// $Id: ivgraph.cpp,v 1.7 2000/04/29 14:56:19 biermann Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/src/ivgraph.cpp,v $

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

#include "ivgraph.h"
#include "compat.h"

#include <QvChildList.h>
#include <QvNode.h>
#include <QvSeparator.h>

#include <QvCoordinate3.h>
#include <QvGroup.h>
#include <QvIndexedFaceSet.h>
#include <QvMFLong.h>
#include <QvMFVec3f.h>
#include <QvUnknownNode.h>

#include <QvIn.h>
#include <QvOut.h>
#include <set>

#include "flatmesh.h"
#include "vertex.h"

template <class NodeType> QvNode* findNode(QvNode* node, NodeType* dummy) {
    QvNode* res;
    QvGroup* grp;
    if ((grp = dynamic_cast<QvGroup*>(node))) {
        for (int i = 0; i < grp->getNumChildren(); i++) {
            if ((res = findNode(grp->getChild(i), dummy)))
                return res;
        }
        return 0;
    } else {
        return dynamic_cast<NodeType*>(node);
    }
}

// find nodes of specified name
//
static QvNode* findNamedNode(QvNode* node, const QvName& name) {
    QvNode* res;
    QvGroup* grp;

    if (node->getName() == name)
        return node;

    else if ((grp = dynamic_cast<QvGroup*>(node))) {
        for (int i = 0; i < grp->getNumChildren(); i++)
            if ((res = findNamedNode(grp->getChild(i), name)))
                return res;
        return 0;
    }

    else
        return 0;
}

IvGraph::IvGraph() {
    root = new QvSeparator();
    root->ref();
}

IvGraph::~IvGraph() {
    if (root)
        root->unref();
}

void IvGraph::clear() {
    assert(root);
    root->unref();
    root = new QvSeparator();
    root->ref();
}

void IvGraph::write(char* fileName) {
    QvOut qvOut(fileName);
    qvOut.write(root);
}

bool IvGraph::read(char* fileName) {
    QvIn qvIn(fileName);
    if (qvIn.error())
        return !qvIn.error();
    if (root)
        root->unref();
    root = qvIn.read();
    if (qvIn.error())
        root = 0;
    return !qvIn.error();
}

bool IvGraph::toFlatMesh(FlatMesh* im, bool split) {

    // find coord and index nodes
    im->Cleanup();

    std::set<uint> vset;
    std::pair<uint>::iterator, bool > res;

    QvCoordinate3* coord3 = NULL;
    QvIndexedFaceSet* indexSet = NULL;

    QvNode* pathToCoords = findNode(root, (QvCoordinate3*)0);

    if (pathToCoords == 0) {
        if (root)
            root->unref();
        im->Cleanup();
        return false;
    } else {
        coord3 = (QvCoordinate3*)pathToCoords;
        coord3->ref();
    }

    QvNode* pathToIndices = findNode(root, (QvIndexedFaceSet*)0);

    if (pathToIndices == 0) {
        coord3->unref();
        root->unref();
        im->Cleanup();
        return false;
    } else {
        indexSet = (QvIndexedFaceSet*)pathToIndices;
        indexSet->ref();
    }

    const QvMFVec3f& coords = coord3->point;
    const QvMFLong& indices = indexSet->coordIndex;

    int i;

    for (i = 0; i < coords.num; i++) {
        Vertex* vp = new Vertex();
        vp->setPos(cvec3f(coords.values[3 * i + 0], coords.values[3 * i + 1], coords.values[3 * i + 2]));
        Vertex::ref(vp);
        im->vert_v.push_back(vp);
    }

    i = 0;
    if (indices.num == 0) {
        im->Cleanup();
        return false;
    }

    if (!split) {
        while (i < indices.num) {
            vset.erase(vset.begin(), vset.end());
            if (im->poly_v.size() > 0 && im->poly_v[im->poly_v.size() - 1].novtx() < 3) {
                // skip polygons with < 3 vertices
                std::cerr << "skipping polygon with " << im->poly_v[im->poly_v.size() - 1].novtx() << " vertices ";
                for (uint l = 0; l < im->poly_v[im->poly_v.size() - 1].novtx(); l++)
                    std::cerr << im->index_v[im->poly_v[im->poly_v.size() - 1].start() + l] << " ";
                std::cerr << std::endl;
                im->poly_v[im->poly_v.size() - 1] = IPoly(im->index_v.size(), 0);
                im->triindex_v.push_back(-1);
            } else {
                im->poly_v.push_back(IPoly(im->index_v.size(), 0));
                im->triindex_v.push_back(im->poly_v.size() - 1);
            }
            while (indices.values[i] != -1) {
                res = vset.insert(indices.values[i]);
                // skip duplicate vertices
                if (res.second) {
                    im->poly_v[im->poly_v.size() - 1].incnovtx();
                    im->index_v.push_back(indices.values[i++]);
                } else {
                    std::cerr << "\t\t skipping duplicate vertex " << std::endl;
                    i++;
                }
            }
            i++; // skip -1
        }
        // remove the last polygon if its size is zero
        assert(im->poly_v.size() > 0);
        if (im->poly_v.back().novtx() == 0) {
            if (im->poly_v.size() == 1) {
                im->Cleanup();
                return false;
            }
            im->poly_v.erase(im->poly_v.end());
        }
    } else { // split all polygons into triangles
        int first_vertex, second_vertex, third_vertex;
        while (i < indices.num) {
            while (i < indices.num && indices.values[i] == -1)
                i++;
            first_vertex = indices.values[i];
            i++;
            while (i < indices.num && indices.values[i] != -1 && indices.values[i] == first_vertex)
                i++;
            if (i < indices.num && indices.values[i] != -1)
                second_vertex = indices.values[i];
            else
                continue;
            i++;
            while (i < indices.num && indices.values[i] != -1 &&
                   (indices.values[i] == second_vertex || indices.values[i] == first_vertex))
                i++;
            if (i < indices.num && indices.values[i] != -1)
                third_vertex = indices.values[i];
            else
                continue;
            assert(i < indices.num && indices.values[i] != -1 && first_vertex < indices.num &&
                   second_vertex < indices.num);
            i++;
            im->poly_v.push_back(IPoly(im->index_v.size(), 3));
            im->triindex_v.push_back(im->poly_v.size() - 1);
            im->index_v.push_back(first_vertex);
            im->index_v.push_back(second_vertex);
            im->index_v.push_back(third_vertex);
            while (i < indices.num && indices.values[i] != -1) {
                second_vertex = third_vertex;
                while (i < indices.num && indices.values[i] != -1 &&
                       (indices.values[i] == first_vertex || indices.values[i] == second_vertex))
                    i++;
                if (i < indices.num && indices.values[i] != -1)
                    third_vertex = indices.values[i];
                else
                    break;
                i++;
                im->poly_v.push_back(IPoly(im->index_v.size(), 3));
                im->index_v.push_back(first_vertex);
                im->index_v.push_back(second_vertex);
                im->index_v.push_back(third_vertex);
            }
            assert(indices.values[i] == -1);
        }
    }

    indexSet->unref();
    coord3->unref();

    return true;
}

static void addIndex(QvMFLong& index, long v) {
    index.allocValues(index.num + 1);
    index.values[index.num - 1] = v;
}

QvNode* IvGraph::createCoordinate3Node(const std::vector<Vertex*>& v, int d) {
    QvCoordinate3* node = new QvCoordinate3();

    node->point.allocValues(v.size());

    for (uint i = 0; i < v.size(); ++i) {
        node->point.values[3 * i + 0] = v[i]->getPos(d).x();
        node->point.values[3 * i + 1] = v[i]->getPos(d).y();
        node->point.values[3 * i + 2] = v[i]->getPos(d).z();
    }

    return node;
}

QvNode* IvGraph::createIndexedFaceSet(FlatMesh* flatMesh) {
    QvIndexedFaceSet* node = new QvIndexedFaceSet();

    uint p;
    uint v;
    node->coordIndex.allocValues(0);
    for (p = 0; p < flatMesh->poly_v.size(); ++p) {
        for (v = flatMesh->poly_v[p].start(); v < flatMesh->poly_v[p].start() + flatMesh->poly_v[p].novtx(); ++v)
            addIndex(node->coordIndex, (long)flatMesh->index_v[v]);
        addIndex(node->coordIndex, -1);
    }

    return node;
}

void IvGraph::fromFlatMesh(FlatMesh* flatMesh) {

    if (root)
        root->unref();
    root = new QvSeparator();
    root->ref();

    QvNode* coordinate3Node = createCoordinate3Node(flatMesh->vert_v, flatMesh->vertex_depth);
    QvNode* indexedFaceSetNode = createIndexedFaceSet(flatMesh);

    root->getChildren()->append(coordinate3Node);
    root->getChildren()->append(indexedFaceSetNode);
}
