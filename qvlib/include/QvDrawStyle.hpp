#ifndef _QV_DRAWSTYLE_
#define _QV_DRAWSTYLE_

#include <QvSFEnum.h>
#include <QvSFFloat.h>
#include <QvSFUShort.h>
#include <QvSubNode.h>

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
