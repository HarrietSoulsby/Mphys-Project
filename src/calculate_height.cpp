// Imports neccesary functions
#include <cmath>

// Begins the function
double calculate_height(const double distance, const double angle){

	// Calculates the position along the path in Cartesian coordinates centred on the Earth's core
	double earth_x_distance = distance * std::cos(angle);
	double earth_y_distance = 6371000.0 + (distance * std::sin(angle));

	// Uses Pythagoras' theorem to find the distance to the centre of the Earth, then subtracts the radius of the Earth to obtain the height above sea level, returns the answer
	return std::sqrt( (earth_x_distance * earth_x_distance) * (earth_y_distance * earth_y_distance) ) - 6371000.0;
}
