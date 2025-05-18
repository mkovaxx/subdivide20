#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "compat.hpp"

#include <deque.h>

#include "ballviewer.hpp"
#include "general.hpp"
#include "geoobject.hpp"
#include "subtri.hpp"
#include "tagflatmesh.hpp"
#include "tagivgraph.hpp"
#include "tagmesh.hpp"

#include "timer.hpp"

#define MAX_VALENCE 20
#define SIN120 0.86602540378444f

typedef TagMeshTp<Tri> MeshType;

class PointRing {
  public:
    PointRing() : _start(0), _noPts(0){};
    int noPts() const { return _noPts; }

    PointRing(const PointRing& pr) {
        _start = pr._start;
        _center = pr._center;
        for (int i = 0; i < pr.noPts(); ++i) {
            addPoint(pr._ring[i]);
        }
    }

    PointRing& operator=(const PointRing& pr) {
        _start = pr._start;
        _center = pr._center;
        _noPts = pr._noPts;
        for (int i = 0; i < MAX_VALENCE; ++i) {
            _ring[i] = pr._ring[i];
        }
        return (*this);
    }

    const cvec3f& c() const { return _center; }
    cvec3f n() const {
        cvec3f sa, ca;
        if (noPts() == 6) {
            for (int i = 0; i < noPts(); ++i) {
                sa += s6[i] * p(i);
                ca += c6[i] * p(i);
            }
        } else {
            for (int i = 0; i < noPts(); ++i) {
                sa += sin(2.0f * M_PI / float(_noPts) * float(i)) * p(i);
                ca += cos(2.0f * M_PI / float(_noPts) * float(i)) * p(i);
            }
        }
        return sa.cross(ca).dir();
    }

    const cvec3f& pt(int i) const { return _ring[(_start + i + noPts()) % noPts()]; }

    void subdivide(PointRing& r) const {
        r._noPts = noPts();
        r._start = _start;
        cvec3f a(0);
        for (int i = 0; i < noPts(); ++i) {
            r.p(i) = (1.0f / 8.0f * (p(i - 1) + p(i + 1)) + 3.0f / 8.0f * (_center + p(i)));
            a += p(i);
        }
        float beta = 3.0f / 8.0f / float(noPts());
        r._center = beta * a + (1.0f - beta * float(noPts())) * _center;
    }

    void setStart(int i) {
        assert(i < noPts());
        _start = (i + _start + noPts()) % noPts();
    }

    void assignStart(int i) { _start = (i + noPts()) % noPts(); }

    int getStart() const { return _start; }
    void setCenter(const cvec3f& c) { _center = c; }
    void addPoint(const cvec3f& p) {
        _ring[_noPts] = p;
        ++_noPts;
        assert(_noPts < MAX_VALENCE);
    }

    void print() const {
        std::cerr << _center << "  " << std::endl;
        for (int i = 0; i < noPts(); ++i) {
            std::cerr << "\t" << _ring[i] << std::endl;
        }
        std::cerr << std::endl;
    }

    void clear() {
        _noPts = 0;
        _start = 0;
    }

  private:
    int _noPts;
    int _start;

    cvec3f _center;
    cvec3f _ring[MAX_VALENCE];

    cvec3f& p(int i) { return _ring[(i + noPts()) % noPts()]; }

    const cvec3f& p(int i) const { return _ring[(i + noPts()) % noPts()]; }

  public:
    static const float s6[6];
    static const float c6[6];
};

const float PointRing::s6[6] = {0, SIN120, SIN120, 0.0000, -SIN120, -SIN120};
const float PointRing::c6[6] = {1.0000, 0.5000, -0.5000, -1.0000, -0.5000, 0.5000};

// class for upper left matrices
//
class UL {
  public:
    UL(int k) : _n(k) { _f = new cvec3f[k * (k + 1) / 2]; }
    virtual ~UL() { delete[] _f; }

    void clear(cvec3f f = cvec3f(-99.0f, -99.0f, -99.0f)) {
        for (int i = 0; i < _n * (_n + 1) / 2; ++i) {
            _f[i] = f;
        }
    }

    UL(const UL&) { assert(0); }
    UL& operator=(const UL&) {
        assert(0);
        return (*this);
    }

    cvec3f& operator()(int i, int j) {
        assert(i >= 0);
        assert(j >= 0);
        assert(i + j < size());
        return _f[i * _n - i * (i - 1) / 2 + j];
    }

