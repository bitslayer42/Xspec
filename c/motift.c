/*Motif Curved Perspective Program*/

#include "motif.h"
#include <X11/Xutil.h>
#include <Xm/MainW.h>
#include <Xm/RowColumn.h>
#include <stdio.h>
#include <math.h>

Display* global_display;
Window   global_window;
GC       global_gc;
Region   global_region = (Region) NULL;
/****************************************/
#define MAXPOINTS 30
#define MAXLINES 44
#define WINDOWSIZE 800
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
}line;

point3d eye = {3, 2, 3};   /*these are the globals that can be changed */
point3d view = {2, 1, 3};  /*from the control window */
double width = 2;

line  lines[MAXLINES];
point3d verts3d[MAXPOINTS];
point2d screenVerts[MAXPOINTS];
angles viewAngles[MAXPOINTS];

int numlines;
int numpoints;

/****************************************/
/*returns the dot product of two vectors  */

double dotP(vector A, vector B)
{
 double result;                      
 result = A.x*B.x + A.y*B.y + A.z*B.z;
 return result;
}

/****************************************/
/*returns the cross product of two vectors  */

vector crossP(vector A, vector B)
{
 vector result;                   
 result.x = A.y*B.z - A.z*B.y;
 result.y = A.z*B.x - A.x*B.z;
 result.z = A.x*B.y - A.y*B.x;
 return result;
}

/*******************************************/
/*returns the angle between two vectors- the inverse cos of */
/*their dot product over the product of their magnitudes */

double angleBetweenVectors (vector A, vector B)
{
 double dotprod, magA, magB;
 dotprod = dotP(A,B);              printf("angleBetweenVectors\n");
 magA = sqrt((A.x*A.x) + (A.y*A.y) + (A.z*A.z));
 magB = sqrt((B.x*B.x) + (B.y*B.y) + (B.z*B.z));
 return acos(dotprod/(magA*magB));
}



/********************************************/
/*returns a value of t for the parametric eq. of the line from p to r */
/*where the line intersects the plane which the vector eye-r is a normal */
/*and eye is in the plane */

double pointOfIntersection(point3d p, point3d r)
{
 double t;
 t = (dotP(p,eye)-dotP(r,r)+dotP(r,eye)-dotP(eye,eye))/
        (dotP(p,r)-dotP(p,eye)+dotP(r,eye)-dotP(r,r));
 return t;
}
/********************************************/
/*boolean function, returns true if p and r are on the opposite sides  */
/*of the plane to which v is a normal and r is the tip of v  */
 
int oppositeSide(point3d p, point3d r, vector v)
{
 vector rp;
 double t;                       printf("oppositeSide\n");
 rp.x = p.x-r.x;
 rp.y = p.y-r.y;
 rp.z = p.z-r.z;

 return(dotP(rp,v)!=0)        /* if rp isn't parallel to plane */
        &&((t=pointOfIntersection(p,r))<=1)     
        &&(t>=0);                                 /* and 0<t<1 */
}



/********************************************/
/*given a 3D point, returns the theta and phi of the view from the global 'eye' */

angles findAngles(point3d point)
{
 angles result;
 point3d right;
 vector viewPlaneV, pointPlaneV;
 vector viewV, pointV, upV;
                                 printf("findAngles\n");
 viewV.x = (view.x-eye.x);
 viewV.y = (view.y-eye.y);
 viewV.z = (view.z-eye.z);
 pointV.x = (point.x-eye.x);
 pointV.y = (point.y-eye.y);
 pointV.z = (point.z-eye.z);
 upV.x = 0;
 upV.y = 1;
 upV.z = 0;

 result.phi = angleBetweenVectors(viewV,pointV);
 viewPlaneV = crossP(viewV, upV);
 pointPlaneV = crossP(viewV, pointV);
 result.theta = angleBetweenVectors(viewPlaneV, pointPlaneV);
 right.x = viewPlaneV.x+eye.x;
 right.y = viewPlaneV.y+eye.y;
 right.z = viewPlaneV.z+eye.z;
                                                            
 if(oppositeSide(point, right, viewPlaneV))
   {
     result.theta = -result.theta;
   }                                                        
 return result;   
}

/********************************************/
/* given theta and phi, the view angles, return points to be plotted */

point2d findScreenPoints(angles anglePair)
{
 point2d result;                     printf("findScreenPoints\n");
 result.x = (int)floor(WINDOWSIZE/width * anglePair.phi * cos(anglePair.theta) + WINDOWSIZE/2);
 result.y = (int)floor(WINDOWSIZE/width * anglePair.phi * sin(anglePair.theta) + WINDOWSIZE/2);
 return result; 
} 
/*******************************************/

