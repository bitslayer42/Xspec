/*Xspec*/
/*Motif Curved Perspective Program*/
/*Jon Bierce-Wilson*/

#include <Xm/Xm.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/MainW.h>
#include <Xm/RowColumn.h>
#include <Xm/ToggleB.h>
#include <Xm/MessageB.h>
#include <Xm/BulletinB.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Xm/FileSB.h>
#define OK     1
#define CANCEL 2
#include "XspecConv.h"
#include "XspecMath.h"
#define UNSAVED 0
#define SAVED   1
#define CHANGED 2
int saveState = UNSAVED;
#define NOACTION   0
#define EXITACTION 1
#define OPENACTION 2
int actionAfterSave = 0;
#define Xchoice 0
#define Ychoice 1
#define Zchoice 2
int XYorZ;

/*void translateDialogCB(Widget , XtPointer , XtPointer );*/
void whichAxisCB(Widget , int , XtPointer );
XmStringCharSet char_set = XmSTRING_DEFAULT_CHARSET;
Display* global_display;
Window   global_window;
GC       global_gc;
Region   global_region = (Region) NULL;
 Widget parent;
 Widget mainwindow;
 Widget menubar;
 Widget filemenu, editmenu, optionsmenu;
 Widget openchoice, newchoice, savechoice, appendchoice, exitchoice;
 Widget boxeschoice, tabletchoice;
 Widget rotatechoice, translatechoice, scalechoice;
 Widget constantviewanglechoice, autoredrawchoice;
 Widget drawingarea;
 Widget dialog, row;
 Widget openDialog;
 Widget saveDialog;
 Widget cantOpenDialog;
 Widget saveQuestionDialog;
 Widget cantAppendDialog; 
 Widget outOfMemoryDialog;
 Widget appendDialog;
 Widget eyeX, eyeY, eyeZ;
 Widget viewX, viewY, viewZ;
 Widget widthget;
 Widget redraw;
 Widget rotateDialog;
 Widget translateDialog;
 Widget scaleDialog; 
 Widget rowcol; 
 Widget changeValue; 
 Widget radiorow; 
 Widget okButton; 
 Widget cancelButton; 
 Widget radio_box; 
 Widget radio_text; 
 Widget Xbutton; 
 Widget Ybutton; 
 Widget Zbutton; 

 Boolean autoRedrawMode = TRUE;
 Boolean constantViewAngle = FALSE;
 double transformAmount;

const int WINDOWSIZE=800;
double width;    /*these are the globals that can be changed */    
point3d eye;   /*from the control window */
point3d view;
double widthD;  /*widthD is in degrees, width is in radians */

vector upV;

line  *lines;
point3d *verts3d;
point2d *screenVerts;
angles *viewAngles;

line  *templines;
point3d *tempverts3d;

int numlines = 0;
int numpoints = 0;



/*******************************************/

