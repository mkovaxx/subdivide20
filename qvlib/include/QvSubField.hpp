#ifndef _QV_SUB_FIELD_
#define _QV_SUB_FIELD_

#include <QvField.hpp>
#include <QvInput.hpp>

#include <assert.h>

/////////////////////////////////////////////////////////////////////////////

#define QV_SFIELD_HEADER(className)                                                                                    \
  public:                                                                                                              \
    className();                                                                                                       \
    virtual ~className();                                                                                              \
    virtual QvBool readValue(QvInput* in)

/////////////////////////////////////////////////////////////////////////////

#define QV_MFIELD_HEADER(className)                                                                                    \
  public:                                                                                                              \
    className();                                                                                                       \
    virtual ~className();                                                                                              \
    virtual QvBool read1Value(QvInput* in, int index);                                                                 \
    void allocValues(int newNum);                                                                                      \
                                                                                                                       \
  protected:                                                                                                           \
    static QvBool canUseMalloc

/////////////////////////////////////////////////////////////////////////////

#define QV_SFIELD_SOURCE(className)                                                                                    \
                                                                                                                       \
    className::className() {}                                                                                          \
    className::~className() {}

/////////////////////////////////////////////////////////////////////////////

// Note: field types whose values require constructors to be called
// should pass TRUE for valueHasConstructor. Otherwise, pass FALSE.

#define QV_MFIELD_SOURCE(className, valueType, numValues, valueHasConstructor)                                         \
                                                                                                                       \
    QvBool className::canUseMalloc;                                                                                    \
                                                                                                                       \
    className::className() {                                                                                           \
        canUseMalloc = !valueHasConstructor;                                                                           \
        values = NULL;                                                                                                 \
        /* Make room for 1 value to start */                                                                           \
        allocValues(1);                                                                                                \
    }                                                                                                                  \
                                                                                                                       \
    className::~className() {                                                                                          \
        if (values != NULL) {                                                                                          \
            if (canUseMalloc)                                                                                          \
                free((char*)values);                                                                                   \
            else                                                                                                       \
                delete[] values;                                                                                       \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    void className::allocValues(int newNum) {                                                                          \
        if (values == NULL) {                                                                                          \
            if (newNum > 0) {                                                                                          \
                if (canUseMalloc)                                                                                      \
                    values = (valueType*)malloc(numValues * sizeof(valueType) * newNum);                               \
                else                                                                                                   \
                    values = new valueType[numValues * newNum];                                                        \
            }                                                                                                          \
        } else {                                                                                                       \
            if (newNum > 0) {                                                                                          \
                if (canUseMalloc) {                                                                                    \
                    values = (valueType*)realloc(values, numValues * sizeof(valueType) * newNum);                      \
                    assert(values);                                                                                    \
                } else {                                                                                               \
                    valueType* oldValues = values;                                                                     \
                    values = new valueType[numValues * newNum];                                                        \
                    for (int i = 0; i < num && i < newNum; i++)                                                        \
                        values[i] = oldValues[i];                                                                      \
                    delete[] oldValues;                                                                                \
                }                                                                                                      \
            } else {                                                                                                   \
                if (canUseMalloc)                                                                                      \
                    free((char*)values);                                                                               \
                else                                                                                                   \
                    delete[] values;                                                                                   \
                values = NULL;                                                                                         \
            }                                                                                                          \
        }                                                                                                              \
        num = maxNum = newNum;                                                                                         \
    }

#endif /* _QV_SUB_FIELD_ */
