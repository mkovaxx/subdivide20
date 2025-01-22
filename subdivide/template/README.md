# template

This directory contains header and template implementation files
for mesh connectivity, tag and geometry management
for meshes, as well as Loop and Catmull-Clark subdivision rule
implementation extended to handle piecewise smooth boundaries
as described in the paper.

```
H. Biermann, A. Levin, D. Zorin.
Piecewise smooth subdivision surfaces with normal control.
SIGGRAPH 2000 Conference Proceedings.
```

Large template instantiations are separated into `.impl.h` files with the
same names as `.h` files containing the template definition.
Template classes have names ending in `Tp`.

The primary data structure is a face, which represents a node in
a quad or triangular hierarchical mesh obtained by subdivision.
Different functionality (mesh connectivity, hierarchy, tags and
geometry) is separated into a number of templates.

Templates here are used to create interfaces
that can be shared by triangular and quadrilateral hierarchical meshes,
and to be able to create member functions in base classes that are able
to return pointers to the derived classes. (This is achieved by
parameterizing the base class templates by the derived class, see
for example `subquad.hpp`.)

In the dependency lists we do not enumerate dependencies
on global headers, such as `general.hpp`, `compat.hpp`, and `cvec3t.hpp`
containing information shared throughout the application.

The files in this directory depend on external files
from `../include`: `general.hpp`, `compat.hpp`, `cvec3t.hpp`, `sectorinfo.hpp`,
`flatmesh.hpp`, and `tagflatmesh.hpp`.

The following files have corresponding `.cpp` files in `../src` directory:
`subquad.hpp`, `subtri.hpp`, `quadrule.hpp`, and `trirule.hpp`.

The file contents are as follows.

## Vertices

```
basevertex.hpp          BaseVertex: vertex class.
vertex.hpp              Vertex: class with reference counting added to
                        BaseVertex.
                        Depends on basevertex.hpp
```

## Faces: Base

```
baseface.hpp            BaseFaceTp: base template for all faces.

convface.hpp            ConvenientFaceTp: contains
                        accessors common for triangles and quads.

tlbaseface.hpp          TLBaseFaceTp: template class to support pointers
                        to neighbors for top-level faces.
```

## Faces: Tags

```
tagface.hpp             TagFaceTp: encapsulates accessors to tags
                        for all faces.
                        Depends on sectorinfo.hpp facering.hpp

tltagface.hpp           TlTagFaceTp: stores tags and prescribed normals
                        at top-level faces.
                        Depends on sectorinfo.hpp
```

## Faces: Quad and Triangle-Specific

```
basequad.hpp            BaseQuadTp: base for all faces of a
                        quad hierarchy (including top-level);
                        TLBaseQuadTp: a specialization for top-level faces.
                        Functionality specific to quad-based hierarchies,
                        such as finding neighbors of faces across edges.
                        Depends on vertex.hpp

basetri.hpp             BaseTriTp: base template class for all faces of a
                        triangle hierarchy (including top-level).
                        TLBaseTriTp: a specialization for top-level faces.
                        Functionality specific to quad-based hierarchies,
                        such as finding neighbors of faces across edges.
                        Depends on vertex.hpp
```

## Faces: Geometry

```
geoface.hpp             GeoFaceTp: encapsulates accessors to vertex positions.
```

## Faces: Assembly and Subdivision

```
subquad.hpp             Quad and TLQuad: classes assembling together
                        and instantiating templates to handle connectivity,
                        geometry and tags, as well as quad hierarchy specific
                        functions. with member functions to perform
                        subdivision.
                        Depends on
                        baseface.hpp basequad.hpp convface.hpp geoface.hpp tagface.hpp
                        tlbaseface.hpp tltagface.hpp quadruletable.hpp subdivide.hpp

subtri.hpp              Tri and TLTri: classes assembling together
                        and instantiating templates to handle connectivity,
                        geometry and tags, as well as triangle hierarchy
                        specific functions. with member functions to perform
                        subdivision.
                        Depends on
                        baseface.hpp basetri.hpp convface.hpp geoface.hpp tagface.hpp
                        tlbaseface.hpp tltagface.hpp triruletable.hpp subdivide.hpp
```

## Auxiliary Structures

```
facering.hpp            FaceRingTp: represents rings of faces adjacent to
                        a vertex.

subdivide.hpp           SubdivideTp: Ring extended with
                        a collection of functions to apply subdivision rules.
                        Depends on sectorinfo.hpp
```

## Subdivision Rules

```
quadrule.hpp            QuadRule and derived classes:
                        quadrilateral subdivision coefficients
                        (extends Catmull Clark).

quadruletable.hpp       QuadRuleTableTp:
                        All rule tables for the quad scheme and
                        functions to apply the rules to rings.
                        Depends on quadrule.hpp

trirule.hpp             TriRule: Triangular subdivision coefficients
                        (extends Loop scheme).

triruletable.hpp        TriRuleTableTp:
                        All rule tables for the triangular scheme and
                        functions to apply the rules to rings.
                        Depends on trirule.hpp
```

## Meshes

```
mesh.hpp                MeshTp: base mesh class: set of faces +
                        set of vertices.
                        Depends on flatmesh.hpp

tagmesh.hpp             TagMeshTp: extends mesh to handle tags.
                        Depends on mesh.hpp sectorinfo.hpp tagflatmesh.hpp

facemanipulator.hpp     FaceManipulatorTp: support for modifying tags
                        on vertices and edges of a face.
                        Depends on sectorinfo.hpp
```

## Miscellaneous

```
optdeque.hpp            An implementation of deque used to store
                        rings of faces.
```
