# Compiler
CC=g++
# Libraries
LIBS=-lstdc++ -lgsl -lgslcblas
# Compilation flags
CC_CFLAGS=-O3 -mtune=native -I./hdr -std=c++20 -fopenmp
# Link flags
CC_LDFLAGS= -lstdc++ -I./hdr -std=c++20 -fopenmp
# Object (source) files
OBJECTS= obj/main.o obj/calculate_satellite_distance.o obj/calculate_height.o obj/integrate_turbulence.o obj/calculate_beam_intensity.o obj/calculate_diffraction_transmissivity.o obj/calculate_extinction_transmissivity.o
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
