CC = g++
CXXFLAGS = -std=c++17

all: ./build/server ./build/cgi

clean:
	@rm -rf ./build/*.o
	@rm -rf ./build/server
	@rm -rf ./build/cgi

./build/server: ./build/main.o ./build/server.o ./build/request.o
	$(CC) $(CXXFLAGS) -o ./build/server $^

./build/cgi: ./src/cgi.cpp ./src/request.cpp
	$(CC) $(CXXFLAGS) -o ./build/cgi $^

./build/main.o: ./src/main.cpp ./src/server.h
	$(CC) $(CXXFLAGS) -c -o ./build/main.o ./src/main.cpp

./build/server.o: ./src/server.cpp ./src/server.h
	$(CC) $(CXXFLAGS) -c -o ./build/server.o ./src/server.cpp

./build/request.o: ./src/request.cpp ./src/request.h
	$(CC) $(CXXFLAGS) -c -o ./build/request.o ./src/request.cpp