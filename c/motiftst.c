/*Motif test program*/

/* #include "motif.h" */
#include <X11/Xutil.h>
#include <Xm/MainW.h>
#include <Xm/RowColumn.h>
#include <stdio.h>

Display* global_display; 
Window   global_window;
GC       global_gc;
Region   global_region = (Region) NULL;

/****************************************/

extern void DrawTest(Display* display, Window window, GC gc);
extern Widget CreateMenu (Widget parent, char* name);
extern Widget CreateMenuChoice (Widget menu, char* name,
                         XtCallbackProc callback,
                         XtPointer client_data);
extern Widget CreateDrawingArea (Widget parent, char* name,
                                 int width, int height,
                                 XtCallbackProc callback,
                                 XtPointer client_data);

/*****************************************/
void HandleExpose(XExposeEvent* event)
{
 XRectangle rectangle;
 if (global_region == (Region) NULL) 
  {
   global_region = XCreateRegion();
  }
 UnionExposeWithRegion(event, global_region);
 if (event->count == 0)
  {
   ClipToRegion(global_display, global_gc, global_region);
   XDestroyRegion(global_region);
   global_region = (region) NULL;
   DrawTest(global_display, global_window, global_gc);
   XSetClipMask(global_display, global_gc, (Pixmap) None);
  }
}

/****************************************/

void drawCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
 XmDrawingAreaCallbackStruct*  ptr;
 ptr = (XmDrawingAreaCallbackStruct*) call_data;
 if (ptr->reason == XmCR_EXPOSE)
  {
   HandleExpose((XExposeEvent*) ptr->event);
  }
}

/******************************************/

static void exitCB(Widget widget, XtPointer client_data,
                   XtPointer call_data)
{
 exit(0);
}

/*******************************************/

int main(int argc, char** argv)
{
 XtAppContext app_context;
 Display* display;
 Widget parent;
 Widget mainwindow;
 Widget menubar;
 Widget filemenu;
 Widget exitchoice;
 Widget drawingarea;
 Arg    args[20];
 int    n;
 int    screen;

 n = 0;
 XtSetArg(args[n], XmNmappedWhenManaged, False); n++;
 XtSetArg(args[n], XmNallowResize, True); n++;
 XtSetArg(args[n], XmNwidth, 500); n++;
 XtSetArg(args[n], XmNheight, 360); n++;
 parent = XtAppInitialize(&app_context, "ProX", (XrmOptionDescList) NULL,
                          0,  ARGC_PTR &argc, argv, (String*) NULL, args, n);
 n = 0;
 XtSetArg(args[n], XmNmappedWhenManaged, True); n++;
 mainwindow = XmCreateMainWindow(parent, "main", args, n);
 n = 0;
 menubar = XmCreateMenuBar(mainwindow, "menubar", args, n);
 XtManageChild(menubar);
 filemenu = CreateMenu(menubar, "filemenu");
 exitchoice = CreateMenuChoice(filemenu, "exitchoice", (XtCallbackProc) exitCB,
                              (XtPointer) NULL);
 drawingarea = CreateDrawingArea(mainwindow, "drawingarea", 640, 480,
                                (XtCallbackProc) drawCB, (XtPointer) NULL);
 XmMainWindowSetAreas(mainwindow, menubar, (Widget) NULL,
                                      (Widget) NULL, (Widget) NULL, drawingarea);
 XtManageChild(mainwindow);
 XtRealizeWidget(parent);
 global_display = XtDisplay(drawingarea);
 global_window = XtWindow(drawingarea);
 screen = DefaultScreen(global_display);
 global_gc = XCreateGC(global_display, global_window, OL, (XGCValues*) NULL);
 XSetForeground(global_display, global_gc,
               BlackPixel(global_display, screen));
 XtMapWidget(parent);
 XtAppMainLoop(app_context);
 return 0;
}