void DrawTest(Display* display, Window window, GC gc)
{                                                      
                                                                
 int k;                              printf("drawtest %lf %lf \n",width,eye.x);        
 for(k=0;k<numpoints;k++)
   {
    printf("verts: %lf %lf %lf\n",verts3d[k].x,verts3d[k].y,verts3d[k].z);
    viewAngles[k] = findAngles(verts3d[k]);
    printf("angles: %lf %lf\n",viewAngles[k].phi,viewAngles[k].theta);
    screenVerts[k] = findScreenPoints(viewAngles[k]);
    printf("points: %d %d\n",screenVerts[k].x,screenVerts[k].y);
   }

 for(k=0;k<numlines;k++)
   {
    XDrawLine(display, window, gc, screenVerts[lines[k].from].x,
                                   screenVerts[lines[k].from].y,
                                   screenVerts[lines[k].to].x,
                                   screenVerts[lines[k].to].y);        
  }
  XFlush(global_display); 
}

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
   global_region = (Region) NULL;
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

static void procCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
 XmScaleCallbackStruct* ptr;
 int* value;
 ptr = (XmScaleCallbackStruct*) call_data;
  value = (int*) client_data;
  if (value != (int*) NULL)
   {
    *value = ptr->value;
   } 
  printf("value = %d \n",*value);
}

/****************************************/

void redrawCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
 XtSetSensitive(widget, False);
 XmUpdateDisplay(widget);
 XClearWindow(global_display, global_window);
 DrawTest(global_display, global_window, global_gc); 
 XFlush(global_display); 
 XtSetSensitive(widget, True);
}

/******************************************/

Widget CreateControlWindow(Widget parent, char* app_name, char* app_class)
{
 Widget dialog, row;
 Widget eyeX, eyeY, eyeZ;
 Widget viewX, viewY, viewZ;
 Widget widthget;
 Widget redraw;
 Arg args[20];
 int n;

 dialog = CreateShell(parent, app_name, app_class);
 n = 0;
 row = XmCreateRowColumn(dialog, "row", args, n);

 eyeX = CreateHSlider(row, "eyeX", -20, 20, "Eye Position, X", 
                         (XtCallbackProc) procCB,
                         (XtPointer) &(eye.x));

 eyeY  = CreateHSlider(row, "eyeY", -20, 20, "Eye Position, Y", 
                         (XtCallbackProc) procCB,
                         (XtPointer) &(eye.y));

 eyeZ  = CreateHSlider(row, "eyeZ", -20, 20, "Eye Position, Z", 
                         (XtCallbackProc) procCB,
                         (XtPointer) &(eye.z));

 viewX  = CreateHSlider(row, "viewX", -20, 20, "Center of View, X", 
                         (XtCallbackProc) procCB,
                         (XtPointer) &(view.x));

 viewY  = CreateHSlider(row, "viewY", -20, 20, "Center of View, Y", 
                         (XtCallbackProc) procCB,
                         (XtPointer) &(view.y));

 viewZ  = CreateHSlider(row, "viewZ", -20, 20, "Center of View, Z", 
                         (XtCallbackProc) procCB,
                         (XtPointer) &(view.z));

 widthget  = CreateHSlider(row, "widthget", 1, 3, "Width of View", 
                         (XtCallbackProc) procCB,
                         (XtPointer) &(width));

 redraw = (Widget) CreatePushbutton(row, "redraw", (XtCallbackProc) redrawCB,
                           (XtPointer) NULL);
 XtManageChild(row);
 XtRealizeWidget(dialog);
 return dialog;
}

/*************************************************************/

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
 Widget dialog;                     
 Arg    args[20];
 int    n, i;                            int k;
 int    screen;

 FILE *f1, *f2;
 f1= fopen("data.vertices","r");
 fscanf(f1," %d", &numpoints);
 for (i=0;i<numpoints;i++)
   {
    fscanf(f1," %lf %lf %lf", &verts3d[i].x, & verts3d[i].y, &verts3d[i].z);
  }
 fclose(f1);
 f2= fopen("data.lines","r");
 fscanf(f2," %d", &numlines);
 for (i=0;i<numlines;i++)
   {
    fscanf(f2," %d %d", &lines[i].from, &lines[i].to);
  }
 fclose(f2); 
                                   

                                   
 for(k=0;k<numpoints;k++)
   {
    printf("In Main, verts3d: %lf %lf %lf\n",verts3d[k].x,verts3d[k].y,verts3d[k].z);
  }

 n = 0;
 XtSetArg(args[n], XmNmappedWhenManaged, False); n++;
 XtSetArg(args[n], XmNallowResize, False); n++;
 XtSetArg(args[n], XmNwidth, WINDOWSIZE); n++;
 XtSetArg(args[n], XmNheight, WINDOWSIZE); n++;
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

 dialog = CreateControlWindow(parent, argv[0], "ProX");

 XmMainWindowSetAreas(mainwindow, menubar, (Widget) NULL,
                      (Widget) NULL, (Widget) NULL, drawingarea);
 XtManageChild(mainwindow);
 XtRealizeWidget(parent);
 global_display = XtDisplay(drawingarea);
 global_window = XtWindow(drawingarea);
 screen = DefaultScreen(global_display);
 global_gc = XCreateGC(global_display, global_window, 0L, (XGCValues*) NULL);
 XSetForeground(global_display, global_gc,
               BlackPixel(global_display, screen));
 XtMapWidget(parent);
 XtAppMainLoop(app_context);
 return 0;
}

   












