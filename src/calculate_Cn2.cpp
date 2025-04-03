// Imports the neccesary libraries
#include <cmath>
#include "header.hpp"

// Begins the function
double calculate_Cn2(const double distance, const double angle, const double satellite_distance, const double wind_speed, const double Cn2_0)
{
	// Calculates the height above sea level at the current distance along the path
	double height = calculate_height(distance, angle);

	// Calculates the refractive index structure constant using the SLC model
	if(height <= 18.5)
	{
		return 8.40e-15;
	}
	else if(height > 18.5 && height <= 110.0)
	{
		return 2.87e-12 / (height*height);
	}
	else if(height > 110.0 && height <= 1500.0)
	{
		return 2.5e-16;
	}
	else if(height > 1500.0 && height <= 7200.0)
	{
		return 8.87e-7 / (height*height*height);
	}
	else
	{
		return 2.00e-16 / std::sqrt(height);
	}

}
