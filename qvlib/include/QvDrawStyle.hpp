#ifndef _QV_DRAWSTYLE_
#define _QV_DRAWSTYLE_

#include <QvSFEnum.hpp>
#include <QvSFFloat.hpp>
#include <QvSFUShort.hpp>
#include <QvSubNode.hpp>

class QvDrawStyle : public QvNode {

    QV_NODE_HEADER(QvDrawStyle);

  public:
    enum Style { FILLED, LINES, POINTS, INVISIBLE };

    // Fields
    QvSFEnum style;
    QvSFFloat pointSize;
    QvSFFloat lineWidth;
    QvSFUShort linePattern;
};

#endif /* _QV_DRAWSTYLE_ */