void DrawTest(Display* display, Window window, GC gc)
{                                                      
                                                                
 int k;

 width = widthD/180*3.14;
 for(k=0;k<numpoints;k++)
   {
    viewAngles[k] = findAngles(verts3d[k]);
    screenVerts[k] = findScreenPoints(viewAngles[k]);
   }
 for(k=0;k<numlines;k++)
   {
    XDrawLine(display, window, gc, screenVerts[lines[k].from].x,
                                   screenVerts[lines[k].from].y,
                                   screenVerts[lines[k].to].x,
                                   screenVerts[lines[k].to].y);        
  }
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


/*******************************************/

static void sliderCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
 XmScaleCallbackStruct* ptr;
 double* value;
 ptr = (XmScaleCallbackStruct*) call_data;
  value = (double*) client_data;
  if (value != (double*) NULL)
   {
    *value = ptr->value;
   } 
 XtSetSensitive(widget, False);
 XmUpdateDisplay(widget);
 if(autoRedrawMode)
   {
     XClearWindow(global_display, global_window);
     DrawTest(global_display, global_window, global_gc);
   } 
 XFlush(global_display); 
 XtSetSensitive(widget, True);
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
 Arg args[20];
 int n;

 dialog = CreateShell(parent, app_name, app_class, "Control Window");
 n = 0;
 row = XmCreateRowColumn(dialog, "row", args, n);

 eyeX = CreateHSlider(row, "eyeX", -20, 20, eye.x, "Eye Position, X", 
                         (XtCallbackProc) sliderCB,
                         (XtPointer) &(eye.x));

 eyeY  = CreateHSlider(row, "eyeY", -20, 20, eye.y, "Eye Position, Y", 
                         (XtCallbackProc) sliderCB,
                         (XtPointer) &(eye.y));

 eyeZ  = CreateHSlider(row, "eyeZ", -20, 20, eye.z,"Eye Position, Z", 
                         (XtCallbackProc) sliderCB,
                         (XtPointer) &(eye.z));

 viewX  = CreateHSlider(row, "viewX", -20, 20, view.x, "Center of View, X", 
                         (XtCallbackProc) sliderCB,
                         (XtPointer) &view.x);

 viewY  = CreateHSlider(row, "viewY", -20, 20, view.y, "Center of View, Y", 
                         (XtCallbackProc) sliderCB,
                         (XtPointer) &view.y);

 viewZ  = CreateHSlider(row, "viewZ", -20, 20, view.z,"Center of View, Z", 
                         (XtCallbackProc) sliderCB,
                         (XtPointer) &view.z);

 widthget  = CreateHSlider(row, "widthget", 1, 360, widthD, "Width of View", 
                         (XtCallbackProc) sliderCB,
                         (XtPointer) &widthD);

 redraw = (Widget) CreatePushButton(row, "redraw", (XtCallbackProc) redrawCB,
                           (XtPointer) NULL);
 XtManageChild(row);
 XtRealizeWidget(dialog);
  return dialog;
}

/*************************************************************/

Widget CreateTransformDialog(Widget parent,
                             char* name,
                             char* sliderText, 
			     char* radioText,
                             XtCallbackProc callback,
			     int min,
                             int max)
{
  Widget widget;
  Arg  args[20];
  int  n;

  n = 0;
  XtSetArg(args[n],XmNautoUnmanage,False); n++;
  widget = XmCreateBulletinBoardDialog(parent, name,args,n);
  n = 0;
  XtSetArg(args[n],XmNx,10); n++;
  XtSetArg(args[n],XmNy,10); n++;
  rowcol = XmCreateRowColumn(widget,"rowcol",args,n);
  XtManageChild(rowcol);
  changeValue = CreateHSlider(rowcol,"changeValue",min,max,(max-min)/2,sliderText, 
			      (XtCallbackProc) sliderCB, (XtPointer) &transformAmount);
  n = 0;
  radiorow = XmCreateRowColumn(rowcol,"radiorow",args,n);
  XtManageChild(radiorow);
  n = 0;
  radio_box = XmCreateRadioBox(radiorow,"radio_box",args,n);
  XtManageChild(radio_box);
  Xbutton = CreateToggleButton(radio_box,"Xbutton",whichAxisCB,Xchoice);
  Ybutton = CreateToggleButton(radio_box,"Ybutton",whichAxisCB,Ychoice);
  Zbutton = CreateToggleButton(radio_box,"Zbutton",whichAxisCB,Zchoice);
  n = 0;
  XtSetArg(args[n],XmNlabelString,XmStringCreateLtoR(radioText,char_set)); n++;
  radio_text = XmCreateLabel(radiorow,"radio_text",args,n);
  XtManageChild(radio_text);
  okButton = CreatePushButton(rowcol,"okButton",callback,(XtPointer)OK);
  cancelButton = CreatePushButton(rowcol,"cancelButton",callback,(XtPointer)CANCEL);
  return widget;
}
/*************************************************************/
void openCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
 if (numpoints!=0 && saveState!= SAVED)
   {
     actionAfterSave = OPENACTION;
     XtManageChild(saveQuestionDialog);
   }
 else
   {
     XtManageChild(openDialog);
   }
}
/*************************************************************/
void saveCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
 XtManageChild(saveDialog);
}
/*************************************************************/
void appendCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
  if (numpoints == 0) 
   XtManageChild(cantAppendDialog);
 else
   XtManageChild(appendDialog);
}
/******************************************/
void exitCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
 if (numpoints!=0 && saveState!= SAVED)
   {
     actionAfterSave = EXITACTION;
     XtManageChild(saveQuestionDialog);
   }
 else
   {
     exit(0);
   }
}
/*************************************************************/
void boxesCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
  /*XtManageChild(boxesDialog);*/
}
/*************************************************************/
void tabletCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
  /*XtManageChild(tabletDialog);*/
}
/*************************************************************/
void rotateCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
 XtManageChild(rotateDialog);
}
/*************************************************************/
void translateCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
 XtManageChild(translateDialog);
}
/*************************************************************/
void scaleCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
 XtManageChild(scaleDialog);
}

