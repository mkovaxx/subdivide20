# src

Source directory for the subdivision library `libsub`.

Most of the work is actually done by template classes in `../template`.

The library provides 4 main classes: `QuadMesh`, `TriMesh`,
`QuadManipulator` and `TriManipulator`, which are wrappers around
constructions based on templates.

The include files defining the interface are in `../include`.

```
flatmesh.cpp            Intermediate mesh representation used for input.
                        An array of vertices and and array of
                        arrays of indices of vertices forming faces.

tagflatmesh.cpp         Same but with tags added.
ivgraph.cpp             Conversion from VRML 1.0 to FlatMesh and back.
tagivgraph.cpp          Same but with tags.

subquad.cpp             Subdivision for quads.
quadrule.cpp            Rule coefficient computations, Catmull-Clark
quadmesh.cpp            Quad subdivision surface wrapper.

subtri.cpp              Subdivision for triangles.
trirule.cpp             Rule coefficient computations, Loop.
trimesh.cpp             Triangle subdivision surface wrapper.

quadmanipulator.cpp     Wrapper around the class Quad to provide
                        access to tags and normals for interactive
                        manipulation.

trimanipulator.cpp      Wrapper around the class Tri to provide
                        access to tags and normals for interactive
                        manipulation.
```