    cvec3f operator()(int i, int j) const {
        assert(i + j < size());
        return _f[i * _n - i * (i - 1) / 2 + j];
    }

    int size() const { return _n; }

    void print() const {
        for (int i = 0; i < size(); ++i) {
            for (int j = 0; (j < size() - i); ++j) {
                if ((*this)(i, j).x() == -99) {
                    std::cerr << ". ";
                } else {
                    std::cerr << (*this)(i, j) << ",   ";
                }
            }
            std::cerr << std::endl;
        }
    }

    cvec3f average(int i, int j, float c, const float w[6]) const {
        assert((i > 0) && (j > 0) && (i + j < size() - 1));
        return c * (*this)(i, j) + w[0] * (*this)(i, j + 1) + w[1] * (*this)(i - 1, j + 1) + w[2] * (*this)(i - 1, j) +
               w[3] * (*this)(i, j - 1) + w[4] * (*this)(i + 1, j - 1) + w[5] * (*this)(i + 1, j);
    }

  private:
    int _n;
    cvec3f* _f;
};

// class for loop subdivision
//
class LoopUL : public UL {
  public:
    LoopUL(int k) : UL(k) { ; }
    virtual ~LoopUL() { ; }
    void upsample(LoopUL& m1) const {
        const LoopUL& m0(*this);

        int i;

        assert(m1.size() == 2 * (m0.size() - 2));
        // even points

        for (i = 1; i < m0.size() - 1; ++i) {
            for (int j = 1; j < m0.size() - i - 1; ++j) {
                m1(2 * i - 1, 2 * j - 1) = m0.vertexPoint(i, j);
            }
        }

        for (i = 1; i < m0.size() - 1; ++i) {
            for (int j = 0; j < m0.size() - i - 1; ++j) {
                m1(2 * i - 1, 2 * j) = m0.edgeHPoint(i, j);
            }
        }

        for (i = 0; i < m0.size() - 2; ++i) {
            for (int j = 1; j < m0.size() - i - 1; ++j) {
                m1(2 * i, 2 * j - 1) = m0.edgeVPoint(i, j);
            }
        }

        for (i = 0; i < m0.size() - 2; ++i) {
            for (int j = 0; j < m0.size() - i - 2; ++j) {
                m1(2 * i, 2 * j) = m0.edgeDPoint(i, j);
            }
        }

        _r0.subdivide(m1._r0);
        _r1.subdivide(m1._r1);
        _r2.subdivide(m1._r2);

        // kill ...
        m1(0, 0) = cvec3f(-99.0f, -99.0f, -99.0f);
        m1(1, 0) = cvec3f(-99.0f, -99.0f, -99.0f);
        m1(0, 1) = cvec3f(-99.0f, -99.0f, -99.0f);
        m1(0, m1.size() - 1) = cvec3f(-99.0f, -99.0f, -99.0f);
        m1(0, m1.size() - 2) = cvec3f(-99.0f, -99.0f, -99.0f);
        m1(m1.size() - 2, 0) = cvec3f(-99.0f, -99.0f, -99.0f);
        m1(m1.size() - 2, 1) = cvec3f(-99.0f, -99.0f, -99.0f);
        m1(m1.size() - 1, 0) = cvec3f(-99.0f, -99.0f, -99.0f);
        m1(1, m1.size() - 2) = cvec3f(-99.0f, -99.0f, -99.0f);
        // fix ...
        m1(1, 1) = m1._r0.c();
        m1(0, 2) = m1._r0.pt(2);
        m1(2, 0) = m1._r0.pt(-1);

        // fix ...
        m1(m1.size() - 3, 1) = m1._r2.c();
        m1(m1.size() - 3, 0) = m1._r2.pt(2);
        m1(m1.size() - 3, 2) = m1._r2.pt(-1);

        // fix ...
        m1(1, m1.size() - 3) = m1._r1.c();
        m1(2, m1.size() - 3) = m1._r1.pt(2);
        m1(0, m1.size() - 3) = m1._r1.pt(-1);
    }

    cvec3f edgeDPoint(int i, int j) const {
        return 3.0f / 8.0f * ((*this)(i + 1, j) + (*this)(i, j + 1)) +
               1.0f / 8.0f * ((*this)(i, j) + (*this)(i + 1, j + 1));
    }

