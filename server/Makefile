CXX_FLAGS = -g  

all: main.o
	g++ -g main.o -o server

main.o: main.cpp
	g++  $(CXX_FLAGS) -o main.o -c main.cpp


.PHONY: clean
clean:
	rm -f server main.o
  
