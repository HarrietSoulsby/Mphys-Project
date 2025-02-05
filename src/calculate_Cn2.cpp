// Imports the neccesary libraries
#include <cmath>
#include "header.hpp"

// Begins the function
double calculate_Cn2(const double distance, const double angle, const double satellite_distance, const double wind_speed, const double Cn2_0){

	// Calculates the height above sea level at the current distance along the path
	double height = calculate_height(distance, angle);

	// Returns the refractive index structure constant
	double temp = wind_speed / 27.0;
	return (0.00594*temp*temp*std::pow(height/(1e+5),10)*std::exp(-height/1000.0)) + (2.7*(1e-16)*std::exp(-height/1500.0)) + (Cn2_0*std::pow(1.0/(height+1.0),4.0/3.0));
}
