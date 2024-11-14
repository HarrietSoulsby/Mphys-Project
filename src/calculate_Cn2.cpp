// Imports the neccesary libraries
#include <cmath>
#include "header.hpp"

// Defines the model for calculating the refractive index structure constant
double calculate_Cn2(double x, double angle, double radius_earth, double satellite_distance, double wind_speed, double Cn2_0){

	// Calls the function to calculate the height above sea level
	double H = calculate_height(angle, radius_earth, x);

	// Initialises the variable for storing the answer
	double temp, Cn2;

	// Calculates the refractive index structure constant and returns the value
	temp = wind_speed / 27;
	Cn2 = (5.94*(1e-53)*temp*temp*std::pow(H,10.0)*std::exp(-H/1000)) + (2.7*(1e-16)*std::exp(-H/1500)) + (Cn2_0*std::exp(-H/100));
	return Cn2;
}
