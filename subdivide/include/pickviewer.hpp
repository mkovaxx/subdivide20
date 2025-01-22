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

#ifndef __PICKVIEWER_H__
#define __PICKVIEWER_H__

#include "compat.hpp"
#include <map>

#include "ballviewer.hpp"
#include "pickobject.hpp"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class PickViewer : public BallViewer {

  public:
    PickViewer(char* name = 0, int w = 512, int h = 512)
        : BallViewer(name, w, h), _pickCB(0), _pickedStuff(0), _uiState(MOVE_STATE) {
        ;
    }
    virtual ~PickViewer() { ; }

    typedef void* DataType;
    typedef void (*CBFuncType)(DataType);

    // use this type to pass callbacks to  registration functions
    typedef std::pair<CBFuncType, DataType> CBPairType;

    // keyboard callback registration
    void addKeyCallback(unsigned char k, CBPairType cbPair) { _cbMap[k] = cbPair; }

    // picking callback registration
    void addPickCallback(void (*pickCB)(PickedStuff*, void*), void* data) {
        _pickCB = pickCB;
        _pickData = data;
    }

    // special key callback
    void addSpecialCallback(int k, CBPairType cbPair) { _specialMap[k] = cbPair; }

    // toggle the interface between picking and camera manipulation
    void toggleState() {
        _uiState = (_uiState == PICK_STATE) ? MOVE_STATE : PICK_STATE;
        switch (_uiState) {
        case PICK_STATE:
            std::cerr << "pickState" << std::endl;
            break;
        case MOVE_STATE:
            std::cerr << "moveState" << std::endl;
            break;
        }
    }

  protected:
    virtual void key(unsigned char k, int x, int y);
    virtual void pick(GLint x, GLint y);
    virtual void mouse(int button, int state, int x, int y);
    virtual void specialKey(int k, int x, int y);

  private:
    void (*_pickCB)(PickedStuff*, void*);
    PickedStuff* _pickedStuff;
    void* _pickData;

    std::map<unsigned char, CBPairType> _cbMap;
    std::map<int, CBPairType> _specialMap;

    typedef enum { PICK_STATE, MOVE_STATE } UIStateType;
    UIStateType _uiState;
};

#endif /* __PICKVIEWER_H__ */
