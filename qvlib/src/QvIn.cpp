#include <QvBasic.h>
#include <QvChildList.h>
#include <QvDB.h>
#include <QvIn.h>
#include <QvInput.h>
#include <QvNode.h>
#include <QvSeparator.h>
#include <iostream>

QvIn::QvIn(char* filename) : _error(false) {

    QvDB::init();

    FILE* fp = fopen(filename, "r");
    qvInput.setFilePointer(fp);

    if (fp == 0) {
        std::cerr << "could not open file" << std::endl;
        _error = true;
    }
}

QvSeparator* QvIn::read() {
    QvSeparator* root = 0;
    root = 0;
    QvNode* node;
    QvBool res = QvDB::read(&qvInput, node);

    if (!res || (node == 0) || (_error)) {
        std::cerr << "Problem reading file!" << std::endl;
        _error = true;
        return 0;
    }

    if (dynamic_cast<QvSeparator*>(node)) {
        root = (QvSeparator*)node;
    } else {
        root = new QvSeparator;
        root->getChildren()->append(node);
    }

    root->ref();

    while (node) {
        res = QvDB::read(&qvInput, node);
        if (res && (node != NULL)) {
            root->getChildren()->append(node);
        }
    }

    fclose(qvInput.getCurFile());

    return root;
}
