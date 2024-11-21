// Import neccessary libraries
#include "header.hpp"
#include <iostream>
#include <cmath>
#include <numbers>
#include <fstream>
#include <iomanip>
#include <omp.h>

// Defines main
int main(){

	// Defines the parameters we will calculate
	double beam_intensity_night, beam_intensity_day, integrated_turbulence_night, integrated_turbulence_day, satellite_distance, diffraction_transmissivity_day, diffraction_transmissivity_night, extinction_transmissivity, total_transmissivity_day, total_transmissivity_night, key_rate_day, key_rate_night;

	// Initialises the variables to store the user defined parameters for the system
	double satellite_altitude, diameter_laser, wavelength_laser, spot_size_laser;
	float inner_scale_size;
	int data_points;

	// Initialises a structure to hold the results of the diffraction calculations
	diffraction_parameters diffraction_day;
	diffraction_parameters diffraction_night;

	// Sets up a stream for importing parameters from an input file
	std::ifstream parameterFile;
	parameterFile.open("input_parameters");

	//Imports the parameters from the file
	std::string dummy;
	std::getline(parameterFile, dummy);
	parameterFile >> dummy >> diameter_laser;
	parameterFile >> dummy >> wavelength_laser;
	parameterFile >> dummy >> inner_scale_size;
	parameterFile >> dummy >> satellite_altitude;
	parameterFile >> dummy >> data_points;
	parameterFile >> dummy >> spot_size_laser;
	parameterFile.close();

	// Defines our constants for the system
	double radius_earth = 6371000;
	double radius_LEO = radius_earth + satellite_altitude;

	// Calculates the parameters for the laser
	double wavenumber_laser = (2 * std::numbers::pi) / wavelength_laser;
	double curvature_laser = 100;

	// Defines the angle parameters we will er
	double angle, angle_degrees;

	// Sets up a data file to write our outputs into
	std::ofstream dataFile;
	dataFile.open("output_data", std::ios::out | std::ios::trunc);
	dataFile << std::fixed << std::setprecision(20);

	// Calculates number of iterations to perform
	int iterations = 180*data_points;

	// Loops through angles between 0 and 180 degrees, performing the turbulence simulation each time
	#pragma omp parallel for private(angle, angle_degrees, beam_intensity_night, beam_intensity_day, integrated_turbulence_night, integrated_turbulence_day, satellite_distance, diffraction_day, diffraction_night, extinction_transmissivity, total_transmissivity_day, total_transmissivity_night, key_rate_day, key_rate_night)
	for(int i = 0; i <= iterations; ++i){

		// Converts the angle to radians
		angle_degrees = (float)i / (float)data_points;
		angle = angle_degrees * std::numbers::pi / 180.0;

		// Calculate the effects of turbulence on the laser
		satellite_distance = calculate_satellite_distance(angle, radius_earth, radius_LEO); 
		integrated_turbulence_night = integrate_turbulence(angle, radius_earth, satellite_distance, 21, 1.7*(1e-14));
		integrated_turbulence_day = integrate_turbulence(angle, radius_earth, satellite_distance, 57, 2.75*(1e-14));
		beam_intensity_night = calculate_turbulence_transmissivity(satellite_distance, integrated_turbulence_night, diameter_laser, wavenumber_laser, curvature_laser, inner_scale_size);
		beam_intensity_day = calculate_turbulence_transmissivity(satellite_distance, integrated_turbulence_day, diameter_laser, wavenumber_laser, curvature_laser, inner_scale_size);
		diffraction_day = calculate_diffraction_transmissivity(satellite_distance, wavelength_laser, spot_size_laser, diameter_laser, integrated_turbulence_day);
		diffraction_night = calculate_diffraction_transmissivity(satellite_distance, wavelength_laser, spot_size_laser, diameter_laser, integrated_turbulence_night);
		extinction_transmissivity = calculate_extinction_transmissivity(angle, radius_earth, satellite_distance);

		// Calculates the total transmissivity	
		total_transmissivity_night = beam_intensity_night*diffraction_night.transmissivity*extinction_transmissivity;
		total_transmissivity_day = beam_intensity_day*diffraction_day.transmissivity*extinction_transmissivity;

		// Calculates the key rate
		key_rate_day = calculate_key_rate(diffraction_day.transmissivity, diffraction_day.beam_spread, total_transmissivity_day, diameter_laser, diffraction_day.beam_wander);
		key_rate_night = calculate_key_rate(diffraction_night.transmissivity, diffraction_day.beam_spread, total_transmissivity_night, diameter_laser, diffraction_night.beam_wander);

		// Outputs the calculated parameters to the datafile, along with the current angle in degrees
		#pragma omp critical
		{
			dataFile << angle_degrees << " " << key_rate_night << " " << key_rate_day << std::endl;
		}

	}

	// Closes the datafile
	dataFile.close();

	// End the program
	return 0;
}