    cvec3f edgeVPoint(int i, int j) const {
        return 3.0f / 8.0f * ((*this)(i, j) + (*this)(i + 1, j)) +
               1.0f / 8.0f * ((*this)(i + 1, j - 1) + (*this)(i, j + 1));
    }

    cvec3f edgeHPoint(int i, int j) const {
        return 3.0f / 8.0f * ((*this)(i, j) + (*this)(i, j + 1)) +
               1.0f / 8.0f * ((*this)(i + 1, j) + (*this)(i - 1, j + 1));
    }

    cvec3f vertexPoint(int i, int j) const {
        return 5.0 / 8.0f * (*this)(i, j) + 3.0 / 8.0f / 6.0f *
                                                ((*this)(i, j - 1) + (*this)(i - 1, j) + (*this)(i - 1, j + 1) +
                                                 (*this)(i, j + 1) + (*this)(i + 1, j) + (*this)(i + 1, j - 1));
    }

    PointRing _r0, _r1, _r2;
};

deque<LoopUL*> ulvec;
LoopUL* normals;

void createULVec(int l) {
    ulvec.push_back(new LoopUL(5));
    for (int i = 0; i < l; ++i) {
        ulvec.push_front(new LoopUL(2 * ulvec.front()->size() - 4));
    }
    normals = new LoopUL(ulvec.front()->size());
}

void deleteULVec() {
    for (int i = 0; i < ulvec.size(); ++i) {
        delete ulvec[i];
    }
    delete normals;
}

void subdiv(int l, LoopUL& m0) {
    if (l > 0) {
        LoopUL& m1(*ulvec[l - 1]);
        m0.upsample(m1);
        subdiv(l - 1, m1);
    } else {
        int i;

        // compute normals...
        for (i = 1; i < m0.size() - 1; ++i) {
            for (int j = 1; j < m0.size() - i - 1; ++j) {
                cvec3f t0 = m0.average(i, j, 0.0f, PointRing::s6);
                cvec3f t1 = m0.average(i, j, 0.0f, PointRing::c6);
                (*normals)(i, j) = t0.cross(t1).dir();
            }
        }

        (*normals)(1, 1) = m0._r0.n();
        (*normals)(1, m0.size() - 3) = m0._r1.n();
        (*normals)(m0.size() - 3, 1) = m0._r2.n();
        ;

        for (i = 1; i < m0.size() - 2; ++i) {
            for (int j = 1; j < m0.size() - i - 2; ++j) {
                cvec3f p0 = m0(i, j);
                cvec3f p1 = m0(i + 1, j);
                cvec3f p2 = m0(i, j + 1);
                cvec3f n0 = (*normals)(i, j);
                cvec3f n1 = (*normals)(i + 1, j);
                cvec3f n2 = (*normals)(i, j + 1);
                glNormal3fv(n2);
                glVertex3fv(p2);
                glNormal3fv(n1);
                glVertex3fv(p1);
                glNormal3fv(n0);
                glVertex3fv(p0);
            }
        }
        for (i = 1; i < m0.size() - 2; ++i) {
            for (int j = 1; j < m0.size() - i - 3; ++j) {
                cvec3f p0 = m0(i + 1, j);
                cvec3f p1 = m0(i + 1, j + 1);
                cvec3f p2 = m0(i, j + 1);
                cvec3f n0 = (*normals)(i + 1, j);
                cvec3f n1 = (*normals)(i + 1, j + 1);
                cvec3f n2 = (*normals)(i, j + 1);
                glNormal3fv(n2);
                glVertex3fv(p2);
                glNormal3fv(n1);
                glVertex3fv(p1);
                glNormal3fv(n0);
                glVertex3fv(p0);
            }
        }
    }
}

void subdivRings(int l, const PointRing& r0, const PointRing& r1, const PointRing& r2) {

    assert((r0.pt(0) - r2.c()).l2() < 0.001);
    assert((r0.pt(1) - r1.c()).l2() < 0.001);

    assert((r1.pt(0) - r0.c()).l2() < 0.001);
    assert((r1.pt(1) - r2.c()).l2() < 0.001);

    assert((r2.pt(0) - r1.c()).l2() < 0.001);
    assert((r2.pt(1) - r0.c()).l2() < 0.001);

    LoopUL& m0(*ulvec[l]);
    m0.clear(0);

    m0(1, 1) = r0.c();
    m0(1, 2) = r1.c();
    m0(2, 1) = r2.c();

    m0(0, 2) = r0.pt(2);
    m0(2, 0) = r2.pt(2);
    m0(2, 2) = r1.pt(2);

    m0._r0 = r0;
    m0._r1 = r1;
    m0._r2 = r2;

    subdiv(l, m0);
}

