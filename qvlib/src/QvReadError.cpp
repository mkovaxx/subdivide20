#include <QvInput.hpp>
#include <QvReadError.hpp>
#include <QvString.hpp>
#include <cstdarg>
#include <cstdio>

void QvReadError::post(const QvInput* in, const char* formatString...) {
    char buf[10000];
    va_list ap;

    va_start(ap, formatString);
    vsnprintf(buf, sizeof(buf), formatString, ap);
    va_end(ap);
    buf[sizeof(buf) - 1] = '\0';  // Ensure null-termination

    QvString locstr;
    in->getLocationString(locstr);
    fprintf(stderr, "VRML read error: %s\n%s\n", buf, locstr.getString());
}
