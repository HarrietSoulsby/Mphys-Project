#ifndef FUNC_H_
#define FUNC_H_
int calculate_satellite_distance(float angle, int radius_earth, int radius_LEO);
float integrate_turbulence(float angle, int radius_earth, int satellite_distance);
int calculate_height(float angle, int radius_earth, int distance);
//float calculate_beam_spread(int satellite_distance, float integrated_turbulence);
//float calculate_beam_intensity(int satellite_distance, float integrated_turbulence);
#endif
