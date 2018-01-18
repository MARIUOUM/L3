CC 	= gcc -g
CFLAGS 	= -Wall --pedantic -O2 -std=c11
HFLAGS  = -Ilib
HXML	= `xml2-config --cflags`
LIBXML	= `xml2-config --libs`
HSDL	= `sdl2-config --cflags`
LIBSDL	= `sdl2-config --libs` -lSDL2_gfx -lSDL2_ttf -lm

VPATH 	= src:lib
EXEC 	= openMap

LIB 	= $(patsubst lib/%.c, lib/%.o, $(wildcard lib/*.c))

HEADERS	= $(wildcard lib/*.h)
OBJECTS	= $(patsubst src/%.c, src/%.o, $(wildcard src/*.c)) $(LIB)


all : lib exe

exe : $(EXEC)

lib : $(LIB)

openMap: $(OBJECTS) $(LIB)
	$(CC) -o $@ $^ $(LIBXML) $(LIBSDL)

%.o : %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(HXML) $(HSDL) $(HFLAGS)

clean :
	rm -f $(OBJECTS)

mrproper: clean
	rm -f $(EXEC) $(LIB)
