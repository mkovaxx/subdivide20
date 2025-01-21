# viewer

Files in this directory implement rendering and simple interaction
for subdivision surfaces.

The interface is provided by two main classes:
`QuadMeshObject` and `TriMeshObject`.

## Math Classes

```
quat.h          Quaternions (for the arcball).
hmatrix.h       4x4 matrices representing homogeneous transforms
                (for the arcball).
```

## General GUI Functionality

```
arcball.h               Arcball: based on Ken Shoemake's code.
camera.h                Camera: a perspective camera model
uiaction.h              Camera-ArcBall interaction.
glcheck.h               A macro to check for OpenGL errors.
pickableparam.h         Colors for rendering and picking.
```

## Viewers

```
viewer.h                Viewer: basic viewer class.
ballviewer.h            BallViewer: ArcBall point of view manipulation added.
pickviewer.h            PickViewer: interaction with mesh added.
```

## Renderable and Pickable Meshes

```
geoobject.cpp           GeoObject: Base class for renderable objects.
pickobject.cpp          PickObject: Pickable object base class.
pickedstuff.cpp         Default result of a picking operation.

meshobject.h            MeshObjectTp rendarable mesh template.

pickmeshobject.h        PickMeshObjectTp: pickable mesh template.

selectedobject.h        SelectedObjectTp: stores and manipulates
                        current selection information; enforces
                        consistency of tag modification.

quadmeshobject.cpp      QuadMeshObject: wrapper for quad subdivision
                        surface implementation.

trimeshobject.cpp       TriMeshObject:  wrapper for triangle subdivision
                        surface implementation.
```
