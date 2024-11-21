// Imports neccesary functions
#include <cmath>
#include <numbers>
#include "header.hpp"

// Begins defining the function
diffraction_parameters calculate_diffraction_transmissivity(double satellite_distance, double wavelength_laser, double spot_size_laser, double diameter_laser, double integrated_turbulence){

	// Initialises the variables to store the final result, the Rayleigh range, and the enlarged spot size
	double transmissivity, z_R, w_d, w_st2, temp1, temp2, sigma2;

	// Calculates the Rayleigh range
	z_R = std::numbers::pi * spot_size_laser * spot_size_laser / wavelength_laser;

	// Calculates the enlarged spot size due to diffraction
	w_d = spot_size_laser * std::sqrt(1.0 + ((satellite_distance*satellite_distance)/(z_R*z_R)));

	// Calculates the short term average spot size squared due to diffraction and turbulence
	temp1 = (wavelength_laser*wavelength_laser*wavelength_laser) / (4.0 * std::numbers::pi * std::numbers::pi * std::numbers::pi * 1.46 * integrated_turbulence);
	temp2 = 1 - (0.33 * std::pow((1.46*integrated_turbulence*4*std::numbers::pi*std::numbers::pi) / (spot_size_laser*wavelength_laser*wavelength_laser),1.0/3.0));
	w_st2 = (w_d*w_d) + (2.0*temp1*temp1*temp2*temp2);
	
	// Calculates the beam wander
	sigma2 = 2.0*temp1*temp1*(1.0 - (temp2*temp2));

	// Calculates the diffraction induced transmissivity
	transmissivity = 1.0 - std::exp(-2.0 * diameter_laser * diameter_laser / w_st2);

	// Ends the function, returning the result
	return {transmissivity, w_st2, sigma2};
}