/*************************************************************/
void openDialogCB(Widget widget, int client_data, 
                  XmSelectionBoxCallbackStruct *call_data)
{
 char *filename;
 FILE *fileIn;
 register int i;
 char *cookie;

 switch(client_data)
   {
   case OK:
     XmStringGetLtoR(call_data->value, char_set, &filename);
     fileIn = fopen(filename, "r");
     if (!fileIn) { XtManageChild(cantOpenDialog); break;}
/*     (void)fgets(cookie,12,fileIn);
     cookie[strlen(cookie)-1] = '\0';
     if(strcmp(cookie,"XspecFile")) {printf("No cookie"); exit(1);}
*/
     (void)fscanf(fileIn, " %d", &numpoints);
     tempverts3d= calloc(numpoints, sizeof(point3d));
     if (!tempverts3d)
       {
	 XtManageChild(outOfMemoryDialog); 
	 break;
      }
     screenVerts= calloc(numpoints, sizeof(point2d));
     if (!screenVerts)
       {	 	 
	 XtManageChild(outOfMemoryDialog); 
	 break;
      }
     viewAngles= calloc(numpoints, sizeof(angles));
     if (!viewAngles)
       {	 	 
	 XtManageChild(outOfMemoryDialog); 
	 break;
      }

     for (i=0;i<numpoints;i++)
       {
	 (void)fscanf(fileIn, " %lf %lf %lf", &tempverts3d[i].x, 
                                              &tempverts3d[i].y, 
                                              &tempverts3d[i].z);
       }
     verts3d = tempverts3d;
     (void)fscanf(fileIn," %d", &numlines);
     templines = calloc(numlines, sizeof(line));
     if (!templines)
       {	 	 
	 XtManageChild(outOfMemoryDialog); 
	 break;
      }

     for (i=0;i<numlines;i++)
       {
	 (void)fscanf(fileIn," %d %d", &templines[i].from, &templines[i].to);
       }
     lines = templines;
     fclose(fileIn);
     saveState = SAVED;
     break;
   case CANCEL: 
     break;
   }
 XtUnmanageChild(openDialog);
 actionAfterSave = NOACTION;
 XClearArea(global_display,global_window,0,0,0,0,FALSE);
 DrawTest(global_display, global_window, global_gc); 
}

