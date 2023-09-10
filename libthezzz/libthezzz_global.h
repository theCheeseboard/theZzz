#ifndef LIBTHEZZZ_GLOBAL_H
#define LIBTHEZZZ_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBTHEZZZ_LIBRARY)
    #define LIBTHEZZZ_EXPORT Q_DECL_EXPORT
#else
    #define LIBTHEZZZ_EXPORT Q_DECL_IMPORT
#endif

#define LIBTHEZZZ_TRANSLATOR "thezzz/libthezzz"

#endif // LIBTHEZZZ_GLOBAL_H
