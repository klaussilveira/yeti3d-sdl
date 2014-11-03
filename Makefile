TARGET = yeti
SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LDFLAGS = $(shell sdl2-config --libs)

CC = gcc
CFLAGS = -D __SDL__ -O2 -g -Wall $(SDL_CFLAGS)
LDFLAGS = -lm $(SDL_LDFLAGS)

SOURCES?=${shell find src/game -type f -iname '*.h'}
OBJECTS?= $(patsubst %.h,%.o, ${SOURCES}) \
		src/y3d/y3d_all.o \
		src/platform/sdl/main.o

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o game/*.o game/ai/*.o y3d/*.o ${TARGET}
