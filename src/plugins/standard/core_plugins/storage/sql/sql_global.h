#ifndef SQL_GLOBAL_H
#define SQL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SQL_LIBRARY)
#  define SQLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SQLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SQL_GLOBAL_H
