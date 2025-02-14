// Header guard
#ifndef FUNC_H_
#define FUNC_H_

// Structures

// Stores the constants related to the system which will be used in the calculations
struct SystemParameters
{ 
	double aperture_laser;
	double wavelength_laser;
	double wavenumber_laser;
	double spot_size_laser;
	double pointing_error_laser;
	double satellite_altitude;
	double inner_scale_size;
	double detector_efficiency;
	double setup_noise;
	double spectral_filter;
	double detection_time;
	double fov_detector;
};

struct OutputParameters
{
	double transmissivity;
	double PLOB_bound;
	double SKR;
};

// Stores the constants characterising the turbulence of the system, which will vary between daytime and nighttime
struct TimeofdayParameters
{ 
	double Cn2_0;
	double wind_speed;
	double albedo_parameter;
	double spectral_irradiance;
};

// Stores the calculated values of parameters determined by the turbulence
struct DiffractionParameters
{
	double transmissivity;
	double beam_widening;
	double beam_wander;
};

// Functions
OutputParameters calculate_system_parameters(const TimeofdayParameters time_params, const SystemParameters params, const double angle, const double satellite_distance);

double calculate_satellite_distance(const double angle, const double radius_LEO);

double integrate_turbulence(const double angle, const double satellite_distance, const TimeofdayParameters time_params);

double calculate_Cn2(const double distance, const double angle, const double satellite_distance, const double wind_speed, const double Cn2_0);

double calculate_height(const double distance, const double angle);

double calculate_extinction(const double angle, const double satellite_distance);

double calculate_delta_n(const double transmissivity, const double beam_wander, const double y, const double r_0);

DiffractionParameters calculate_diffraction(const double turbulence, const double satellite_distance, const SystemParameters system_params);

double calculate_PLOB(const double transmissivity, const DiffractionParameters diffraction, const SystemParameters system_params);

double calculate_skr(const double transmissivity, const double PLOB_bound, const SystemParameters system_params, TimeofdayParameters time_params);

// End header guard
#endif
