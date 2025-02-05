// Header guard
#ifndef FUNC_H_
#define FUNC_H_

// Structures

// Stores the constants related to the system which will be used in the calculations
struct SystemParameters{ 
	double aperture_laser;
	double wavelength_laser;
	double wavenumber_laser;
	double spot_size_laser;
	double curvature_laser;
	double radius_LEO;
	double inner_scale_size;
};

// Stores the constants characterising the turbulence of the system, which will vary between daytime and nighttime
struct TurbulenceParameters{ 
	double Cn2_0;
	double wind_speed;
};

struct DiffractionParameters{
	double transmissivity;
	double beam_widening;
	double beam_wander;
};

// Functions
double calculate_skr(const TurbulenceParameters atmosphere_day, const SystemParameters params, const double angle, const double satellite_distance);

double calculate_satellite_distance(const double angle, const double radius_LEO);

double integrate_turbulence(const double angle, const double satellite_distance, const TurbulenceParameters atmosphere_params);

double calculate_Cn2(const double distance, const double angle, const double satellite_distance, const double wind_speed, const double Cn2_0);

double calculate_height(const double distance, const double angle);

double calculate_extinction(const double angle, const double satellite_distance);

double calculate_delta_n(const double transmissivity, const double beam_wander, const double y, const double r_0);

DiffractionParameters calculate_diffraction(const double turbulence, const double satellite_distance, const SystemParameters system_params);

// End header guard
#endif
