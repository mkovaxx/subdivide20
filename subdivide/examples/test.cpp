#include "compat.h"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include "ballviewer.h"
#include "general.h"
#include "geoobject.h"
#include "subtri.h"
#include "tagflatmesh.h"
#include "tagivgraph.h"
#include "tagmesh.h"

#include "timer.h"

typedef TagMeshTp<Tri> MeshType;

#define MAX_VALENCE 20
#define MAX_STACK 100
#define SIN120 0.86602540378444f

class PointRing {
  public:
    PointRing() : _start(0), _noPts(0){};
    PointRing(const cvec3f& c, const cvec3f& p0, const cvec3f& p1, const cvec3f& p2, const cvec3f& p3, const cvec3f& p4,
              const cvec3f& p5)
        : _noPts(6) {
        _start = 0;
        _center = c;
        _ring[0] = p0;
        _ring[1] = p1;
        _ring[2] = p2;
        _ring[3] = p3;
        _ring[4] = p4;
        _ring[5] = p5;
    }

    int noPts() const { return _noPts; }

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
        for (int i = 0; i < noPts(); ++i)
            std::cerr << "\t" << _ring[i] << std::endl;
        std::cerr << std::endl;
    }

  private:
    int _noPts;
    int _start;

    static const float s6[6];
    static const float c6[6];

    cvec3f _center;
    cvec3f _ring[MAX_VALENCE];

    cvec3f& p(int i) { return _ring[(i + noPts()) % noPts()]; }

    const cvec3f& p(int i) const { return _ring[(i + noPts()) % noPts()]; }
};

const float PointRing::s6[6] = {0, SIN120, SIN120, 0.0000, -SIN120, -SIN120};
const float PointRing::c6[6] = {1.0000, 0.5000, -0.5000, -1.0000, -0.5000, 0.5000};

class PointRingStack {
  public:
    static PointRing& push() {
        assert(_cnt < MAX_STACK);
        ++_cnt;
        return _pr[_cnt - 1];
    }
    static void pop() {
        --_cnt;
        assert(_cnt >= 0);
    }

  private:
    static int _max_cnt;
    static int _cnt;
    static PointRing _pr[MAX_STACK];
};

int PointRingStack::_max_cnt = 0;
int PointRingStack::_cnt = 0;
PointRing PointRingStack::_pr[MAX_STACK];

void setRing(Tri::FaceRingType& tr, PointRing& pt) {
    pt.setCenter(tr.centerVert()->getPos(0));
    for (int i = 0; i < tr.noVtx(); ++i)
        pt.addPoint(tr.vert(i)->getPos(0));
}

void writeRec(int l, const PointRing& r0, const PointRing& r1, const PointRing& r2) {

    assert((r0.pt(0) - r2.c()).l2() < 0.001);
    assert((r0.pt(1) - r1.c()).l2() < 0.001);

    assert((r1.pt(0) - r0.c()).l2() < 0.001);
    assert((r1.pt(1) - r2.c()).l2() < 0.001);

    assert((r2.pt(0) - r1.c()).l2() < 0.001);
    assert((r2.pt(1) - r0.c()).l2() < 0.001);

    if (l == 0) {
        cvec3f p0(r0.c());
        cvec3f p1(r1.c());
        cvec3f p2(r2.c());

        cvec3f n0(r0.n());
        cvec3f n1(r1.n());
        cvec3f n2(r2.n());

        /*
        glNormal3fv(n0);
        glVertex3fv(p0);

        glNormal3fv(n1);
        glVertex3fv(p1);

        glNormal3fv(n2);
        glVertex3fv(p2);
        */

    } else {

        PointRing &nr0(PointRingStack::push()), &nr1(PointRingStack::push()), &nr2(PointRingStack::push());

        r0.subdivide(nr0);
        r1.subdivide(nr1);
        r2.subdivide(nr2);

        PointRing r01(nr1.pt(0), nr0.pt(0), nr2.pt(0), nr1.c(), nr1.pt(-1), nr0.pt(2), nr0.c());

        PointRing r12(nr2.pt(0), nr1.pt(0), nr0.pt(0), nr2.c(), nr2.pt(-1), nr1.pt(2), nr1.c());

        PointRing r20(nr0.pt(0), nr2.pt(0), nr1.pt(0), nr0.c(), nr0.pt(-1), nr2.pt(2), nr2.c());

        writeRec(l - 1, r01, r12, r20);

        r01.assignStart(-1);
        r20.assignStart(1);
        writeRec(l - 1, nr0, r01, r20);

        r12.assignStart(-1);
        r01.assignStart(1);
        writeRec(l - 1, nr1, r12, r01);

        r20.assignStart(-1);
        r12.assignStart(1);
        writeRec(l - 1, nr2, r20, r12);

        PointRingStack::pop();
        PointRingStack::pop();
        PointRingStack::pop();
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
        writeRec(l, p0, p1, p2);
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
            for (int i = 0; i < 3; ++i)
                ma = ma.max((*fi)->pos(i, 0));
        }
        return ma;
    }

    virtual cvec3f minPoint() const {
        cvec3f mi = (*(_m)->faceBegin())->pos(0, 0);
        MeshType::FaceIterType fi;
        for (fi = (_m)->faceBegin(0); fi != (_m)->faceEnd(0); ++fi) {
            for (int i = 0; i < 3; ++i)
                mi = mi.min((*fi)->pos(i, 0));
        }
        return mi;
    }

    virtual void render() {
        if (glIsList(_listId))
            glCallList(_listId);
        else {
            _listId = glGenLists(1);
            glNewList(_listId, GL_COMPILE_AND_EXECUTE);
            glBegin(GL_TRIANGLES);
            writeTri(_m, _d);
            glEnd();
            glEndList();
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

    MyObject o(&triMesh, atoi(argv[2]));
    bv->setObject(&o);

    {
        int i;
        int d = atoi(argv[2]);
        InitTimer();
        // timing for meshless subdivision
        //
        StartTimer();
        for (i = 0; i < 10; ++i)
            writeTri(&triMesh, d);
        EndTimer();
        PrintDeltaTimer(std::cerr);

        // timing for subdivision with mesh
        //
        std::cerr << std::endl;
        StartTimer();
        triMesh.subdivide(d);
        EndTimer();
        std::cerr << "time for initial subdivision: ";
        PrintDeltaTimer(std::cerr);
        std::cerr << std::endl;

        // and 10 subdivivision iterations
        StartTimer();
        for (i = 0; i < 10; ++i)
            triMesh.subdivide(d);
        EndTimer();
        std::cerr << "10 subdivision iterations: ";
        PrintDeltaTimer(std::cerr);
        std::cerr << std::endl;
    }

    exit(0);

    glShadeModel(GL_SMOOTH);

    glutMainLoop();
}
