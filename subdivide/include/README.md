This is the include directory defining the interface to the subdivision
surface library and rendering/picking support library.

The main interface files, that should be included by applications are 

For libsub:

quadmesh.h trimesh.h          Subdivision surfaces.

flatmesh.h tagflatmesh.h      Intermediate mesh representation for I/O.
ivgraph.h                     VRML I/O.

For libviewer:

pickablequad.h pickabletri.h  Interaction interface added.
quadmanipulator.h             Tagging operations on faces of a quad mesh.
trimanipulator.h              Tagging operations on faces of a triangular mesh.

pickviewer.h                  Viewer with support for picking.

----------------------------------------------------------------------
Complete list of files: 

*** General ***

compat.h        Various conditional defines required for portability.
general.h       Some general definitions.

*** Math ** 

cvec3t.h        3d vectors/points. 
hmatrix.h       Homogeneous 4x4 matrices.
quat.h          Quaternions.

*** Tag data ****

sectorinfo.h            SectorInfo: tag information for sectors.

*** Intermediate Mesh Representation ***

ipoly.h         Polygon class, polygons represented as arrays of vertex 
                indices.

flatmesh.h      The mesh class, array for vertices, + array of IPoly's.
        
tagflatmesh.h   Same but with arrays of tags added.


ivgraph.h       VRML 1.0 subset reading/writing.
tagivgraph.h

*** Subdivision surfaces ***

quadmesh.h      Quadrilateral (Catmull-Clark with special boundary rules).
trimesh.h       Triangular (Loop with special boundary rules).
pickablequad.h  Picking support for quad-based surfaces.
pickabletri.h   Picking support for triangle-based surfaces.


*** Rendering and Picking support ***

geoobject.h             Base class for renderable objects.
pickobject.h            Base class for pickable objects.
pickedstuff.h                   
quadmanipulator.h       Tagging-related operations on faces of a quad mesh.
trimanipulator.h        Tagging-related operations on faces of a triangular 
                        mesh.
*** UI ***

viewer.h        Simple viewer.
ballviewer.h    A viewer with a camera associated with the ball.
pickviewer.h    A viewer with support for picking.

