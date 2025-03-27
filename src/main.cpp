// Import neccessary libraries
#include "header.hpp"
#include <numbers>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <omp.h>
#include <iostream>

// Begins the program
int main()
{
	// Defines the variables which contain the user specified parameters of the system
	SystemParameters system_params;
	double data_points;
	TimeofdayParameters params_day;

	// Defines the variables related to the system's geometry which will change for each iteration
	double satellite_distance_zenith, satellite_distance_1r;
	double time_zenith, time_1r;

	// Defines the structs which will store the final outputs of the program
	OutputParameters outputs_zenith;
	OutputParameters outputs_1r;

	// Sets up a stream for importing parameters from an input file
	std::ifstream parameterFile;
	parameterFile.open("input_parameters");

	//Imports the parameters from the file
	std::string dummy;
	std::getline(parameterFile, dummy);
	parameterFile >> dummy >> system_params.aperture_laser;
	parameterFile >> dummy >> system_params.inner_scale_size;
	parameterFile >> dummy >> system_params.satellite_altitude;
	parameterFile >> dummy >> system_params.spot_size_laser;
	parameterFile >> dummy >> system_params.pointing_error_laser;
	parameterFile >> dummy >> system_params.detector_efficiency;
	parameterFile >> dummy >> system_params.setup_noise;
	parameterFile >> dummy >> system_params.spectral_filter;
	parameterFile >> dummy >> system_params.detection_time;
	parameterFile >> dummy >> system_params.fov_detector;
	parameterFile >> dummy >> params_day.Cn2_0;
	parameterFile >> dummy >> params_day.wind_speed;
	parameterFile >> dummy >> params_day.albedo_parameter;
	parameterFile >> dummy >> data_points;
	parameterFile.close();

	// Calculates the tangential velocity of the satellite
	system_params.satellite_velocity = std::sqrt(3.986e+14 / (system_params.satellite_altitude + 6.371e+6));

	// Sets up a data file to write the final values into
	std::ofstream dataFile;
	dataFile.open("output_data", std::ios::out | std::ios::trunc);
	dataFile << std::fixed << std::setprecision(40);

	// Calculates the number of iterations to perform based on the user specified required number of data points
	int iterations = 2000*data_points;

	// Calculates an offset to the time so that t=0s corresponds to angle=0^r
	double time_offset = std::asin(calculate_satellite_distance(-std::numbers::pi, system_params.satellite_altitude) / (system_params.satellite_altitude + 6.371e+6));

	// Defines the angles which will be investigated
	double angle_zenith = std::numbers::pi / 2.0;
	double angle_1r = (std::numbers::pi / 2.0) - 1.0;

	// Calculate the distance from Alice to the satellite
	satellite_distance_zenith = calculate_satellite_distance(angle_zenith, system_params.satellite_altitude); 
	satellite_distance_1r = calculate_satellite_distance(angle_1r, system_params.satellite_altitude); 

	// Loops through angles between 0 and 180 degrees, performing the turbulence calculations each time
	#pragma omp parallel for firstprivate(system_params, params_day) private(outputs_zenith, outputs_1r) shared(angle_zenith, angle_1r, time_offset, satellite_distance_zenith, satellite_distance_1r)
	for(int i = 1*data_points; i <= iterations; ++i)
	{
		// Determines the wavelength to investigate for the current iteration
		system_params.wavelength_laser = (double)i*1.0e-9 / (double)data_points;

		// Calculates the wavenumber of the laser
		system_params.wavenumber_laser = (2.0 * std::numbers::pi) / system_params.wavelength_laser;

		// Calculates the solar spectral irradiance at the current wavelength
		params_day.spectral_irradiance = 2.0 * 2.998e+8 * std::numbers::pi / (system_params.wavelength_laser * system_params.wavelength_laser * system_params.wavelength_laser * system_params.wavelength_laser * (std::exp(6.626e-34 * 2.998e+8 / (system_params.wavelength_laser * 1.381e-23 * 5778)) - 1.0));

		// Calculates the total transmissivity of the channel, the PLOB bound, and the secret key rate during the day and at night
		outputs_zenith = calculate_system_parameters(params_day, system_params, angle_zenith, satellite_distance_zenith);
		outputs_1r = calculate_system_parameters(params_day, system_params, angle_1r, satellite_distance_1r);

		// Outputs the calculated values to a file
		#pragma omp critical
		{
			dataFile << system_params.wavelength_laser  * 1.0e+9 << " " << outputs_zenith.transmissivity << " " << outputs_1r.transmissivity << " " << outputs_zenith.PLOB_bound << " " << outputs_1r.PLOB_bound << " " << outputs_zenith.SKR << " " << outputs_1r.SKR << std::endl;
		}
	}

	// Ends the program
	return 0;
}
