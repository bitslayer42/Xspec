/********************************/
/* convenience functions for motif drawing program */


#include "motif.h"
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/DrawingA.h>

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

/******************************************/

Widget CreateMenuChoice(Widget parent, char* name,
                        XtCallbackProc callback,
                        XtPointer client_data)
{
 return CreatePushbutton(parent, name, callback, client_data);
}

/*****************************************/

Widget CreatePushbutton(Widget parent, char* name,
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
               