/*************************************************************/
void saveDialogCB(Widget widget, int client_data, 
                  XmSelectionBoxCallbackStruct *call_data)
{
 char *filename;
 FILE *fileOut;
 register int i;
 char *cookie;

 switch(client_data)
   {
   case OK:
     XmStringGetLtoR(call_data->value, char_set, &filename);
     fileOut = fopen(filename, "w");
     if(!fileOut) { XtManageChild(cantOpenDialog); break;}
     (void) fprintf(fileOut, " %d\n", numpoints);
     for (i=0;i<numpoints;i++)
       {
	 (void) fprintf(fileOut, " %lf %lf %lf\n", verts3d[i].x, verts3d[i].y, verts3d[i].z);
       }
     (void) fprintf(fileOut," %d\n", numlines);
     for (i=0;i<numlines;i++)
       {
	 fprintf(fileOut," %d %d\n", lines[i].from, lines[i].to);
       }
     fclose(fileOut);
     saveState = SAVED;
     break;
   case CANCEL: 
     break;
   }
 XtUnmanageChild(saveDialog);
 if (actionAfterSave == EXITACTION) {exit(0);}
 else if (actionAfterSave == OPENACTION)
   {
     XtManageChild(openDialog);
   }
}
/*************************************************************/
void appendDialogCB(Widget widget, int client_data, 
                  XmSelectionBoxCallbackStruct *call_data)
{
 char *filename;
 FILE *fileIn;
 register int i;
 char *cookie;
 int numpoints2;
 int numlines2;

 switch(client_data)
   {
   case OK:
	 XmStringGetLtoR(call_data->value, char_set, &filename);
	 fileIn = fopen(filename, "r");
	 if (!fileIn) { XtManageChild(cantOpenDialog); break;}

	 /*allocate verts3d*/
	 verts3d= calloc(numpoints, sizeof(point3d));
	 if (!verts3d) 
	   {
	     XtManageChild(outOfMemoryDialog); 
	     break;
	   }
	 for(i=0;i<numpoints;i++)
	   {
	     verts3d[i].x = tempverts3d[i].x;
	     verts3d[i].y = tempverts3d[i].y;
	     verts3d[i].z = tempverts3d[i].z;
	   }
	 free(tempverts3d);
         (void) fscanf(fileIn, " %d", &numpoints2);
	 tempverts3d = calloc((numpoints+numpoints2), sizeof(point3d));
	 if (!tempverts3d)
	   {
	     XtManageChild(outOfMemoryDialog); 
	     break;
	   }
	 for (i=0;i<numpoints2;i++)
	   {
	     (void) fscanf(fileIn, " %lf %lf %lf",   &tempverts3d[i].x, 
                                  &tempverts3d[i].y, &tempverts3d[i].z);
	   }
	 for(i=0;i<numpoints;i++)
	   {
	     tempverts3d[numpoints2+i].x = verts3d[i].x;
	     tempverts3d[numpoints2+i].y = verts3d[i].y;
	     tempverts3d[numpoints2+i].z = verts3d[i].z;
	   }
	 free(verts3d);
	 verts3d = tempverts3d;
	 numpoints += numpoints2;

	 /*allocate screenVerts and viewAngles*/
	 free(screenVerts);
	 screenVerts= calloc(numpoints, sizeof(point2d));
	 if (!screenVerts) 
	   {
	     XtManageChild(outOfMemoryDialog); 
	     break;
	   }

	 free(viewAngles);
	 viewAngles= calloc(numpoints, sizeof(angles));
	 if (!viewAngles)
	   {
	     XtManageChild(outOfMemoryDialog); 
	     break;
	   }

	 /*allocate lines*/
	 lines= calloc(numlines, sizeof(line));
	 if (!lines) 
	   {
	     XtManageChild(outOfMemoryDialog); 
	     break;
	   }

	 for(i=0;i<numlines;i++)
	   {
	     lines[i].from = templines[i].from;
	     lines[i].to = templines[i].to;
	   }
	 free(templines);
         (void) fscanf(fileIn, " %d", &numlines2);
	 templines = calloc((numlines+numlines2), sizeof(line));
	 if(!templines)
	   {
	     XtManageChild(outOfMemoryDialog); 
	     break;
	   }

	 for (i=0;i<numlines2;i++)
	   {
	     (void) fscanf(fileIn," %d %d", &templines[i].from, &templines[i].to);
	   }
	 for(i=0;i<numlines;i++)
	   {
	     templines[numlines2+i].from = lines[i].from + numpoints2;
	     templines[numlines2+i].to = lines[i].to + numpoints2;
	   }
	 free(lines);
	 lines = templines;
	 numlines += numlines2;

	
	 fclose(fileIn);
	 saveState = UNSAVED;
     break;
   case CANCEL: 
     break;
   }
 XtUnmanageChild(appendDialog);
 DrawTest(global_display, global_window, global_gc); 

}
/*************************************************************/

void whichAxisCB(Widget widget, int client_data, XtPointer call_data)
{
  Boolean set;
  Arg args[10];
  int n;

  n = 0;
  XtSetArg(args[n],XmNset,&set); n++;
  XtGetValues (widget,args,n);
  if(set)
     XYorZ = client_data;
}
/*************************************************************/

