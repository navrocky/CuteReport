#ifndef DESIGNER_GLOBALS_H
#define DESIGNER_GLOBALS_H

#ifdef WIN32
#ifdef CUTEREPORT_DESIGNER_EXPORTS
#define DESIGNER_EXPORTS  __declspec(dllexport)
#else
#define DESIGNER_EXPORTS  __declspec(dllimport)
#endif
#else
#define DESIGNER_EXPORTS
#endif


#endif // DESIGNER_GLOBALS_H
