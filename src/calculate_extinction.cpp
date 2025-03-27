// Imports the neccesary libraries
#include "header.hpp"
#include <cmath>
#include <numbers>

// Begins the function
double calculate_extinction(const double angle, const SystemParameters system_params)
{
	// Converts the wavelength from meters to micrometers
	double wavelength = system_params.wavelength_laser * 1.0e+6;
	double wavelength_cm = system_params.wavelength_laser * 1.0e+2;

	// Converts the CO2 concentration from being in terms of ppm to being in terms of parts per volume by percent and parts per volume
	double CO2_concentration_percent = system_params.CO2_concentration * 1e-4;
	double CO2_concentration_volume = system_params.CO2_concentration * 1e-6;

	// Calculates g (the acceleration due to gravity) at the mass weighted altitude of the air collumn and the specified latitude
	double gravity_acceleration = (980.6160 * (1.0 - (0.0026373 * std::cos(2.0 * system_params.latitude)) + (0.0000059 * std::cos(2.0 * system_params.latitude) * std::cos(2.0 * system_params.latitude)))) - (((3.085462e-4) + (2.27e-7 * std::cos(2.0 * system_params.latitude))) * 5517.56) + (((7.254e-11) + (1.0e-13 * std::cos(2.0 * system_params.latitude))) * 5517.56 * 5517.56) - (((1.517e-17) + (6e-20 * std::cos(2.0 * system_params.latitude))) * 5517.56 * 5517.56 * 5517.56);

	// Calculates the king factor for N2
	double king_factor_N2 = 1.034 + ((3.17e-4)/(wavelength * wavelength));

	// Calculates the king factor for O2
	double king_factor_O2 = 1.096 + ((1.385e-3)/(wavelength * wavelength)) + ((1.448e-4)/(wavelength * wavelength * wavelength * wavelength));

	// Calculates the king factor for the air, accounting for contributions from N2, O2, and CO2
	double king_factor_air = ((78.084 * king_factor_N2) + (20.946 * king_factor_O2) + (0.934) + (1.15 * CO2_concentration_percent)) / (99.964 + CO2_concentration_percent);

	// Calculates the refractive index of the air at the given wavelength and CO2 concentration
	double temp1 = ((8.06051e-5) + (2.480990e-2 / (132.274 - (1.0 / (wavelength * wavelength)))) + (1.74557e-4 / (39.32957 - (1.0 / (wavelength * wavelength))))) + 1.0;
	double refractive_index = temp1 + ((temp1 - 1.0) * ((0.54 * CO2_concentration_volume) - 0.000162));

	// Calculates the density of air molecules at the given temperature
	double molecular_density = 2.68676266e+19 * 273.15 / system_params.air_temperature;

	// Calculates the scattering cross section of the molecules in the atomsphere
	double temp2 = (refractive_index * refractive_index) - 1.0;
	double temp3 = (refractive_index * refractive_index) + 2.0;
	double scattering_cross_section = (24.0 * std::numbers::pi * std::numbers::pi * std::numbers::pi * temp2 * temp2 * king_factor_air) / (wavelength_cm * wavelength_cm * wavelength_cm * wavelength_cm * molecular_density * molecular_density * temp3 * temp3);

	// Calculates the mean molecular mass for the molecules in the atmosphere
	double mean_molecular_mass = (15.0556 * CO2_concentration_volume) + 28.9595;

	// Calculates the contribution to the optical depth from Rayleigh scattering
	double optical_depth = scattering_cross_section * system_params.air_pressure * 6.02214076e+26 / (mean_molecular_mass * gravity_acceleration);

	// Returns the transmissivity induced by atmospheric extinction and ends the function
	return 	std::exp(-optical_depth / std::sin(angle));
}
