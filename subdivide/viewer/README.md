# viewer

Files in this directory implement rendering and simple interaction
for subdivision surfaces.

The interface is provided by two main classes:
`QuadMeshObject` and `TriMeshObject`.

## Math Classes

```
quat.hpp        Quaternions (for the arcball).
hmatrix.hpp     4x4 matrices representing homogeneous transforms
                (for the arcball).
```

## General GUI Functionality

```
arcball.hpp             Arcball: based on Ken Shoemake's code.
camera.hpp              Camera: a perspective camera model
uiaction.hpp            Camera-ArcBall interaction.
glcheck.hpp             A macro to check for OpenGL errors.
pickableparam.hpp       Colors for rendering and picking.
```

## Viewers

```
viewer.hpp              Viewer: basic viewer class.
ballviewer.hpp          BallViewer: ArcBall point of view manipulation added.
pickviewer.hpp          PickViewer: interaction with mesh added.
```

## Renderable and Pickable Meshes

```
geoobject.cpp           GeoObject: Base class for renderable objects.
pickobject.cpp          PickObject: Pickable object base class.
pickedstuff.cpp         Default result of a picking operation.

meshobject.hpp          MeshObjectTp rendarable mesh template.

pickmeshobject.hpp      PickMeshObjectTp: pickable mesh template.

selectedobject.hpp      SelectedObjectTp: stores and manipulates
                        current selection information; enforces
                        consistency of tag modification.

quadmeshobject.cpp      QuadMeshObject: wrapper for quad subdivision
                        surface implementation.

trimeshobject.cpp       TriMeshObject:  wrapper for triangle subdivision
                        surface implementation.
```
