#ifndef _QV_PLIST_
#define _QV_PLIST_

#include <QvBasic.hpp>

class QvPList {
  public:
    QvPList();
    virtual ~QvPList();
    void append(void* ptr) {
        if (nPtrs + 1 > ptrsSize) {
            expand(nPtrs + 1);
        }
        ptrs[nPtrs++] = ptr;
    }
    int find(const void* ptr) const;
    void remove(int which);
    int getLength() const { return (int)nPtrs; }
    void truncate(int start) { nPtrs = start; }
    void*& operator[](int i) const { return ptrs[i]; }

  private:
    void** ptrs;
    int nPtrs;
    int ptrsSize;
    void setSize(int size) {
        if (size > ptrsSize) {
            expand(size);
        }
        nPtrs = size;
    }
    void expand(int size);
};

#endif /* _QV_PLIST_ */
