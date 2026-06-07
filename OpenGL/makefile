CC = g++
FLAGS = -Wall -std=c++20
LIBS = -lglfw3 -lGL

TARGETS = main

all: $(TARGETS)

# Remeber to have a lib folder containing glad
main: src/main.cpp lib/glad.c 
	$(CC) $(FLAGS) src/main.cpp lib/glad.c -o main  $(LIBS)

clean:
	rm -f $(TARGETS)