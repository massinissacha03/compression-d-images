CC = gcc
CFLAGS = -Wall -fPIC -Iinclude
LDFLAGS = -shared
SRC = src/qtc.c src/codage.c src/decodage.c src/segmentation.c src/filtrage.c src/image.c src/Quadtree.c
OBJ = $(SRC:src/%.c=obj/%.o)
TARGET = libqtc.so

all: build $(TARGET)

build:
	mkdir -p obj

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

obj/%.o: src/%.c include/qtc.h 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj $(TARGET)

.PHONY: all clean build
