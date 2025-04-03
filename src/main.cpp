// Import neccessary libraries
#include "header.hpp"
#include <numbers>
#include <cmath>
#include <fstream>
#include <iomanip>

// Begins the program
int main()
{
	// Defines the variables which contain the user specified parameters of the system
	SystemParameters system_params;
	double data_points;
	double max_altitude;
	double angle;
	double satellite_distance;

	// Defines the structs which will store the final outputs of the program
	OutputParameters outputs;

	// Sets up a stream for importing parameters from an input file
	std::ifstream parameterFile;
	parameterFile.open("input_parameters");

	//Imports the parameters from the file
	std::string dummy;
	std::getline(parameterFile, dummy);
	parameterFile >> dummy >> system_params.aperture_laser;
	parameterFile >> dummy >> system_params.satellite_altitude;
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
	parameterFile >> dummy >> angle;
	parameterFile.close();

	// Calculate the distance from Alice to the satellite
	satellite_distance = calculate_satellite_distance(angle, system_params.satellite_altitude); 

	// Sets up a data file to write the final values into
	std::ofstream dataFile;
	dataFile.open("output_data", std::ios::out | std::ios::trunc);
	dataFile << std::fixed << std::setprecision(40);

	// Loops through all wavelengths of the laser between 100nm and 2000nm, performing the turbulence calculations each time
	for(int i = (100.0*data_points); i <= (2000.0*data_points); ++i)
	{
		// Calculates the wavelength and wavenumber of the laser for this iteration
		system_params.wavelength_laser = (double) (i / data_points) * 1.0e-9;
		system_params.wavenumber_laser = (2.0 * std::numbers::pi) / system_params.wavelength_laser;

		// Calculates the transmissivity, PLOB bound, scintillation flux variance, beam widening, beam wandering, and coherence length
		outputs = calculate_system_parameters(system_params, angle, satellite_distance);

		// Outputs the calculated values to a file
		#pragma omp critical
		{
			dataFile << system_params.wavelength_laser << " " << outputs.total_transmissivity << " " << outputs.PLOB_bound << " " << outputs.scintillation << " " << outputs.diffraction_transmissivity << " " << outputs.beam_widening << " " << outputs.beam_wander << " " << outputs.coherence_length << " " << outputs.extinction_transmissivity << std::endl;
		}
	}

	// Ends the program
	return 0;
}
