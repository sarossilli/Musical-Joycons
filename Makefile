COMPILER := g++
INCLUDE := include/*

build:
	g++ -o bin/build src/main.cpp src/midi/*.cpp -Llibs/ -lhidapi-hidraw -Iinclude/ -Iinclude/midi/
