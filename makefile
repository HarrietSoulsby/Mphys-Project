# Compiler
CC=g++
# Libraries
LIBS=-lstdc++ -lgsl -lgslcblas
# Compilation flags
CC_CFLAGS=-O3 -mtune=native -I./hdr
# Link flags
CC_LDFLAGS= -lstdc++ -I./hdr
# Object (source) files
OBJECTS= obj/main.o obj/calculate_satellite_distance.o obj/calculate_height.o obj/integrate_turbulence.o
# Name of generated executable
EXECUTABLE=exe
# Compilation stuff - do not edit
all: $(OBJECTS) serial
serial: $(OBJECTS)
	$(CC) $(CC_LDFLAGS) $(LIBS) $(OBJECTS) -o $(EXECUTABLE)
$(OBJECTS): obj/%.o: src/%.cpp
	$(CC) -c -o $@ $(CC_CFLAGS) $<
clean:
	@rm -f obj/*.o
