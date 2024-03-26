CC=gcc
CFLAGS=-std=c2x -O2 -g3 -Wall -Wextra -Wstrict-aliasing -Wpointer-arith 

LD=gcc
LDFLAGS=lib/glad/glad.o lib/cglm/build/libcglm.a lib/glfw/build/src/libglfw3.a -lm -lopenal -laudio
INCLUDE=-Ilib/glad/include -Ilib/glfw/include -Ilib/cglm/include -Ilib/stb

OBJ=obj
SRC=src
BIN=bin

SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

.PHONY: all dirs lib pong cmake clean
all: dirs lib pong

dirs:
	mkdir -p obj bin

lib:
	cd lib/glad && $(CC) -Iinclude -o glad.o -c src/glad.c
	cd lib/glfw && cmake . -B build && cd build && make
	cd lib/cglm && cmake . -B build -DCGLM_STATIC=ON && cd build && make

$(OBJ)/%.o:$(SRC)/%.c
	$(CC) -o $@ -c $^ $(CFLAGS) $(INCLUDE)

pong: $(OBJS)
	$(LD) -o $(BIN)/$@ $^ $(LDFLAGS)
	./$(BIN)/$@

cmake:
	cmake -B build -S . -G Ninja
	cd build && ninja && ./pong

clean:
	cd lib/glfw/build && make clean
	cd lib/cglm/build && make clean
	rm -rf $(OBJ) $(BIN)
