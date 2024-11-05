// Import neccessary libraries
#include "header.hpp"
#include <iostream>
#include <cmath>

// Defines main
int main(){

	// Defines the parameters we will calculate
	float beam_spread, beam_intensity, integrated_turbulence;
	int satellite_distance;

	// Defines our constants for the system
	int radius_earth = 6371000;
	int radius_LEO = radius_earth + 800000;

	// Takes the angle of the satellite wrt the laser as a user input
	float angle;
	std::cout << "Enter the inclination angle of the satellite: ";
	std::cin >> angle;

	// Calculate the effects of turbulence on the laser
	satellite_distance = calculate_satellite_distance(angle, radius_earth, radius_LEO); 
	integrated_turbulence = integrate_turbulence(angle, radius_earth, satellite_distance);
	beam_spread = calculate_beam_spread(satellite_distance, integrated_turbulence);
	beam_intensity = calculate_beam_intensity(satellite_distance, integrated_turbulence);

	// Output the calculated results
	std::cout << "Beam spread: " << beam_spread << std::endl;
	std::cout << "Beam intensity: " << beam_intensity << std::endl;

	// End the program
	return 0;
}
