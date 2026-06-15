CC = g++
FLAGS = -Wall -std=c++20 -MMD -MP
LIBS = -lglfw3 -lGL

TARGETS = main

all: $(TARGETS)

main: main.o lib/glad.o
	$(CC) $(FLAGS) main.o lib/glad.o -o main $(LIBS)

main.o: src/main.cpp
	$(CC) $(FLAGS) -c src/main.cpp -o main.o

lib/glad.o: lib/glad.c
	$(CC) $(FLAGS) -c lib/glad.c -o lib/glad.o

-include main.d lib/glad.d

clean:
	rm -f $(TARGETS) main.o main.d lib/glad.o lib/glad.d