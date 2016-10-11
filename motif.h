/*************************/
/* motif.h               */
/*************************/
#ifndef motif_h_
#define motif_h_      1
#endif

#include <Xm/Xm.h>
#if XtSpecificationRelease > 4
#define ARGC_PTR  (int*)
#else
#define ARGC_PTR  (unsigned int*)
#endif

/**************************************/

extern Widget CreateMenu (Widget parent, char* name);

extern Widget CreateMenuChoice(Widget parent, char* name,
                               XtCallbackProc callback,
                               XtPointer client_data);

extern Widget CreateDrawingArea(Widget parent, char* name,
                                int width, int height,
                                XtCallbackProc callback,
                                XtPointer client_data);