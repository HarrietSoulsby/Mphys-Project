// Imports neccesary libraries
#include "header.hpp"
#include <cmath>

// Begins the function
double calculate_skr(const double transmissivity, const double PLOB_bound, const SystemParameters system_params)
{

	// Calculates gamma_R (used in subsequent calculations)
	double gamma_R = system_params.spectral_filter * system_params.detection_time * system_params.fov_detector * system_params.aperture_laser * system_params.aperture_laser;

	// Calculates the background noise entering the detector's aperture
	double background_noise = system_params.albedo_parameter * system_params.spectral_irradiance * gamma_R;

	// Calculates the total noise recorded by the detector
	double detected_noise = (system_params.detector_efficiency * background_noise) + system_params.setup_noise;

	// Defines a temporary variable to simplify the following calculation
	double temp = detected_noise / (1.0 - transmissivity);

	// Calculates and returns the secret key rate
	return PLOB_bound - (((temp + 1.0) * std::log2(temp + 1.0)) - (temp * std::log2(temp)));
}
