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
	double integrated_turbulence;
	int satellite_distance;

	// Initialises the variables to store the user defined parameters for the system
	int satellite_altitude;
	float diameter_laser, wavelength_laser, inner_scale_size;

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
	parameterFile.close();

	// Defines our constants for the system
	int radius_earth = 6371000;
	int radius_LEO = radius_earth + satellite_altitude;

	// Calculates the parameters for the laser
	float wavenumber_laser = (2 * std::numbers::pi) / wavelength_laser;
	float curvature_laser = diameter_laser / wavelength_laser;

	// Defines the angle parameters we will er
	double angle, angle_degrees;

	// Sets up a data file to write our outputs into
	std::ofstream dataFile;
	dataFile.open("output_data");
	dataFile << std::fixed << std::setprecision(20);

	// Loops through angles more than 30 degrees above the horizon, performing the turbulence simulation each time
	#pragma omp parallel for private(angle, angle_degrees, beam_spread, beam_intensity, integrated_turbulence, satellite_distance)
	for(int i = 3000; i <= 15000; i+=1){

		// Converts the angle to radians
		angle_degrees = i / 100.0;
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
