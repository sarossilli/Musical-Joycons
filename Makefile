COMPILER := g++
INCLUDE := include/*

build:
	g++ -pthread -o bin/build src/main.cpp src/midi/*.cpp -Llibs/* -lhidapi-hidraw -Iinclude/* -Iinclude/midi/*
