CFLAGS = `pkg-config --cflags gtk+-2.0` -std=c++11
LDLIBS = `pkg-config --libs gtk+-2.0`
FILES = main.c

all:
	g++ $(CFLAGS) $(FILES) -o pexeso $(LDLIBS)

clean :
	rm -f pexeso.o pexeso
