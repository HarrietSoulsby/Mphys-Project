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

	// Takes the angle of the satellite wrt the laser as a user input
	double angle, angle_degrees;

	std::ofstream dataFile("data");
	dataFile << std::fixed << std::setprecision(20);

	for(double i = 30; i <= 150; i+=0.01){

		angle_degrees = i;
		angle = angle_degrees * std::numbers::pi / 180.0;

		// Calculate the effects of turbulence on the laser
		satellite_distance = calculate_satellite_distance(angle, radius_earth, radius_LEO); 
		integrated_turbulence = integrate_turbulence(angle, radius_earth, satellite_distance);
//		beam_spread = calculate_beam_spread(satellite_distance, integrated_turbulence);
//		beam_intensity = calculate_beam_intensity(satellite_distance, integrated_turbulence);
//
		dataFile << angle_degrees << " " << integrated_turbulence << std::endl;
//
//		// Output the calculated results
		std::cout << "Angle: " << angle_degrees << " degrees" << std::endl;
//		std::cout << "Beam spread: " << beam_spread << std::endl;
//		std::cout << "Beam intensity: " << beam_intensity << std::endl;
		std::cout << "C_n^2: " << integrated_turbulence << "\n" << std::endl;

	}

	dataFile.close();

	// End the program
	return 0;
}