//-------------------------------------------------------------------

void setRing(Tri::FaceRingType& tr, PointRing& pt) {
    pt.clear();
    pt.setCenter(tr.centerVert()->getPos(0));
    for (int i = 0; i < tr.noVtx(); ++i) {
        pt.addPoint(tr.vert(i)->getPos(0));
    }
}

void writeTri(MeshType* m, int l) {
    MeshType::FaceIterType it;
    for (it = m->faceBegin(0); it != m->faceEnd(0); ++it) {
        PointRing p0, p1, p2;
        Tri::FaceRingType tr;
        tr.collectRing((*it), 1);
        setRing(tr, p0);
        tr.collectRing((*it), 2);
        setRing(tr, p1);
        tr.collectRing((*it), 3);
        setRing(tr, p2);
        subdivRings(l, p0, p1, p2);
    }
}

class MyObject : public GeoObject {
  public:
    MyObject(MeshType* m, int d = 0) : _m(m), _d(d), _listId(0) {}
    int depth() { return _d; }
    void setDepth(int d) { _d = d; }

    virtual cvec3f maxPoint() const {
        cvec3f ma = (*(_m)->faceBegin())->pos(0, 0);
        MeshType::FaceIterType fi;
        for (fi = (_m)->faceBegin(0); fi != (_m)->faceEnd(0); ++fi) {
            for (int i = 0; i < 3; ++i) {
                ma = ma.max((*fi)->pos(i, 0));
            }
        }
        return ma;
    }

    virtual cvec3f minPoint() const {
        cvec3f mi = (*(_m)->faceBegin())->pos(0, 0);
        MeshType::FaceIterType fi;
        for (fi = (_m)->faceBegin(0); fi != (_m)->faceEnd(0); ++fi) {
            for (int i = 0; i < 3; ++i) {
                mi = mi.min((*fi)->pos(i, 0));
            }
        }
        return mi;
    }

    virtual void render() {
        if (glIsList(_listId)) {
            glCallList(_listId);
        } else {
            _listId = glGenLists(1);
            glNewList(_listId, GL_COMPILE_AND_EXECUTE);
            glBegin(GL_TRIANGLES);
            writeTri(_m, _d);
            glEnd();
            glEndList();
            PrintDeltaTimer(std::cerr);
            std::cerr << std::endl;
        }
    }

  private:
    int _listId;
    MeshType* _m;
    int _d;
};

int main(int argc, char** argv) {
    Viewer::initGL(&argc, argv);
    Viewer* bv = new BallViewer();

    TagIvGraph tagIvGraph;
    tagIvGraph.read(argv[1]);

    TagFlatMesh tagFlatMesh;
    tagIvGraph.toTagFlatMesh(&tagFlatMesh, true);

    MeshType triMesh(tagFlatMesh);

    int depth = atoi(argv[2]);

    createULVec(depth);
    MyObject o(&triMesh, depth);
    bv->setObject(&o);

    /*
    {
      int i;
      int d = atoi(argv[2]);
      InitTimer();
      // timing for meshless subdivision
      //
      StartTimer();
      for(i = 0; i < 10; ++i)
        writeTri(&triMesh, d);
      EndTimer();
      PrintDeltaTimer(std::cerr);

      // timing for subdivision with mesh
      //
      std::cerr<<std::endl;
      StartTimer();
      triMesh.subdivide(d);
      EndTimer();
      std::cerr<<"time for initial subdivision: "; PrintDeltaTimer(std::cerr); std::cerr<<std::endl;

      // and 10 subdivivision iterations
      StartTimer();
      for(i = 0; i < 10; ++i)
        triMesh.subdivide(d);
      EndTimer();
      std::cerr<<"10 subdivision iterations: "; PrintDeltaTimer(std::cerr); std::cerr<<std::endl;
    }
    exit(0);
    */

    glShadeModel(GL_SMOOTH);

    glutMainLoop();
}
