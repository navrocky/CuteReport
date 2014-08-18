#ifndef WIDGETS_GLOBALS_H
#define WIDGETS_GLOBALS_H

#ifdef NO_CUTEREPORT_WIDGET_EXPORTS
#define WIDGET_EXPORTS
#else
#ifdef WIN32
#ifdef CUTEREPORT_WIDGET_EXPORTS
#define WIDGET_EXPORTS  __declspec(dllexport)
#else
#define WIDGET_EXPORTS  __declspec(dllimport)
#endif
#else
#define WIDGET_EXPORTS
#endif
#endif

#endif // WIDGETS_GLOBALS_H
