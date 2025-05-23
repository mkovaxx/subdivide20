#ifndef _QV_MATERIAL_
#define _QV_MATERIAL_

#include <QvMFColor.hpp>
#include <QvMFFloat.hpp>
#include <QvSubNode.hpp>

class QvMaterial : public QvNode {

    QV_NODE_HEADER(QvMaterial);

  public:
    // Fields
    QvMFColor ambientColor;  // Ambient color
    QvMFColor diffuseColor;  // Diffuse color
    QvMFColor specularColor; // Specular color
    QvMFColor emissiveColor; // Emissive color
    QvMFFloat shininess;     // Shininess
    QvMFFloat transparency;  // Transparency
};

#endif /* _QV_MATERIAL_ */
