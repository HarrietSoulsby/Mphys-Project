// Header guard
#ifndef FUNC_H_
#define FUNC_H_

// Structures
struct diffraction_parameters{
	double transmissivity;
	double beam_spread;
	double beam_wander;
};

// Functions
double calculate_satellite_distance(double angle, double radius_earth, double radius_LEO);

double calculate_Cn2(double x, double angle, double radius_earth, double satellite_distance, double wind_speed, double Cn2_0);

double integrate_turbulence(double angle, double radius_earth, double satellite_distance, double wind_speed, double Cn2_0);

double calculate_height(double angle, double radius_earth, double distance);

double calculate_turbulence_transmissivity(double satellite_distance, double integrated_turbulence, double diameter_laser, double wavenumber_laser, double curvature_laser, float inner_scale_size);

diffraction_parameters calculate_diffraction_transmissivity(double satellite_distance, double wavelength_laser, double spot_size_laser, double diameter_laser, double integrated_turbulence);

double calculate_extinction_transmissivity(double angle, double radius_earth, double satellite_distance);

double calculate_key_rate(double n_st, double w_st2, double n, float a_R, double sigma2);

// End header guard
#endif
