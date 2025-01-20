// -*- Mode: c++ -*-
// $Id: optdeque.h,v 1.2 2000/04/29 09:27:19 dzorin Exp $
// $Source: /tools/cvs-repos/sig00code/nyuSub/template/optdeque.h,v $

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

#ifndef __OPTDEQUE_H__
#define __OPTDEQUE_H__

#include "compat.h"
#define OPTDEQUE_SIZE 2000
#define MAX_RINGS 100

//: simulate some features of deque avoiding allocation;
//
// this goes into the innermost loop and should be really fast
// multiple erases are not recommended insert is not here yet

template <class T> class optdeque {
  public:
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T& reference;
    typedef const T& const_reference;

    // this perverted way of initializing
    // the buffer results in a
    // 3x speedup of the program; guess why

    optdeque(void)
        : // buffer((T*)malloc(sizeof(T)*OPTDEQUE_SIZE)),
          buffer((T*)&(secret_buffer[0])), start(buffer + OPTDEQUE_SIZE / 2), finish(buffer + OPTDEQUE_SIZE / 2) {}

    ~optdeque(void) { // free(buffer);
    }

    optdeque(const optdeque& q) {
        optdeque::const_iterator it;
        for (it = q.begin(); it != q.end(); ++it)
            push_back(*it);
    }

    optdeque* operator=(const optdeque& q) {
        if ((&q) != this) {
            erase(begin(), end());
            optdeque::const_iterator it;
            for (it = q.begin(); it != q.end(); ++it)
                push_back(*it);
        }
        return this;
    }

    void push_front(const_reference x) {
        assert(start > buffer);

        *(--start) = x;
    }

    void push_back(const_reference x) {
        assert(finish < buffer + OPTDEQUE_SIZE);

        *finish++ = x;
    }

    void pop_back(void) { --finish; }

    void pop_front(void) { start++; }

    void erase(iterator first, iterator last) {
        assert(last <= finish);
        assert(first >= start);
        if (last == finish) {
            finish = first;
            if (start == finish) {
                finish = buffer + OPTDEQUE_SIZE / 2;
                start = buffer + OPTDEQUE_SIZE / 2;
            }
        } else {
            iterator result = first;
            while (last != finish)
                *result++ = *last++;
        }
    }

    const_reference operator[](int n) const { return *(start + n); }
    reference operator[](int n) { return *(start + n); }
    const_iterator begin(void) const { return start; }
    const_iterator end(void) const { return finish; }
    iterator begin(void) { return start; }
    iterator end(void) { return finish; }
    unsigned int size(void) const { return finish - start; }

  private:
    void* secret_buffer[OPTDEQUE_SIZE * (sizeof(T) / 2 + 1)];
    T* buffer;
    iterator start;
    iterator finish;
};

#endif /* __OPTDEQUE_H__ */
