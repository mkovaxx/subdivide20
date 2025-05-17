#include <QvDebugError.hpp>
#include <QvString.hpp>
#include <cstdarg>
#include <cstdio>

void QvDebugError::post(const char* methodName, const char* formatString...) {
    char buf[10000];
    va_list ap;

    va_start(ap, formatString);
    vsnprintf(buf, sizeof(buf), formatString, ap);
    va_end(ap);
    buf[sizeof(buf) - 1] = '\0';  // Ensure null-termination

    fprintf(stderr, "VRML error in %s(): %s\n", methodName, buf);
}
