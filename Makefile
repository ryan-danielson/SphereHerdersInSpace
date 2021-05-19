INCLUDE_DIRS := SDL2
LIBRARY := /usr/lib/

CC 			 := gcc
CFLAGS 		 := -I$(INCLUDE_DIRS) -L$(LIBRARY) -lSDL2 -lSDL2_image

OBJS 		 := game.o map.o update.o movement.o load.o
DEPS 		 := ${INCLUDE_DIRS}/

.PHONY: clean all

all: SphereHerdersInSpace.o SphereHerdersInSpace

clean:
	rm $(OBJS) SphereHerdersInSpace SphereHerdersInSpace.o

SphereHerdersInSpace.o: game.c ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<
map.o:  map.c ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<
update.o: update.c ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<
movement.o: movement.c ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<
load.o: load.c ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

SphereHerdersInSpace: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
