CFLAGS=-Wall -O3

UMLSLDPMapping: bin/UMLSLDPMapping

bin/UMLSLDPMapping: .obj/main.o .obj/Dictionary.o .obj/layeredDP.o
	g++ $(CFLAGS) $^ -o bin/UMLSLDP

.obj/%.o: src/%.cpp
	g++ $(CFLAGS) $< -c -o $@

clean:
	rm -f bin/*
	rm -f .obj/*.o
