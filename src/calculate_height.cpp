// Imports neccesary functions
#include <cmath>

// Begin defining the function
int calculate_height(float angle, int radius_earth, int distance){

	// Define the variables that will be used in the calculation
	int height, earth_x, earth_y, earth_distance;

	// Converts a position in polar coordinates in Alice's frame into cartesian coordinates in the Earth's core's frame
	earth_x = distance * std::cos(angle);
	earth_y = radius_earth + (distance * std::sin(angle));

	// Finds the distance of the position from Earth's centre
	earth_distance = std::sqrt((earth_x*earth_x) + (earth_y*earth_y));

	// Subtracts Earth's radius from the position to obtain height above sea level
	height = earth_distance - earth_radius;

	// Returns the height above see level and ends the function
	return height;
}
