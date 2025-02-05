// Imports the neccesary libraries
#include "header.hpp"
#include <cmath>
#include <gsl/gsl_sf_bessel.h>
#include <iostream>

// Begins the function
double calculate_skr(const TurbulenceParameters atmosphere_params, const SystemParameters system_params, const double angle, const double satellite_distance){

	// Calculates the turbulence accumulated over the path
	double turbulence = integrate_turbulence(angle, satellite_distance, atmosphere_params);

	// Calculates the transmissivity, beam widening, and beam wander due to diffraction
	DiffractionParameters diffraction = calculate_diffraction(turbulence, satellite_distance, system_params);

	// Calculates the total transmissivity for the laser
	double transmissivity = 0.4 * diffraction.transmissivity * calculate_extinction(angle, satellite_distance);

	// Calculates the short term far field approximation for the diffraction transmissivity
	double n_st_far = (2.0*system_params.aperture_laser*system_params.aperture_laser)/(diffraction.beam_widening);

	// Calculates functions of the modified Bessel functions, used to calculate the SKR
	double f_0 = 1.0 / (1.0 - (std::exp(-2.0 * n_st_far) * gsl_sf_bessel_I0(2.0 * n_st_far)));
	double f_1 = std::exp(-2.0*n_st_far) * gsl_sf_bessel_I1(2.0 * n_st_far);

	std::cout << transmissivity << " " << n_st_far << " " << f_0 << " " << f_1 << std::endl;

	// Calculates geometry dependent positive parameters, used to calculate the SKR
	double y = (4.0*n_st_far*f_0*f_1) / (std::log(2.0*diffraction.transmissivity*f_0));
	double r_0 = system_params.aperture_laser / (std::pow(std::log(2.0*diffraction.transmissivity*f_0), 1.0/y));

	// Calculates the correction factor to apply in the calculation of the SKR to account for random beam wander
	double delta_n = calculate_delta_n(transmissivity, diffraction.beam_wander, y, r_0);

	// Returns the secret key rate and ends the function
	return -1.0 * std::log2(1.0 - transmissivity) * (1.0 + ((transmissivity * delta_n) / (std::log(1.0 - transmissivity))));
}
