CC = g++
CXXFLAGS = -std=c++1z 

all: server

clean:
	@rm -rf ./build/*.o
	@rm -rf ./build/server

./build/server: ./build/main.o ./build/server.o
	$(CC) $(CXXFLAGS) -o ./build/server $^

./build/main.o: ./src/main.cpp ./src/server.h
	$(CC) $(CXXFLAGS) -c -o ./build/main.o ./src/main.cpp

./build/server.o: ./src/server.cpp ./src/server.h
	$(CC) $(CXXFLAGS) -c -o ./build/server.o ./src/server.cpp