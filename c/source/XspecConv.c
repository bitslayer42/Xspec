/********************************/
/* XspecConv.c */
/* convenience functions for Xspec, a motif curved */
/* perspective drawing program */
/* Jon Bierce-Wilson */


#include <Xm/Xm.h>
#include "XspecConv.h"
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/DrawingA.h>
#include <X11/Shell.h>
#include <Xm/Scale.h>
extern XmStringCharSet char_set;

/********************************/

Widget CreateMenu(Widget parent, char* name)
{
 Widget cascade;
 Widget menuwidget;
 Arg args[20];
 int n;
 
 n = 0;
 menuwidget = XmCreatePulldownMenu(parent, name, args, n);
 n = 0;
 XtSetArg(args[n], XmNsubMenuId, menuwidget); n++;
 cascade = XmCreateCascadeButton(parent, name, args, n);
 XtManageChild(cascade);
 return menuwidget;
}

/*****************************************/
Widget CreateToggleButton(Widget parent, char* name,
                        XtCallbackProc callback,
                        int client_data)
                        
{
 Widget button;
 Arg args[20];
 int n;

 n = 0;
 button = XmCreateToggleButton(parent, name, args, n);
 XtManageChild(button);
 XtAddCallback(button, XmNvalueChangedCallback, callback, client_data);
 return button;
}

/*****************************************/
Widget CreatePushButton(Widget parent, char* name,
                        XtCallbackProc callback,
                        XtPointer client_data)
                        
{
 Widget button;
 Arg args[20];
 int n;

 n = 0;
 button = XmCreatePushButton(parent, name, args, n);
 XtManageChild(button);
 XtAddCallback(button, XmNactivateCallback, callback, client_data);
 return button;
}
/******************************************/

Widget CreateMenuChoice(Widget parent, char* name,
                        XtCallbackProc callback,
                        XtPointer client_data)
              
{
 return CreatePushButton(parent, name, callback, client_data);
}
/**********************************************/

Widget CreateDrawingArea(Widget parent, char* name, int width, int height,
                         XtCallbackProc callback, XtPointer client_data)
{
 Widget widget;
 Arg args[20];
 int n;

 n = 0;
 XtSetArg(args[n], XmNwidth, width); n++;
 XtSetArg(args[n], XmNheight, height); n++;
 XtSetArg(args[n], XmNallowResize, True); n++; 
 widget = XmCreateDrawingArea(parent, name, args, n);
 XtManageChild(widget);
 XtAddCallback(widget, XmNresizeCallback, callback, client_data);
 XtAddCallback(widget, XmNinputCallback, callback, client_data);
 XtAddCallback(widget, XmNexposeCallback, callback, client_data);
 return widget;
}

/*****************************************************/

void UnionExposeWithRegion(XExposeEvent* event, Region region)
{
 XRectangle rectangle;
 rectangle.x = event->x;
 rectangle.y = event->y;
 rectangle.width = event->width;
 rectangle.height = event->height;
 XUnionRectWithRegion(&rectangle, region, region);
}

/*********************************************************/

void ClipToRegion(Display* display, GC gc, Region region)
{
 XRectangle rectangle;
#define CLIP_REGIONS
#ifdef CLIP_REGIONS
 XSetRegion(display, gc, region);
#else
 XClipBox(region, &rectangle);
 XSetClipRectangles(display, gc, 0, 0, &rectangle, 1, YXBanded);
#endif
}

/********************************************************/

Widget CreateShell(Widget parent, char* app_name, char* app_class, char* title)
{
 Widget widget;
 Arg args[20];
 int n;

 n = 0;
 XtSetArg(args[n], XmNwinGravity, NorthWestGravity); n++;
 XtSetArg(args[n], XmNtitle, title); n++;
 XtSetArg(args[n], XmNminWidth, 200 ); n++;
 XtSetArg(args[n], XmNtransientFor, parent); n++;
 XtSetArg(args[n], XmNallowShellResize, True); n++;
 widget = XtAppCreateShell(app_name, app_class, topLevelShellWidgetClass,
                           XtDisplay(parent), args, n);
 return widget;
}

/*********************************************************/

Widget CreateHSlider(Widget parent, char* name, int minimum, int maximum,
                     int init, char* title, XtCallbackProc callback,
                     XtPointer client_data)

{
 Widget widget;
 Arg args[20];
 int n;
 XmString xmstring;
 
 n = 0;
 XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
 XtSetArg(args[n], XmNprocessingDirection, XmMAX_ON_RIGHT); n++;
 XtSetArg(args[n], XmNmaximum, maximum); n++;
 XtSetArg(args[n], XmNminimum, minimum); n++;
 XtSetArg(args[n], XmNshowValue, True); n++;
 XtSetArg(args[n], XmNscaleMultiple, 1); n++;
 xmstring = XmStringCreateSimple(title);
 XtSetArg(args[n], XmNtitleString, xmstring); n++; 
 widget = XmCreateScale(parent, name, args, n);
 XmScaleSetValue(widget, init);
 XmStringFree(xmstring);
 XtAddCallback(widget, XmNvalueChangedCallback, callback, client_data);
 XtManageChild(widget);
 return widget;
}

/*************************************************************/









