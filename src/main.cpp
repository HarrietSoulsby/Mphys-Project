// Import neccessary libraries
#include "header.hpp"
#include <iostream>
#include <cmath>
#include <numbers>
#include <fstream>
#include <iomanip>
#include <omp.h>

// Begins the program
int main()
{
	// Defines the variables which contain the user specified parameters of the system
	SystemParameters system_params;
	double satellite_altitude;
	double data_points;
	TurbulenceParameters atmosphere_day;
	TurbulenceParameters atmosphere_night;

	// Defines the variables related to the system's geometry which will change for each iteration
	double angle;
	double angle_degrees;
	double satellite_distance;

	// Defines the variables for the final values of the secret key rate
	double skr_day;
	double skr_night;

	// Sets up a stream for importing parameters from an input file
	std::ifstream parameterFile;
	parameterFile.open("input_parameters");

	//Imports the parameters from the file
	std::string dummy;
	std::getline(parameterFile, dummy);
	parameterFile >> dummy >> system_params.aperture_laser;
	parameterFile >> dummy >> system_params.wavelength_laser;
	parameterFile >> dummy >> system_params.inner_scale_size;
	parameterFile >> dummy >> satellite_altitude;
	parameterFile >> dummy >> data_points;
	parameterFile >> dummy >> system_params.spot_size_laser;
	parameterFile >> dummy >> atmosphere_day.Cn2_0;
	parameterFile >> dummy >> atmosphere_day.wind_speed;
	parameterFile >> dummy >> atmosphere_night.Cn2_0;
	parameterFile >> dummy >> atmosphere_night.wind_speed;
	parameterFile.close();

	// Defines the distance of the satellite from the Earth's core
	system_params.radius_LEO = 6371000.0 + satellite_altitude;

	// Calculates the wavenumber of the laser
	system_params.wavenumber_laser = (2.0 * std::numbers::pi) / system_params.wavelength_laser;

	// Sets up a data file to write the final values into
	std::ofstream dataFile;
	dataFile.open("output_data", std::ios::out | std::ios::trunc);
	dataFile << std::fixed << std::setprecision(20);

	// Calculates the number of iterations to perform based on the user specified required number of data points
	int iterations = 180*data_points;

	// Loops through angles between 0 and 180 degrees, performing the turbulence calculations each time
	#pragma omp parallel for private(skr_day, skr_night, angle, angle_degrees, satellite_distance)
	for(int i = 0; i <= iterations; ++i)
	{
		// Determines the elevation angle to investigate for the current iteration
		angle_degrees = (double)i / (double)data_points;
		angle = angle_degrees * std::numbers::pi / 180.0;

		// Calculate the distance from Alice to the satellite
		satellite_distance = calculate_satellite_distance(angle, system_params.radius_LEO); 

		// Calculates the secret key rate during the day and at night
		skr_day = calculate_skr(atmosphere_day, system_params, angle, satellite_distance);
		skr_night = calculate_skr(atmosphere_night, system_params, angle, satellite_distance);

		// Outputs the calculated secret key rates to a file
		#pragma omp critical
		{
			dataFile << angle_degrees << " " << skr_day << " " << skr_night << " " << satellite_distance << std::endl;
		}
	}

	// Ends the program
	return 0;
}
