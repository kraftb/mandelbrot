
INCLUDE=-I./include -I/usr/include/SDL
LIBS=-lSDL -lm

SOURCES=	mandelbrot.c		\
			mandel_video.c		\
			mandel_alg.c		\
			mandel_misc.c

HEADERS=	include/mandel_alg.h		\
			include/mandel_const.h	\
			include/mandel_misc.h	\
			include/mandel_video.h

OBJECTS=	mandelbrot.o		\
			mandel_video.o		\
			mandel_alg.o		\
			mandel_misc.o


all: mandelbrot

clean:
	rm -f *.o

$(OBJECTS): %.o: %.c $(HEADERS)
	gcc -c $(INCLUDE) -o $@ $<

mandelbrot: $(OBJECTS) $(SOURCES) $(HEADERS)
	gcc -o mandelbrot $(LIBS) $(OBJECTS)


