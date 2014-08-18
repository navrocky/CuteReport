#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef WIN32
#ifdef PROPERTYEDITOR_LIB_EXPORTS
#define PROPERTYEDITOR_EXPORTS __declspec(dllexport)
#else
#define PROPERTYEDITOR_EXPORTS __declspec(dllimport)
#endif
#else
#define PROPERTYEDITOR_EXPORTS
#endif

#endif // GLOBALS_H