void scaleDialogCB(Widget widget, int client_data, XtPointer call_data)
{
  int k;
  double transform;
 
    switch (client_data)
      {
      case OK:
	transform = transformAmount/100;
        if(XYorZ == Xchoice)
          for(k=0;k<numpoints;k++)
            verts3d[k].x *= transform;
        else if(XYorZ == Ychoice)
          for(k=0;k<numpoints;k++)
            verts3d[k].y *= transform;
        else if(XYorZ == Zchoice)
          for(k=0;k<numpoints;k++)
            verts3d[k].z *= transform;
	break;
      case CANCEL:
        break;
      }
    XtUnmanageChild(scaleDialog);
    XClearArea(global_display,global_window,0,0,0,0,FALSE);
    DrawTest(global_display, global_window, global_gc); 
    saveState = CHANGED;
}

/*************************************************************/

void translateDialogCB(Widget widget, int client_data, XtPointer call_data)
{
  int k;

    switch (client_data)
     {
     case OK:
	if(XYorZ == Xchoice)
	  for(k=0;k<numpoints;k++)
	    verts3d[k].x += transformAmount;
	else if(XYorZ == Ychoice)
	  for(k=0;k<numpoints;k++)
	    verts3d[k].y += transformAmount;
	else if(XYorZ == Zchoice)
	  for(k=0;k<numpoints;k++)
	    verts3d[k].z += transformAmount;
	break;
      case CANCEL:
        break;
     }
    XtUnmanageChild(translateDialog);
    XClearArea(global_display,global_window,0,0,0,0,FALSE);
    DrawTest(global_display, global_window, global_gc); 
    saveState = CHANGED;
}

/*************************************************************/

void rotateDialogCB(Widget widget, int client_data, XtPointer call_data)
{
  int k;
  double angleRad;
  double temp; 
    switch (client_data)
      {      
      case OK:
	angleRad = transformAmount*3.14/180;
    if(XYorZ == Xchoice)
      for(k=0;k<numpoints;k++)
	{
        temp = verts3d[k].y*cos(angleRad) - verts3d[k].z*sin(angleRad);
        verts3d[k].z = verts3d[k].y*sin(angleRad) + verts3d[k].z*cos(angleRad);
	verts3d[k].y = temp;
	}
    else if(XYorZ == Ychoice)
      for(k=0;k<numpoints;k++)
	{
        temp = verts3d[k].x*cos(angleRad) + verts3d[k].z*sin(angleRad);
        verts3d[k].z = verts3d[k].x*-sin(angleRad) + verts3d[k].z*cos(angleRad);
	verts3d[k].x = temp;
}
    else if(XYorZ == Zchoice)
      for(k=0;k<numpoints;k++)
	{
        temp = verts3d[k].x*cos(angleRad) - verts3d[k].y*sin(angleRad);
        verts3d[k].y = verts3d[k].x*sin(angleRad) + verts3d[k].y*cos(angleRad);
	verts3d[k].x = temp;
	}
	break;
      case CANCEL:
        break;
      }
    XtUnmanageChild(rotateDialog);
    XClearArea(global_display,global_window,0,0,0,0,FALSE);
    DrawTest(global_display, global_window, global_gc); 
    saveState = CHANGED;
}

/*************************************************************/

void saveQuestionDialogCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
  switch ((int) client_data)
    {
    case OK:
      XtUnmanageChild(saveQuestionDialog);
      XtManageChild(saveDialog);
      break;
    case CANCEL:
      XtUnmanageChild(saveQuestionDialog);
      if(actionAfterSave == EXITACTION) 
	exit(0);
      else if(actionAfterSave == OPENACTION)
	XtManageChild(openDialog);
      break;
    }
}

/************************************************************/

void outOfMemoryDialogCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
 XtUnmanageChild(outOfMemoryDialog);
}

/************************************************************/

void cantOpenDialogCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
 XtUnmanageChild(cantOpenDialog);
}

/************************************************************/

 void cantAppendDialogCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
 XtUnmanageChild(cantAppendDialog);
}

