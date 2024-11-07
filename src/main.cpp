// Import neccessary libraries
#include "header.hpp"
#include <iostream>
#include <cmath>
#include <numbers>
#include <fstream>
#include <iomanip>

// Defines main
int main(){

	// Defines the parameters we will calculate
	float beam_spread, beam_intensity;
	double integrated_turbulence;
	int satellite_distance;

	// Defines our constants for the system
	int radius_earth = 6371000;
	int radius_LEO = radius_earth + 800000;

	// Defines the parameters for the laser and atmosphere
	float diameter_laser = 0.1;
	float wavelength_laser = 1550e-9;
	float wavenumber_laser = (2 * std::numbers::pi) / wavelength_laser;
	float curvature_laser = diameter_laser / wavelength_laser;
	float inner_scale_size = 0.005;

	// Defines the angle parameters we will iterate over
	double angle, angle_degrees;

	// Sets up a data file to write our outputs into
	std::ofstream dataFile("data");
	dataFile << std::fixed << std::setprecision(20);

	// Loops through angles more than 30 degrees above the horizon, performing the turbulence simulation each time
	for(double i = 30; i <= 150; i+=0.01){

		// Converts the angle to radians
		angle_degrees = i;
		angle = angle_degrees * std::numbers::pi / 180.0;

		// Calculate the effects of turbulence on the laser
		satellite_distance = calculate_satellite_distance(angle, radius_earth, radius_LEO); 
		integrated_turbulence = integrate_turbulence(angle, radius_earth, satellite_distance);
		beam_spread = calculate_beam_spread(satellite_distance, integrated_turbulence, diameter_laser, wavenumber_laser, curvature_laser, inner_scale_size);
		beam_intensity = calculate_beam_intensity(satellite_distance, integrated_turbulence, diameter_laser, wavenumber_laser, curvature_laser, inner_scale_size);

		// Outputs the calculated parameters to the datafile, along with the current angle in degrees
		dataFile << angle_degrees << " " << beam_spread << " " << beam_intensity << std::endl;

	}

	// Closes the datafile
	dataFile.close();

	// End the program
	return 0;
}
