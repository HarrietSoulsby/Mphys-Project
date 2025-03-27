// Imports the neccesary libraries
#include "header.hpp"
#include <cmath>

// Begins the function
OutputParameters calculate_system_parameters(const TimeofdayParameters time_params, const SystemParameters system_params, const double angle, const double satellite_distance)
{
	// Calculates the turbulence accumulated over the path
	double turbulence = integrate_turbulence(angle, satellite_distance, time_params);

	// Calculates the transmissivity, beam widening, and beam wander due to diffraction
	DiffractionParameters diffraction = calculate_diffraction(turbulence, satellite_distance, system_params);

	// Calculates the total transmissivity for the laser
	double transmissivity = system_params.detector_efficiency * diffraction.transmissivity * calculate_extinction(angle, system_params);

	// Calculates the PLOB bound
	double PLOB_bound = calculate_PLOB(transmissivity, diffraction, system_params);

	// Calculates the secret key rate
	double SKR = calculate_skr(transmissivity, PLOB_bound, system_params, time_params);

	// Returns the transmissivity (in db), PLOB bound, and secret key rate
	return {10.0 * std::log10(transmissivity), PLOB_bound, SKR};
}
