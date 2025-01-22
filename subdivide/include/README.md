# include

This is the include directory defining the interface to the subdivision
surface library and rendering/picking support library.

The main interface files, that should be included by applications, are as follows.

## For `libsub`

```
quadmesh.hpp                    Subdivision surfaces.
trimesh.hpp

flatmesh.hpp                    Intermediate mesh representation for I/O.
tagflatmesh.hpp

ivgraph.hpp                     VRML I/O.
```

## For `libviewer`

```
pickablequad.hpp                Interaction interface added.
pickabletri.hpp

quadmanipulator.hpp             Tagging operations on faces of a quad mesh.
trimanipulator.hpp              Tagging operations on faces of a triangular mesh.

pickviewer.hpp                  Viewer with support for picking.
```

---

## Complete List of Files

### General

```
compat.hpp        Various conditional defines required for portability.
general.hpp       Some general definitions.
```

### Math

```
cvec3t.hpp        3d vectors/points.
hmatrix.hpp       Homogeneous 4x4 matrices.
quat.hpp          Quaternions.
```

### Tag Data

```
sectorinfo.hpp    SectorInfo: tag information for sectors.
```

### Intermediate Mesh Representation

```
ipoly.hpp         Polygon class, polygons represented as arrays of vertex indices.

flatmesh.hpp      The mesh class, array for vertices, + array of IPoly's.

tagflatmesh.hpp   Same but with arrays of tags added.

ivgraph.hpp       VRML 1.0 subset reading/writing.
tagivgraph.hpp
```

### Subdivision Surfaces

```
quadmesh.hpp      Quadrilateral (Catmull-Clark with special boundary rules).
trimesh.hpp       Triangular (Loop with special boundary rules).
pickablequad.hpp  Picking support for quad-based surfaces.
pickabletri.hpp   Picking support for triangle-based surfaces.
```

### Rendering and Picking Support

```
geoobject.hpp             Base class for renderable objects.

pickobject.hpp            Base class for pickable objects.
pickedstuff.hpp

quadmanipulator.hpp       Tagging-related operations on faces of a quad mesh.
trimanipulator.hpp        Tagging-related operations on faces of a triangular mesh.
```

### User Interface

```
viewer.hpp        Simple viewer.
ballviewer.hpp    A viewer with a camera associated with the ball.
pickviewer.hpp    A viewer with support for picking.
```
