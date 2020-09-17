build:
	g++ -o build src/main.cpp src/midi/*.cpp -Llibs/ -lhidapi-hidraw -Iinclude/
miditest:
	g++ -o midi src/midi/*.cpp -Llibs/ -lhidapi-hidraw -Iinclude/