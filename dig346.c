/*This is the program code that reads data from a digitizing pad - based on a program written by Dean Brock*/
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
   FILE *outf ;
   int ttyfd, cx, cy, rx, ry,
       llx, lly,urx, ury, maxx, diffx, maxy, debug, numpts ;
   char button, inbuff[14], filename[101] ;
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

   (void) fputs("Mash on lower left co-ordinate\n\n", stdout) ;
   (void) read(ttyfd, inbuff, 13) ;
   (void) sscanf(inbuff, "%1cD%5d%5d\n", &button, &llx, &lly) ;
   if (debug)
      (void) printf("Lower left  = (%d, %d)\n", llx, lly) ;
   eatum(ttyfd, MSECWAIT) ;

   (void) fputs("Mash on upper right co-ordinate\n\n", stdout) ;
   (void) read(ttyfd, inbuff, 13) ;
   (void) sscanf(inbuff, "%1cD%5d%5d\n", &button, &urx, &ury) ;
   if (debug)
      (void) printf("Upper right = (%d, %d)\n\n", urx, ury) ;

   diffx = urx-llx ;
   (void) printf("Enter the maximum value for x:  ") ;
   (void) fflush(stdout) ;
   (void) scanf("%d", &maxx) ;
   maxy = (ury-lly)*maxx/diffx ;
   (void) printf("\nCo-ordinates should be from (0,0) to (%d,%d)\n\n",
		 maxx, maxy) ;

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
   (void) printf("\nWriting to output file %s\n\n", filename) ;

   eatum(ttyfd, MSECWAIT) ;
   (void) fputs("Begin entering co-ordinates\n", stdout) ;
   (void) fputs("End by pressing the E button\n\n", stdout) ;
   numpts = 0 ;
   (void) read(ttyfd, inbuff, 13) ;
   (void) sscanf(inbuff, "%1cD%5d%5d\n", &button, &cx, &cy) ;
   while (button != 'E')
     {
      
      rx = (cx-llx)*maxx/diffx ;
      ry = (cy-lly)*maxx/diffx ;
      if (debug)
	 (void) printf("(%5d,%5d) --> (%3d,%3d)\n", cx, cy, rx, ry) ;
      else
        {
	 if (numpts % 10 == 0)
	    (void) printf("\n%3d ", numpts) ;
	 if (rx < 0 || rx > maxx || ry < 0 || ry > maxy)
	    (void) fputc('!', stdout) ;
	 else
	    (void) fputc('*', stdout) ;
	 (void) fflush(stdout) ;
        }
      (void) fprintf(outf, " %4d %4d\n", rx, ry) ;
      ++numpts  ;
      eatum(ttyfd, MSECWAIT) ;
      (void) read(ttyfd, inbuff, 13) ;
      (void) sscanf(inbuff, "%1cD%5d%5d\n", &button, &cx, &cy) ;
     }
   (void) fclose(outf) ;
   (void) printf("\n\n%d points written to file %s\n", numpts, filename) ;
      
  } /* main */
