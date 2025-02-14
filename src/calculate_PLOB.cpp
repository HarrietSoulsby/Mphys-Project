// Imports neccesary libraries
#include "header.hpp"
#include <gsl/gsl_sf_bessel.h>
#include <cmath>

// Begins the function
double calculate_PLOB(const double transmissivity, const DiffractionParameters diffraction, const SystemParameters system_params)
{
	// Calculates the short term far field approximation for the diffraction transmissivity
	double n_st_far = (2.0*system_params.aperture_laser*system_params.aperture_laser)/(diffraction.beam_widening);

	// Calculates functions of the modified Bessel functions, used to calculate the PLOB bound
	double f_0 = 1.0 / (1.0 - (std::exp(-2.0 * n_st_far) * gsl_sf_bessel_I0(2.0 * n_st_far)));
	double f_1 = std::exp(-2.0*n_st_far) * gsl_sf_bessel_I1(2.0 * n_st_far);

	// Calculates geometry dependent positive parameters, used to calculate the PLOB bound
	double y = (4.0*n_st_far*f_0*f_1) / (std::log(2.0*diffraction.transmissivity*f_0));
	double r_0 = system_params.aperture_laser / (std::pow(std::log(2.0*diffraction.transmissivity*f_0), 1.0/y));

	// Returns the PLOB bound and ends the function
	return -std::log2(1.0 - transmissivity) * (1.0 + ((transmissivity * calculate_delta_n(transmissivity, diffraction.beam_wander, y, r_0)) / (std::log(1.0 - transmissivity))));
}
