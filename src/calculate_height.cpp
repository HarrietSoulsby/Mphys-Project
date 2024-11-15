// Imports neccesary functions
#include <cmath>

// Begin defining the function
double calculate_height(double angle, double radius_earth, double distance){

	// Define the variables that will be used in the calculation
	double height, earth_x, earth_y, earth_distance;

	// Converts a position in polar coordinates in Alice's frame into cartesian coordinates in the Earth's core's frame
	earth_x = distance * std::cos(angle);
	earth_y = radius_earth + (distance * std::sin(angle));

	// Finds the distance of the position from Earth's centre
	earth_distance = std::sqrt((earth_x*earth_x) + (earth_y*earth_y));

	// Subtracts Earth's radius from the position to obtain height above sea level
	height = earth_distance - radius_earth;

	// Returns the height above see level and ends the function
	return height;
}
