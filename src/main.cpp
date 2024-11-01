// Import neccessary libraries
#include "header.hpp"
#include <iostream>
#include <cmath>

// Defines main
int main(){

	// Defines the parameters we will calculate
	float satellite_distance, beam_spread, beam_intensity;

	// Defines our constants for the system
	int radius_earth = 6371000;
	int radius_LEO = radius_earth + 800000;

	// Takes the angle of the satellite wrt the laser as a user input
	float angle;
	std::cout << "Enter the inclination angle of the satellite: ";
	std::cin >> angle;

	// Calculate the effects of turbulence on the laser
	satellite_distance = calculate_satellite_distance(angle, radius_earth, radius_LEO); 
	beam_spread = calculate_beam_spread(angle, radius_earth, satellite_distance);
	beam_intensity = calculate_beam_intensity(angle, radius_earth, satellite_distance);

	// Output the calculated results
	std::cout << "Beam spread: " << beam_spread << std::endl;
	std::cout << "Beam intensity: " << beam_intensity << std::endl;

	// End the program
	return 0;
}
