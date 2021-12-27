/************************/
/* XspecConv.h               */
/*************************/
#include <Xm/Xm.h>
#define point3d vector

typedef struct
{
double x, y, z;
}vector;

typedef struct
{
double theta, phi;
}angles;

typedef struct
{
int x, y;
}point2d;

typedef struct
{
int from, to;
}
line;

extern Widget CreateMenu (Widget parent, char* name);

extern Widget CreateMenuChoice(Widget parent, char* name,
                               XtCallbackProc callback,
                               XtPointer client_data);
extern Widget CreatePushButton(Widget parent, char* name,
                               XtCallbackProc callback,
                               XtPointer client_data);
extern Widget CreateToggleButton(Widget parent, char* name,
                               XtCallbackProc callback,
                               int client_data);

extern Widget CreateDrawingArea(Widget parent, char* name,
                                int width, int height,
                                XtCallbackProc callback,
                                XtPointer client_data);

extern void UnionExposeWithRegion(XExposeEvent* event, Region region);

extern void ClipToRegion(Display* display, GC gc, Region region);

extern Widget CreateShell(Widget parent, char* app_name, char* app_class, char* title);

extern Widget CreateHSlider(Widget parent, char* name,
                            int minimum, int maximum,
                            int init,
                            char* title, XtCallbackProc callback,
                            XtPointer client_data);








