// Import neccessary libraries
#include "header.hpp"
#include <numbers>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <omp.h>

// Begins the program
int main()
{
	// Defines the variables which contain the user specified parameters of the system
	SystemParameters system_params;
	double data_points;
	double max_altitude;
	double angle;

	// Defines the variables related to the system's geometry which will change for each iteration
	double satellite_distance;
	double satellite_altitude;

	// Defines the structs which will store the final outputs of the program
	OutputParameters outputs;

	// Sets up a stream for importing parameters from an input file
	std::ifstream parameterFile;
	parameterFile.open("input_parameters");

	//Imports the parameters from the file
	std::string dummy;
	std::getline(parameterFile, dummy);
	parameterFile >> dummy >> system_params.aperture_laser;
	parameterFile >> dummy >> system_params.wavelength_laser;
	parameterFile >> dummy >> system_params.inner_scale_size;
	parameterFile >> dummy >> system_params.spot_size_laser;
	parameterFile >> dummy >> system_params.pointing_error_laser;
	parameterFile >> dummy >> system_params.detector_efficiency;
	parameterFile >> dummy >> system_params.Cn2_0;
	parameterFile >> dummy >> system_params.wind_speed;
	parameterFile >> dummy >> system_params.latitude;
	parameterFile >> dummy >> system_params.CO2_concentration;
	parameterFile >> dummy >> system_params.air_temperature;
	parameterFile >> dummy >> system_params.air_pressure;
	parameterFile >> dummy >> data_points;
	parameterFile >> dummy >> max_altitude;
	parameterFile >> dummy >> angle;
	parameterFile.close();

	// Calculates the wavenumber of the laser
	system_params.wavenumber_laser = (2.0 * std::numbers::pi) / system_params.wavelength_laser;

	// Sets up a data file to write the final values into
	std::ofstream dataFile;
	dataFile.open("output_data", std::ios::out | std::ios::trunc);
	dataFile << std::fixed << std::setprecision(40);

	// Calculates the number of iterations to perform based on the user specified required number of data points and maximum altitude
	int iterations = max_altitude*data_points;

	// Loops through all satellite altitudes between 100km and that specified by the user, performing the turbulence calculations each time
	#pragma omp parallel for private(outputs, satellite_distance, satellite_altitude) shared(system_params, data_points, angle)
	for(int i = (100.0*data_points); i <= iterations; ++i)
	{
		// Calculates the altitude of the satellite for the current iteration
		satellite_altitude = (double) (i / data_points) * 1000.0;

		// Calculate the distance from Alice to the satellite
		satellite_distance = calculate_satellite_distance(angle, satellite_altitude); 

		// Calculates the transmissivity, PLOB bound, scintillation flux variance, beam widening, beam wandering, and coherence length
		outputs = calculate_system_parameters(system_params, angle, satellite_distance);

		// Outputs the calculated values to a file
		#pragma omp critical
		{
			dataFile << satellite_altitude << " " << outputs.total_transmissivity << " " << outputs.PLOB_bound << " " << outputs.scintillation << " " << outputs.diffraction_transmissivity << " " << outputs.beam_widening << " " << outputs.beam_wander << " " << outputs.coherence_length << " " << outputs.extinction_transmissivity << std::endl;
		}
	}

	// Ends the program
	return 0;
}
