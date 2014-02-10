TARGET = yeti
SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LDFLAGS = $(shell sdl2-config --libs)

CC = gcc
CFLAGS = -D __SDL__ -O2 -g -Wall $(SDL_CFLAGS)
LDFLAGS = -lm $(SDL_LDFLAGS)

OBJECTS = src/game/data.o \
		src/game/entities.o \
		src/game/game.o \
		src/game/maps.o \
		src/game/models.o \
		src/game/screens.o \
		src/game/sprites.o \
		src/y3d/y3d_all.o \
		src/platform/sdl/main.o

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o game/*.o game/ai/*.o y3d/*.o
