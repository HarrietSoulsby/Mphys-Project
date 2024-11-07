#ifndef FUNC_H_
#define FUNC_H_
int calculate_satellite_distance(double angle, int radius_earth, int radius_LEO);
double integrate_turbulence(double angle, int radius_earth, int satellite_distance);
int calculate_height(double angle, int radius_earth, int distance);
float calculate_beam_spread(int satellite_distance, double integrated_turbulence, float diameter_laser, float wavenumber_laser, float curvature_laser, float inner_scale_size);
float calculate_beam_intensity(int satellite_distance, double integrated_turbulence, float diameter_laser, float wavenumber_laser, float curvature_laser, float inner_scale_size);
#endif
