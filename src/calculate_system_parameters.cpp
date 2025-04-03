// Imports the neccesary libraries
#include "header.hpp"
#include <cmath>

// Begins the function
OutputParameters calculate_system_parameters(const SystemParameters system_params, const double angle, const double satellite_distance)
{
	// Calculates the turbulence accumulated over the path
	double turbulence = integrate_turbulence(angle, satellite_distance, system_params);

	// Calculates the transmissivity, beam widening, and beam wander due to diffraction
	DiffractionParameters diffraction = calculate_diffraction(turbulence, satellite_distance, system_params);

	// Calculates the atmospheric extinction
	double extinction = calculate_extinction(angle, system_params);

	// Calculates the total transmissivity for the laser
	double transmissivity = system_params.detector_efficiency * diffraction.transmissivity * extinction;

	// Calculates the PLOB bound
	double PLOB_bound = calculate_PLOB(transmissivity, diffraction, system_params);

	// Calculates the irradiance flux variance due to scintillation
	double scintillation = calculate_scintillation(angle, satellite_distance, system_params);

	// Returns the transmissivity (in db), PLOB bound, and secret key rate
	return {10.0 * std::log10(transmissivity), PLOB_bound, scintillation, diffraction.transmissivity, diffraction.beam_widening, diffraction.beam_wander, std::pow(1.46 * system_params.wavenumber_laser * system_params.wavenumber_laser * satellite_distance * turbulence, -3.0/5.0), extinction};
}
