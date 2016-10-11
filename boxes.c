/*********************************
      boxes.c
copyright 1996 by Jon T. Bierce-Wilson
creates files to be used by Xspec program
*****************************************/


#include <stdio.h>
#include <stdlib.h>

main()
{
int i, j;
float d = .90;
float x, y, z;
int numrows;
int numcols;
char filename[101];
FILE *f1;
 printf("This program creates files to be used with the Xspec program.\n");
 printf("It creates files for cubes starting at 0,0,0\n");
 printf("How many boxes up y axis? ");
 scanf(" %d",&numrows);
 printf("How many boxes over x axis? ");
 scanf(" %d",&numcols);

 (void)fgets(filename,101,stdin);
 (void)fputs("Enter the output file name:  ",stdout);
 fflush(stdout);
 fgets(filename,101,stdin);
 filename[strlen(filename)-1]='\0';
 f1= fopen(filename,"w");
 if (f1 == NULL)
{
(void)printf("Unable to open file");
perror("open");
exit(1);
}
 fprintf(f1,"%d\n",numcols * numrows * 8);
 x = 0.55;
 y = 0.55;
 z = 0.55;
 for(j=0;j<numrows;j++)
  for(i=0;i<numcols;i++)
  {
    fprintf(f1,"%f %f %f \n", x+i,   y+j,   z);
    fprintf(f1,"%f %f %f \n", x+i+d, y+j,   z);
    fprintf(f1,"%f %f %f \n", x+i+d, y+j+d, z);
    fprintf(f1,"%f %f %f \n", x+i,   y+j+d, z);
    fprintf(f1,"%f %f %f \n", x+i,   y+j,   z+d);
    fprintf(f1,"%f %f %f \n", x+i+d, y+j,   z+d);
    fprintf(f1,"%f %f %f \n", x+i+d, y+j+d, z+d);
    fprintf(f1,"%f %f %f \n\n", x+i, y+j+d, z+d);
   }

 fprintf(f1," %d\n", numrows * numcols * 12);
 for (j=0;j<numrows;j++)
  for (i=0;i<numcols;i++)
   {
    fprintf(f1," %d %d\n", i*8*numrows+j*8,   i*8*numrows+j*8+1);
    fprintf(f1," %d %d\n", i*8*numrows+j*8+1, i*8*numrows+j*8+2);
    fprintf(f1," %d %d\n", i*8*numrows+j*8+2, i*8*numrows+j*8+3);
    fprintf(f1," %d %d\n", i*8*numrows+j*8+3, i*8*numrows+j*8);

    fprintf(f1," %d %d\n", i*8*numrows+j*8,   i*8*numrows+j*8+4);
    fprintf(f1," %d %d\n", i*8*numrows+j*8+1, i*8*numrows+j*8+5);
    fprintf(f1," %d %d\n", i*8*numrows+j*8+2, i*8*numrows+j*8+6);
    fprintf(f1," %d %d\n", i*8*numrows+j*8+3, i*8*numrows+j*8+7);

    fprintf(f1," %d %d\n", i*8*numrows+j*8+4, i*8*numrows+j*8+5);
    fprintf(f1," %d %d\n", i*8*numrows+j*8+5, i*8*numrows+j*8+6);
    fprintf(f1," %d %d\n", i*8*numrows+j*8+6, i*8*numrows+j*8+7);
    fprintf(f1," %d %d\n\n", i*8*numrows+j*8+7, i*8*numrows+j*8+4);

  }
 fclose(f1);
printf("Wrote file %s\n",filename); 
}
