Xspec: Xspec.o XspecMath.o XspecConv.o
	gcc -o Xspec -I/usr/include -I/usr/X11R6/LessTif/Motif2.1/include -L/usr/lib/gcc-lib/i386-redhat-linux/2.96  -L/usr/X11R6/LessTif/Motif2.1/lib -L/usr/X11R6/lib -lXm -lXt -lX11 -lm Xspec.o XspecMath.o XspecConv.o

Xspec.o: Xspec.c XspecMath.h XspecConv.h
	gcc -I/usr/X11R6/LessTif/Motif2.1/include -L/usr/X11R6/LessTif/Motif2.1/lib -lXm -lXt -lX11 -c Xspec.c 

XspecMath.o: XspecMath.c
	gcc -I/usr/include -I/usr/X11R6/LessTif/Motif2.1/include -L/usr/X11R6/LessTif/Motif2.1/lib -lXm -lXt -lX11 -c XspecMath.c 

XspecConv.o: XspecConv.c
	gcc -I/usr/X11R6/LessTif/Motif2.1/include -L/usr/X11R6/LessTif/Motif2.1/lib -lXm -lXt -lX11 -c XspecConv.c 

