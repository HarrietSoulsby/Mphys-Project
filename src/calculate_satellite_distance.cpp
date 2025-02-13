// Imports neccesary libraries
#include <cmath>
#include <numbers>

// Begins the function
double calculate_satellite_distance(const double angle, const double radius_LEO)
{
	// Calculates the position of the satellite in polar coordinates centred on the Earth's core
	double earth_distance = 6371000.0 + radius_LEO;

	// Returns the distance from the ground station to the satellite
	return std::sqrt((radius_LEO * radius_LEO) + (2.0 * radius_LEO * 6371000.0) + (6371000.0 * 6371000.0 * std::sin(angle) * std::sin(angle))) - (6371000.0 * std::sin(angle));
}
