# Compiler
CC=g++
# Libraries
LIBS= -lgsl -lgslcblas -lm
# Compilation flags
CC_CFLAGS=-O3 -mtune=native -I./hdr -std=c++20 -fopenmp
# Link flags
CC_LDFLAGS= -fopenmp $(LIBS)
# Object (source) files
OBJECTS= obj/main.o obj/calculate_Cn2.o obj/calculate_satellite_distance.o obj/calculate_delta_n.o obj/calculate_diffraction.o obj/integrate_turbulence.o obj/calculate_extinction.o obj/calculate_height.o obj/calculate_system_parameters.o obj/calculate_PLOB.o obj/calculate_scintillation.o
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
