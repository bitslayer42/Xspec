/*************************************
             digitize.c
copyright 1996 Jon T. Bierce-Wilson
based on "dig346.c" by Dean Brock
**************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>

#define MSECWAIT 50
#define MAXPOINTS 300
#define MAXLINES 450

void eatum(int fdin, int msec)
  {
   struct timeval waittime ;
   fd_set fdmask ;
   char eatitup[14] ;
   waittime.tv_sec  = msec/1000 ;
   waittime.tv_usec = (msec%1000)*1000 ;
   FD_ZERO(&fdmask) ;
   FD_SET(fdin, &fdmask) ;
   while (select(fdin+1,&fdmask,(fd_set *)NULL,(fd_set *)NULL,&waittime) == 1)
     {
      (void) read(fdin, eatitup, 13) ;
      FD_SET(fdin, &fdmask) ;
     }
  }

void main(int argc, char * argv[])
  {
   FILE *outf;
   int ttyfd, cx, cy, i,
       llx, lly,urx, ury, debug, numpts, numlines ;
   double maxx, diffx, maxy, rz, state;
   struct {double x; double y;} pointArray[MAXPOINTS];
   struct {int from; int to;} lineArray[MAXLINES];
   char button, inbuff[14], filename[101], filename2[101] ;
   debug = (argc == 2 && !strcmp(argv[1], "-d")) ;
#ifdef __alpha
   ttyfd = open("/dev/tty00", O_RDONLY) ;
#else
   ttyfd = open("/dev/tty16", O_RDONLY) ;
#endif
   if (ttyfd < 0)
     {
      perror("open") ;
      exit(1) ;
     }

   (void) fputs("This program creates datafiles for the program Xspec\n",stdout);
   (void) fputs("using a digitizing tablet\n",stdout);
   (void) fputs("Mash on origin with left(0) digitizer button\n\n", stdout) ;
   (void) read(ttyfd, inbuff, 13) ;
   (void) sscanf(inbuff, "%1cD%5d%5d\n", &button, &llx, &lly) ;
   if (debug)
      (void) printf("Lower left  = (%d, %d)\n", llx, lly) ;
   eatum(ttyfd, MSECWAIT) ;

   (void) fputs("Mash on a first quadrant (upper right) reference point\n",stdout);
   (void) fputs("with a known integer X value.\n\n", stdout) ;
   (void) read(ttyfd, inbuff, 13) ;
   (void) sscanf(inbuff, "%1cD%5d%5d\n", &button, &urx, &ury) ;
   if (debug)
      (void) printf("Upper right = (%d, %d)\n\n", urx, ury) ;

   diffx = (double)urx-llx ;
   (void) printf("Enter the value for X at the reference point:  ") ;
   (void) fflush(stdout) ;
   (void) scanf("%lf", &maxx) ;
   maxy = (ury-lly)*maxx/diffx ;
   (void) printf("\nThe Y co-ordinate of the reference point is %f)\n\n",
		 maxy) ;
   (void) printf("Enter the value of the constant Z plane: ");
   (void) fflush(stdout);
   (void) scanf("%lf",&rz);

   (void) fgets(filename, 100, stdin) ;
   (void) fputs("Enter the output file name:  ", stdout) ;
   (void) fflush(stdout) ;
   (void) fgets(filename, 100, stdin) ;
   filename[strlen(filename) - 1] = '\0' ;
   outf = fopen(filename, "w") ;
   if (outf == NULL)
     {
      (void) printf("Unable to open %d\n", filename) ;
      perror("open") ;
      exit(1) ;
     }

   printf("\nWriting to output file %s\n\n", filename) ;

   eatum(ttyfd, MSECWAIT) ;
   (void) fputs("Add each point in polyline with left (0) click.\n", stdout) ;
   (void) fputs("Click right (3) for final point in each polyline.\n", stdout) ;
   (void) fputs("Continue with new polyline or\n", stdout) ;
   (void) fputs("end by pressing the E button.\n\n", stdout) ;

   numpts = 0 ; numlines = 0; state = 0;
   (void) read(ttyfd, inbuff, 13) ;
   (void) sscanf(inbuff, "%1cD%5d%5d\n", &button, &cx, &cy) ;
   while (button != 'E')
     {
      if(state == 0 && button == '0')
	{
	  pointArray[numpts].x = (cx-llx)*maxx/diffx ;
	  pointArray[numpts].y = (cy-lly)*maxx/diffx ;
	  state = 1 ;
	  numpts++;
	}
      else if(state == 1)
	{
	  pointArray[numpts].x = (cx-llx)*maxx/diffx ;
	  pointArray[numpts].y = (cy-lly)*maxx/diffx ;
	  lineArray[numlines].from = numpts - 1;
	  lineArray[numlines].to = numpts;
	  numpts++;
	  numlines++;
	  if(button == '3') {state = 0;}
	}
      else
	printf("%c",0x7);

      eatum(ttyfd, MSECWAIT) ;
      (void) read(ttyfd, inbuff, 13) ;
      (void) sscanf(inbuff, "%1cD%5d%5d\n", &button, &cx, &cy) ;
     }

   fprintf(outf,"%d\n", numpts);
   for(i=0;i<numpts;i++)
    {
  (void) fprintf(outf, "%lf %lf %lf\n",pointArray[i].x,pointArray[i].y,rz);
    }

   fprintf(outf,"%d\n", numlines);
   for(i=0;i<numlines;i++)
    {
  (void) fprintf(outf, "%d %d\n",lineArray[i].from, lineArray[i].to);
    }

   (void) fclose(outf) ;
   (void) printf("\n\n%d points written to file %s\n", numpts, filename) ;
      
  } /* main */