/************************************************************/

 void autoRedrawCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
  Boolean set;
  Arg args[20];
  int n;

  n = 0;
  XtSetArg(args[n],XmNset,&set); n++;
  XtGetValues(widget,args,n);
  if(set)
    autoRedrawMode = TRUE;
  else
    autoRedrawMode = FALSE;
}
/************************************************************/

 void constantViewAngleCB(Widget widget, XtPointer client_data, XtPointer call_data)
{
  Boolean set;
  Arg args[20];
  int n;

  n = 0;
  XtSetArg(args[n],XmNset,&set); n++;
  XtGetValues(widget,args,n);
  if(set)
    constantViewAngle = TRUE;
  else
    constantViewAngle = FALSE;
}
/************************************************************/
int main(int argc, char** argv)
{
 XtAppContext app_context;
 Arg    args[20];
 int    n;
 int screen;


String fallback_resources []=
  {
    "*background:       lightgrey",
    "*drawingarea.background: white",
    "*fontlist: lucidasans-12",
    "*filemenu.labelString: File",
    "*filemenu.mnemonic:  F",
    "*editmenu.labelString: Edit",
    "*editmenu.mnemonic:  E",
    "*openchoice.labelString:  Open",
    "*openchoice.mnemonic:  O",
    "*newchoice.labelString:  New",
    "*newchoice.mnemonic:  N",
    "*savechoice.labelString:  Save",
    "*savechoice.mnemonic:  S",
    "*appendchoice.labelString:  Append",
    "*appendchoice.mnemonic:  A",
    "*exitchoice.labelString:  Exit",
    "*exitchoice.mnemonic:  x",
    "*boxeschoice.labelString: Boxes",
    "*boxeschoice.mnemonic:  B",
    "*tabletchoice.labelString:  Graphic Tablet",
    "*tabletchoice.mnemonic:  G",
    "*rotatechoice.labelString:  Rotate",
    "*rotatechoice.mnemonic:  R",
    "*translatechoice.labelString:  Translate",
    "*translatechoice.mnemonic:  T",
    "*scalechoice.labelString:  Scale",
    "*scalechoice.mnemonic:  c",
    "*optionsmenu.labelString:  Options",
    "*optionsmenu.mnemonic:  p",
    "*autoredrawchoice.labelString:  Auto Redraw Mode",
    "*autoredrawchoice.mnemonic:  u",
    "*constantviewanglechoice.labelString:  Constant Viewangle Mode",
    "*choice.mnemonic:  V",
    NULL
  };

 upV.x = 0;
 upV.y = 1;
 upV.z = 0;
 eye.x = 2;
 eye.y = 1; 
 eye.z = -6;
 view.x =5;
 view.y =5;
 view.z =0;
  widthD = 180;

 n = 0;
 XtSetArg(args[n], XmNwinGravity, NorthEastGravity); n++;
 XtSetArg(args[n], XmNmappedWhenManaged, False); n++;
 XtSetArg(args[n], XmNallowResize, False); n++;
 XtSetArg(args[n], XmNwidth, WINDOWSIZE); n++;
 XtSetArg(args[n], XmNheight, WINDOWSIZE); n++;
 XtSetArg(args[n], XmNtitle, "Xspec Main Window"); n++;
 parent = XtAppInitialize(&app_context, "ProX", (XrmOptionDescList) NULL,
                          0, &argc, argv, fallback_resources, args, n);
 n = 0;
 XtSetArg(args[n], XmNmappedWhenManaged, True); n++;
 mainwindow = XmCreateMainWindow(parent, "main", args, n);
 n = 0;
 menubar = XmCreateMenuBar(mainwindow, "menubar", args, n);
 XtManageChild(menubar);
 filemenu = CreateMenu(menubar, "filemenu");
 editmenu = CreateMenu(menubar, "editmenu");
 optionsmenu = CreateMenu(menubar, "optionsmenu");
 openchoice = CreateMenuChoice(filemenu, "openchoice", (XtCallbackProc) openCB, 
                              (XtPointer) NULL);
 newchoice = CreateMenu(filemenu, "newchoice");
 savechoice = CreateMenuChoice(filemenu, "savechoice", (XtCallbackProc) saveCB, 
                              (XtPointer) NULL);
 appendchoice = CreateMenuChoice(filemenu, "appendchoice", (XtCallbackProc) appendCB, 
                              (XtPointer) NULL);
 exitchoice = CreateMenuChoice(filemenu, "exitchoice", (XtCallbackProc) exitCB,
                              (XtPointer) NULL);
 boxeschoice = CreateMenuChoice(newchoice, "boxeschoice", (XtCallbackProc) boxesCB, 
                              (XtPointer) NULL);
 tabletchoice = CreateMenuChoice(newchoice, "tabletchoice", (XtCallbackProc) tabletCB, 
                              (XtPointer) NULL);
 rotatechoice = CreateMenuChoice(editmenu, "rotatechoice", (XtCallbackProc) rotateCB, 
                              (XtPointer) NULL);
 translatechoice = CreateMenuChoice(editmenu, "translatechoice", (XtCallbackProc) translateCB, 
                              (XtPointer) NULL);
 scalechoice = CreateMenuChoice(editmenu, "scalechoice", (XtCallbackProc) scaleCB, 
                              (XtPointer) NULL);
 autoredrawchoice = CreateToggleButton(optionsmenu, "autoredrawchoice", 
				       (XtCallbackProc) autoRedrawCB, 
				       NULL);
 constantviewanglechoice = CreateToggleButton(optionsmenu, "constantviewanglechoice", 
					      (XtCallbackProc) constantViewAngleCB, 
					      NULL);
 n = 0;
 XtSetArg(args[n],XmNvisibleWhenOff,TRUE);
 XtSetArg(args[n],XmNset,autoRedrawMode);
 XtSetValues(autoredrawchoice,args,n);
 n = 0;
 XtSetArg(args[n],XmNvisibleWhenOff,TRUE);
 XtSetArg(args[n],XmNset,constantViewAngle);
 XtSetValues(constantviewanglechoice,args,n);


 drawingarea = CreateDrawingArea(mainwindow, "drawingarea", 640, 480,
                                (XtCallbackProc) drawCB, (XtPointer) NULL);

 /*create the dialogs*/
 dialog = CreateControlWindow(parent, argv[0], "ProX");
 n = 0;
  XtSetArg(args[n], XmNdialogTitle, XmStringCreateLtoR("Xspec: OPEN",char_set)); n++;
  XtSetArg(args[n],XmNpattern,XmStringCreateLtoR("*.xsp",char_set));  n++;
  XtSetArg(args[n],XmNdirectory,XmStringCreateLtoR("~/motif",char_set));  n++;
 openDialog = XmCreateFileSelectionDialog(parent, "openDialog", args, n);
  XtAddCallback(openDialog, XmNokCallback, openDialogCB, OK);
  XtAddCallback(openDialog, XmNcancelCallback, openDialogCB, CANCEL);
  XtUnmanageChild(XmSelectionBoxGetChild(openDialog, XmDIALOG_HELP_BUTTON));
 n = 0;
  XtSetArg(args[n], XmNdialogTitle, XmStringCreateLtoR("Xspec: SAVE",char_set)); n++;
  XtSetArg(args[n],XmNpattern,XmStringCreateLtoR("*.xsp",char_set));  n++;
  XtSetArg(args[n],XmNdirectory,XmStringCreateLtoR("~/motif",char_set));  n++;
 saveDialog = XmCreateFileSelectionDialog(parent, "saveDialog", args, n);
  XtAddCallback(saveDialog, XmNokCallback, saveDialogCB, OK);
  XtAddCallback(saveDialog, XmNcancelCallback, saveDialogCB, CANCEL);
  XtUnmanageChild(XmSelectionBoxGetChild(saveDialog, XmDIALOG_HELP_BUTTON));
 n = 0;
  XtSetArg(args[n], XmNdialogTitle, XmStringCreateLtoR("Xspec: APPEND",char_set)); n++;
  XtSetArg(args[n],XmNpattern,XmStringCreateLtoR("*.xsp",char_set));  n++;
  XtSetArg(args[n],XmNdirectory,XmStringCreateLtoR("~/motif",char_set));  n++;
 appendDialog = XmCreateFileSelectionDialog(parent, "appendDialog", args, n);
  XtAddCallback(appendDialog, XmNokCallback, appendDialogCB, OK);
  XtAddCallback(appendDialog, XmNcancelCallback, appendDialogCB, CANCEL);
  XtUnmanageChild(XmSelectionBoxGetChild(appendDialog, XmDIALOG_HELP_BUTTON));
 n = 0;
  XtSetArg(args[n], XmNdialogTitle, XmStringCreateLtoR("Xspec: ?",char_set)); n++;
  XtSetArg(args[n],XmNcancelLabelString,XmStringCreateLtoR("No",char_set));  n++;
  XtSetArg(args[n],XmNokLabelString,XmStringCreateLtoR("Yes",char_set));  n++;
  XtSetArg(args[n],XmNmessageString,XmStringCreateLtoR("Save current shape?",char_set));  n++;
 saveQuestionDialog = XmCreateMessageDialog(parent, "saveQuestionDialog", args, n);
  XtAddCallback(saveQuestionDialog, XmNokCallback, saveQuestionDialogCB, OK);
  XtAddCallback(saveQuestionDialog, XmNcancelCallback, saveQuestionDialogCB, CANCEL);
  XtUnmanageChild(XmMessageBoxGetChild(saveQuestionDialog, XmDIALOG_HELP_BUTTON));
 n = 0;
  XtSetArg(args[n], XmNdialogTitle, XmStringCreateLtoR("Xspec: !",char_set)); n++;
  XtSetArg(args[n],XmNmessageString,XmStringCreateLtoR("Unable to allocate memory",char_set));  n++;
 outOfMemoryDialog = XmCreateMessageDialog(parent, "outOfMemoryDialog", args, n);
  XtAddCallback(outOfMemoryDialog, XmNokCallback, outOfMemoryDialogCB, OK);
  XtUnmanageChild(XmMessageBoxGetChild(outOfMemoryDialog, XmDIALOG_HELP_BUTTON));
  XtUnmanageChild(XmMessageBoxGetChild(outOfMemoryDialog, XmDIALOG_CANCEL_BUTTON));
 n = 0;
  XtSetArg(args[n], XmNdialogTitle, XmStringCreateLtoR("Xspec: !",char_set)); n++;
  XtSetArg(args[n],XmNmessageString,XmStringCreateLtoR("Unable to open file",char_set));  n++;
 cantOpenDialog = XmCreateMessageDialog(parent, "cantOpenDialog", args, n);
  XtAddCallback(cantOpenDialog, XmNokCallback, cantOpenDialogCB, OK);
  XtUnmanageChild(XmMessageBoxGetChild(cantOpenDialog, XmDIALOG_HELP_BUTTON));
  XtUnmanageChild(XmMessageBoxGetChild(cantOpenDialog, XmDIALOG_CANCEL_BUTTON));
  n = 0;
   XtSetArg(args[n], XmNdialogTitle, XmStringCreateLtoR("Xspec: !",char_set)); n++;
   XtSetArg(args[n],XmNmessageString,XmStringCreateLtoR("No open file to append to, use OPEN",
                                                        char_set));  n++;
  cantAppendDialog = XmCreateMessageDialog(parent, "cantAppendDialog", args, n);
   XtAddCallback(cantAppendDialog, XmNokCallback, cantAppendDialogCB, OK);
   XtUnmanageChild(XmMessageBoxGetChild(cantAppendDialog, XmDIALOG_HELP_BUTTON));
   XtUnmanageChild(XmMessageBoxGetChild(cantAppendDialog, XmDIALOG_CANCEL_BUTTON));

  scaleDialog = CreateTransformDialog(parent,"scaleDialog","Scale percent",
				      "along which axis?",
				      scaleDialogCB,1,500);

  rotateDialog = CreateTransformDialog(parent,"rotateDialog","Rotation angle (degrees)",
				       "around which axis?",
				       rotateDialogCB,-180,180);
  translateDialog = CreateTransformDialog(parent,"translateDialog","Translation distance",
					  "along which axis?",
					  translateDialogCB,-20,20);


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

   












