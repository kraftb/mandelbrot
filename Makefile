
INCLUDE=-I./include -I/usr/include/SDL
LIBS=-lSDL -lm -lpng
LIBS_CL=-lOpenCL

CFLAGS=-O3 -msse2 -mavx

SOURCES=	mandel_misc.c		\
            mandel_alg.c        \
			mandel_png.c
#			mandel_video.c


SOURCES_DEFAULT=	mandelbrot.c		\

SOURCES_CL=         mandelbrot_opencl.c

HEADERS=	include/mandel_alg.h		\
			include/mandel_const.h	\
			include/mandel_misc.h	\
			include/mandel_video.h

OBJECTS=	mandel_misc.o		\
			mandel_png.o        \
            mandel_alg.o
#			mandel_video.o

OBJECTS_DEFAULT=	mandelbrot.o    \

OBJECTS_CL=         mandelbrot_opencl.o

all: mandelbrot

clean:
	rm -f *.o
	rm -f mandelbrot
	rm -f mandelbrot_opencl
	rm -f output.png
	rm -rf animate/output

opencl: mandelbrot_opencl

$(OBJECTS): %.o: %.c $(HEADERS)
	gcc $(CFLAGS) -c $(INCLUDE) -o $@ $<

$(OBJECTS_DEFAULT): %.o: %.c $(HEADERS)
	gcc $(CFLAGS) -c $(INCLUDE) -o $@ $<

$(OBJECTS_CL): %.o: %.c $(HEADERS) mandel_kernel.cl
	gcc $(CFLAGS) -c $(INCLUDE) -o $@ $<


mandelbrot_opencl: $(OBJECTS) $(OBJECTS_CL) $(SOURCES) $(SOURCES_CL) $(HEADERS) mandel_kernel.cl
	gcc -o mandelbrot_opencl $(LIBS) $(LIBS_CL) $(OBJECTS) $(OBJECTS_CL)

mandelbrot: $(OBJECTS) $(OBJECTS_DEFAULT) $(SOURCES) $(SOURCES_DEFAULT) $(HEADERS)
	gcc -o mandelbrot $(LIBS) $(OBJECTS) $(OBJECTS_DEFAULT)


