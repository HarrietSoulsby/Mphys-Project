// Imports neccesary libraries
#include "header.hpp"
#include <numbers>
#include <cmath>

// Begins the function
DiffractionParameters calculate_diffraction(const double turbulence, const double satellite_distance, const SystemParameters system_params){

	// Calculates the Rayleigh range
	double z_R = std::numbers::pi * system_params.spot_size_laser * system_params.spot_size_laser / system_params.wavelength_laser;

	// Calculates the enlarged spot size due to diffraction
	double w_d = system_params.spot_size_laser * std::sqrt(1.0 + ((satellite_distance * satellite_distance)/(z_R * z_R)));

	// Calculates some repeatedly used values to simplify and speedup subsequent calculations
	double temp1 = std::pow((1.46*turbulence*4.0*std::numbers::pi*std::numbers::pi) / (system_params.wavelength_laser*system_params.wavelength_laser), -3.0 / 5.0);
	double temp2 = (system_params.wavelength_laser*satellite_distance) / (std::numbers::pi * temp1);
	double temp3 = 1.0 - (0.33 * std::pow(temp1 / system_params.spot_size_laser, 1.0/3.0));

	// Calculates the short term average spot size squared, w_{st}^2
	double beam_widening = (w_d*w_d) + (2.0*temp2*temp2*temp3*temp3);

	// Calculates the beam wander, sigma^2
	double beam_wander = 2.0*temp2*temp2*(1.0 - (temp3*temp3));

	// Calculates the diffraction induced transmissivity
	double transmissivity = 1.0 - std::exp(-2.0 * system_params.aperture_laser * system_params.aperture_laser / beam_widening);

	// Returns the outputs and ends the function
	return {transmissivity, beam_widening, beam_wander};
}
