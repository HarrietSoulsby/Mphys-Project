// Imports neccesary functions
#include <cmath>
#include <numbers>

// Begins defining the function
double calculate_diffraction_transmissivity(double satellite_distance, double wavelength_laser, double spot_size_laser, double diameter_laser){

	// Initialises the variables to store the final result, the Rayleigh range, and the enlarged spot size
	double transmissivity, z_R, w_d;

	// Calculates the Rayleigh range
	z_R = std::numbers::pi * spot_size_laser * spot_size_laser / wavelength_laser;

	// Calculates the enlarged spot size
	w_d = spot_size_laser * std::sqrt(1.0 + ((satellite_distance*satellite_distance)/(z_R*z_R)));

	// Calculates the diffraction induced transmissivity
	transmissivity = 1.0 - std::exp(-2.0 * diameter_laser * diameter_laser / (w_d*w_d));

	// Ends the function, returning the result
	return transmissivity;
}

