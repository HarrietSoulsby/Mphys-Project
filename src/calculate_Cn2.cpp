// Imports the neccesary libraries
#include <cmath>
#include "header.hpp"

// Defines the model for calculating the refractive index structure constant
double calculate_Cn2(double x, double angle, double radius_earth, double satellite_distance){

	// Calls the function to calculate the height above sea level
	double H = calculate_height(angle, radius_earth, x);
	// Defines the value of the refractive index structure constant at sea level
	double Cn0 = 1.5e-15;
	// Initialises the variable for storing the answer
	double temp1, temp2, Cn2;

	// Calculates the refractive index structure constant and returns the value
	Cn2 = Cn0 * std::exp(-H/100);
	return Cn2;
}
