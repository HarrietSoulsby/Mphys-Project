// Import neccessary libraries
#include "header.hpp"
#include <iostream>
#include <cmath>
#include <numbers>
#include <fstream>
#include <iomanip>
#include <omp.h>

// Defines main
int main(){

	// Defines the parameters we will calculate
	float beam_spread, beam_intensity;
	double integrated_turbulence, satellite_distance;

	// Initialises the variables to store the user defined parameters for the system
	double satellite_altitude, diameter_laser, wavelength_laser;
	float inner_scale_size;
	int data_points;

	// Sets up a stream for importing parameters from an input file
	std::ifstream parameterFile;
	parameterFile.open("input_parameters");

	//Imports the parameters from the file
	std::string dummy;
	std::getline(parameterFile, dummy);
	parameterFile >> dummy >> diameter_laser;
	parameterFile >> dummy >> wavelength_laser;
	parameterFile >> dummy >> inner_scale_size;
	parameterFile >> dummy >> satellite_altitude;
	parameterFile >> dummy >> data_points;
	parameterFile.close();

	// Defines our constants for the system
	double radius_earth = 6371000;
	double radius_LEO = radius_earth + satellite_altitude;

	// Calculates the parameters for the laser
	double wavenumber_laser = (2 * std::numbers::pi) / wavelength_laser;
	double curvature_laser = 100;

	// Defines the angle parameters we will er
	double angle, angle_degrees;

	// Sets up a data file to write our outputs into
	std::ofstream dataFile;
	dataFile.open("output_data", std::ios::out | std::ios::trunc);
	dataFile << std::fixed << std::setprecision(20);

	// Calculates number of iterations to perform
	int iterations = 180*data_points;

	// Loops through angles more than 30 degrees above the horizon, performing the turbulence simulation each time
	#pragma omp parallel for private(angle, angle_degrees, beam_spread, beam_intensity, integrated_turbulence, satellite_distance)
	for(int i = 0; i <= iterations; ++i){

		// Converts the angle to radians
		angle_degrees = (float)i / (float)data_points;
		angle = angle_degrees * std::numbers::pi / 180.0;

		// Calculate the effects of turbulence on the laser
		satellite_distance = calculate_satellite_distance(angle, radius_earth, radius_LEO); 
		integrated_turbulence = integrate_turbulence(angle, radius_earth, satellite_distance);
		beam_spread = calculate_beam_spread(satellite_distance, integrated_turbulence, diameter_laser, wavenumber_laser, curvature_laser, inner_scale_size);
		beam_intensity = calculate_beam_intensity(satellite_distance, integrated_turbulence, diameter_laser, wavenumber_laser, curvature_laser, inner_scale_size);

		// Outputs the calculated parameters to the datafile, along with the current angle in degrees
		#pragma omp critical
		{
			dataFile << angle_degrees << " " << beam_spread << " " << beam_intensity << std::endl;
		}

	}

	// Closes the datafile
	dataFile.close();

	// End the program
	return 0;
}
