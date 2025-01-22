#include <QvDrawStyle.hpp>

QV_NODE_SOURCE(QvDrawStyle);

QvDrawStyle::QvDrawStyle() {

    QV_NODE_CONSTRUCTOR(QvDrawStyle);
    isBuiltIn = TRUE;

    QV_NODE_ADD_FIELD(style);

    style.value = FILLED;

    QV_NODE_DEFINE_ENUM_VALUE(Style, FILLED);
    QV_NODE_DEFINE_ENUM_VALUE(Style, LINES);
    QV_NODE_DEFINE_ENUM_VALUE(Style, POINTS);
    QV_NODE_DEFINE_ENUM_VALUE(Style, INVISIBLE);

    QV_NODE_SET_SF_ENUM_TYPE(style, Style);

    QV_NODE_ADD_FIELD(pointSize);
    QV_NODE_ADD_FIELD(lineWidth);
    QV_NODE_ADD_FIELD(linePattern);

    pointSize.value = 1.0;
    lineWidth.value = 1.0;
    linePattern.value = 0xffff;
}

QvDrawStyle::~QvDrawStyle() {}
