CC=g++
CXXFLAGS=-g -Wall -std=c++0x
LDLIBS=-lGL -lGLU -lglut

all:	snake
snake:	snake.cc snake.h
		$(CC) $(CXXFLAGS) $^ $(LDLIBS) -o $@
clean:  
	rm  -f snake
