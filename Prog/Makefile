CC = gcc
CFLAGS = -Wall
LDFLAGS = -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lqtc -lm
SRC = src/main.c
OBJ = $(SRC:src/%.c=obj/%.o)
TARGET = codec

all: $(TARGET)

obj:
	mkdir -p obj

$(TARGET): obj $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj $(TARGET)

.PHONY: all clean
