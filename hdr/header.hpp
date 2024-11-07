#ifndef FUNC_H_
#define FUNC_H_
double calculate_satellite_distance(double angle, double radius_earth, double radius_LEO);
double integrate_turbulence(double angle, double radius_earth, double satellite_distance);
double calculate_height(double angle, double radius_earth, double distance);
float calculate_beam_spread(double satellite_distance, double integrated_turbulence, double diameter_laser, double wavenumber_laser, double curvature_laser, float inner_scale_size);
float calculate_beam_intensity(double satellite_distance, double integrated_turbulence, double diameter_laser, double wavenumber_laser, double curvature_laser, float inner_scale_size);
#endif
