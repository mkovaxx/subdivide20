#include <QvFields.hpp>
#include <QvInput.hpp>
#include <QvReadError.hpp>

// Special characters in files
#define OPEN_BRACE_CHAR '['
#define CLOSE_BRACE_CHAR ']'
#define VALUE_SEPARATOR_CHAR ','

QvField::QvField() {
    //  std::cerr<<"QvField::QvField("<<this<<")"<<std::endl;
    flags.hasDefault = TRUE;
}

QvField::~QvField() {}

void QvField::setContainer(QvNode* cont) {
    container = cont;
    setDefault(TRUE);
}

QvBool QvField::read(QvInput* in, const QvName& name) {
    //  std::cerr<<"QvField::read("<<this<<","<<in<<","<<name.getString()<<")"<<std::endl;
    setDefault(FALSE);

    if (!readValue(in)) {
        QvReadError::post(in, "Couldn't read value for field \"%s\"", name.getString());
        return FALSE;
    }

    return TRUE;
}

QvField* QvField::createInstanceFromName(const QvName& className) {
    //  std::cerr<<"QvField::createInstanceFormName("<<className.getString()<<")"<<std::endl;
#define TRY_CLASS(name, class) else if (className == name) inst = new class

    QvField* inst = NULL;

    if (0)
        ; // So "else" works in first TRY_CLASS

    TRY_CLASS("MFColor", QvMFColor);
    TRY_CLASS("MFFloat", QvMFFloat);
    TRY_CLASS("MFInt32", QvMFInt32);
    TRY_CLASS("MFLong", QvMFLong);
    TRY_CLASS("MFString", QvMFString);
    TRY_CLASS("MFVec2f", QvMFVec2f);
    TRY_CLASS("MFVec3f", QvMFVec3f);
    TRY_CLASS("SFBitMask", QvSFBitMask);
    TRY_CLASS("SFBool", QvSFBool);
    TRY_CLASS("SFColor", QvSFColor);
    TRY_CLASS("SFEnum", QvSFEnum);
    TRY_CLASS("SFFloat", QvSFFloat);
    TRY_CLASS("SFImage", QvSFImage);
    TRY_CLASS("SFLong", QvSFLong);
    TRY_CLASS("SFMatrix", QvSFMatrix);
    TRY_CLASS("SFRotation", QvSFRotation);
    TRY_CLASS("SFString", QvSFString);
    TRY_CLASS("SFVec2f", QvSFVec2f);
    TRY_CLASS("SFVec3f", QvSFVec3f);

    return inst;

#undef TRY_CLASS
}

QvSField::QvSField() {}

QvSField::~QvSField() {}

QvMField::QvMField() { maxNum = num = 0; }

QvMField::~QvMField() {}

void QvMField::makeRoom(int newNum) {
    if (newNum != num) {
        allocValues(newNum);
    }
}

QvBool QvMField::readValue(QvInput* in) {
    char c;
    int curIndex = 0;

    if (in->read(c) && c == OPEN_BRACE_CHAR) {

        if (in->read(c) && c == CLOSE_BRACE_CHAR)
            ;
        else {
            in->putBack(c);

            while (TRUE) {

                if (curIndex >= num) {
                    makeRoom(curIndex + 1);
                }

                if (!read1Value(in, curIndex++) || !in->read(c)) {
                    QvReadError::post(in, "Couldn't read value %d of field", curIndex);
                    return FALSE;
                }

                if (c == VALUE_SEPARATOR_CHAR) {
                    if (in->read(c)) {
                        if (c == CLOSE_BRACE_CHAR) {
                            break;
                        } else {
                            in->putBack(c);
                        }
                    }
                }

                else if (c == CLOSE_BRACE_CHAR) {
                    break;
                }

                else {
                    QvReadError::post(in,
                                      "Expected '%c' or '%c' but got "
                                      "'%c' while reading value %d",
                                      VALUE_SEPARATOR_CHAR, CLOSE_BRACE_CHAR, c, curIndex);
                    return FALSE;
                }
            }
        }

        if (curIndex < num) {
            makeRoom(curIndex);
        }
    }

    else {
        in->putBack(c);
        makeRoom(1);
        if (!read1Value(in, 0)) {
            return FALSE;
        }
    }

    return TRUE;
}
