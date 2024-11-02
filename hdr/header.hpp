#ifndef FUNC_H_
#define FUNC_H_
int calculate_satellite_distance(float angle, int radius_earth, int radius_LEO);
int calculate_height(float angle, int radius_earth, int distance);
float calculate_beam_spread(float angle, int radius_earth, int satellite_distance);
float calculate_beam_intensity(float angle, int radius_earth, int satellite_distance);
#endif
