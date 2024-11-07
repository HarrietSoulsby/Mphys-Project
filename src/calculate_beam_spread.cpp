#include <cmath>

float calculate_beam_spread(double satellite_distance, double integrated_turbulence, double diameter_laser, double wavenumber_laser, double curvature_laser, float inner_scale_size){

	float beam_spread;

	beam_spread = ((4*satellite_distance*satellite_distance)/(wavenumber_laser*wavenumber_laser*diameter_laser*diameter_laser)) + (diameter_laser*diameter_laser/4) + (6.6*satellite_distance*satellite_distance*satellite_distance*integrated_turbulence/std::pow(inner_scale_size, 1/3));

	beam_spread = std::sqrt(beam_spread);

	return beam_spread;
}
