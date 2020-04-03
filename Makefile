CPP = g++
C = gcc
CXXFLAGS = -std=c++17

all: ./build/server ./build/cgi

clean:
	@rm -rf ./build/*.o
	@rm -rf ./build/server

./build/server: ./build/main.o ./build/server.o ./build/request.o
	$(CPP) $(CXXFLAGS) -o ./build/server $^

./build/cgi: ./src/cgi.c
	$(C) -o ./build/cgi $^

./build/main.o: ./src/main.cpp ./src/server.h
	$(CPP) $(CXXFLAGS) -c -o ./build/main.o ./src/main.cpp

./build/server.o: ./src/server.cpp ./src/server.h
	$(CPP) $(CXXFLAGS) -c -o ./build/server.o ./src/server.cpp

./build/request.o: ./src/request.cpp ./src/request.h
	$(CPP) $(CXXFLAGS) -c -o ./build/request.o ./src/request.cpp