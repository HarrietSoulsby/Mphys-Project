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
	TimeofdayParameters params_day;
	TimeofdayParameters params_night;

	// Defines the variables related to the system's geometry which will change for each iteration
	double angle;
	double angle_degrees;
	double satellite_distance;
	double time;

	// Defines the structs which will store the final outputs of the program
	OutputParameters outputs_day;
	OutputParameters outputs_night;

	// Sets up a stream for importing parameters from an input file
	std::ifstream parameterFile;
	parameterFile.open("input_parameters");

	//Imports the parameters from the file
	std::string dummy;
	std::getline(parameterFile, dummy);
	parameterFile >> dummy >> system_params.aperture_laser;
	parameterFile >> dummy >> system_params.wavelength_laser;
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
	parameterFile >> dummy >> params_day.spectral_irradiance;
	parameterFile >> dummy >> params_night.Cn2_0;
	parameterFile >> dummy >> params_night.wind_speed;
	parameterFile >> dummy >> params_night.albedo_parameter;
	parameterFile >> dummy >> params_night.spectral_irradiance;
	parameterFile >> dummy >> data_points;
	parameterFile.close();

	// Calculates the wavenumber of the laser
	system_params.wavenumber_laser = (2.0 * std::numbers::pi) / system_params.wavelength_laser;

	// Calculates the tangential velocity of the satellite
	system_params.satellite_velocity = std::sqrt(3.986e+14 / (system_params.satellite_altitude + 6.371e+6));

	// Sets up a data file to write the final values into
	std::ofstream dataFile;
	dataFile.open("output_data", std::ios::out | std::ios::trunc);
	dataFile << std::fixed << std::setprecision(40);

	// Calculates the number of iterations to perform based on the user specified required number of data points
	int iterations = 180*data_points;

	// Calculates an offset to the time so that t=0s corresponds to angle=0^r
	double time_offset = std::asin(calculate_satellite_distance(-std::numbers::pi, system_params.satellite_altitude) / (system_params.satellite_altitude + 6.371e+6));

	// Loops through angles between 0 and 180 degrees, performing the turbulence calculations each time
	#pragma omp parallel for private(outputs_day, outputs_night, angle, angle_degrees, satellite_distance, time) shared(system_params, params_day, params_night)
	for(int i = 0; i <= iterations; ++i)
	{
		// Determines the elevation angle to investigate for the current iteration
		angle_degrees = (double)i / (double)data_points;
		angle = angle_degrees * std::numbers::pi / 180.0;

		// Calculate the distance from Alice to the satellite
		satellite_distance = calculate_satellite_distance(angle, system_params.satellite_altitude); 

		// Calculates the time taken for the satellite to reach the current angle from the horizon
		time = (system_params.satellite_altitude + 6.371e+6) * (std::asin(satellite_distance * std::cos(angle) / (system_params.satellite_altitude + 6.371e+6)) + time_offset) / system_params.satellite_velocity;

		// Calculates the total transmissivity of the channel, the PLOB bound, and the secret key rate during the day and at night
		outputs_day = calculate_system_parameters(params_day, system_params, angle, satellite_distance);
		outputs_night = calculate_system_parameters(params_night, system_params, angle, satellite_distance);

		// Outputs the calculated values to a file
		#pragma omp critical
		{
			dataFile << angle_degrees << " " << time << " " << satellite_distance << " " << outputs_day.transmissivity << " " << outputs_night.transmissivity << " " << outputs_day.PLOB_bound << " " << outputs_night.PLOB_bound << " " << outputs_day.SKR << " " << outputs_night.SKR << std::endl;
		}
	}

	// Ends the program
	return 0;
}
