CC = gcc
LD = gcc
CFLAGS = `sdl-config --cflags`
LDFLAGS = `sdl-config --libs` -lSDL_image -lGL
RM   = /bin/rm -f
OBJS = main.o object.o extra.o shadow.o

.PHONY: clean
all: gl_shadow test_obj
gl_shadow: $(OBJS)
	$(LD) $(LDFLAGS) -o gl_shadow $(OBJS)
test_obj: test_obj.o object.o
	$(LD) $(LDFLAGS) -o test_obj test_obj.o object.o
main.o: main.c extra.h object.h shadow.h
	$(CC) $(CFLAGS) -c main.c
object.o: object.c object.h
	$(CC) $(CFLAGS) -c object.c
extra.o: extra.c extra.h
	$(CC) $(CFLAGS) -c extra.c
shadow.o: shadow.c shadow.h
	$(CC) $(CFLAGS) -c shadow.c
test_obj.o: test_obj.c object.h
	$(CC) $(CFLAGS) -c test_obj.c
clean:
	$(RM) *~ $(OBJS) gl_shadow test_obj test_obj.o
