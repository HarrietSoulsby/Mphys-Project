// Imports neccesary libraries
#include "header.hpp"
#include <numbers>
#include <cmath>

// Begins the function
DiffractionParameters calculate_diffraction(const double turbulence, const double satellite_distance, const SystemParameters system_params)
{
	// Calculates the Rayleigh range
	double z_R = std::numbers::pi * system_params.spot_size_laser * system_params.spot_size_laser / system_params.wavelength_laser;

	// Calculates the radius of curvature of the laser
	double radius_curvature = satellite_distance * (1.0 + ( (z_R * z_R) / (satellite_distance * satellite_distance) ) );

	// Calculates the coherence length of the laser
	double coherence_length = std::pow(1.46 * system_params.wavenumber_laser * system_params.wavenumber_laser * satellite_distance * turbulence, -3.0/5.0);

	// Defines a variable to simplify the subsequent calculation
	double temp = 1.0 - (satellite_distance / radius_curvature);

	// Calculates the short term average spot size squared of the laser, w_{st}^2, at the satellite (see eq39 in Fante 1975)
	double beam_widening = ( (4.0 * satellite_distance * satellite_distance) / (system_params.wavenumber_laser * system_params.wavenumber_laser * system_params.spot_size_laser * system_params.spot_size_laser) ) + ( system_params.spot_size_laser * system_params.spot_size_laser * temp * temp / 4.0 ) + ( 4.0 * satellite_distance * satellite_distance * std::pow(std::abs(1.0 - (0.62 * std::cbrt(coherence_length / system_params.spot_size_laser))), 6.0/5.0) / (system_params.wavenumber_laser * system_params.wavenumber_laser * coherence_length * coherence_length) );

	// Calculates the beam wander, sigma^2 (see eq40 in Fante 1975)
	double beam_wander = (4.3362 * satellite_distance * satellite_distance * satellite_distance * turbulence) / (std::cbrt(system_params.spot_size_laser)) + (system_params.pointing_error_laser * satellite_distance * satellite_distance);

	// Calculates the diffraction induced transmissivity
	double transmissivity = 1.0 - std::exp(-2.0 * system_params.aperture_laser * system_params.aperture_laser / beam_widening);

	// Returns the outputs and ends the function
	return {transmissivity, beam_widening, beam_wander};
}
