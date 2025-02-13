// Imports neccesary libraries
#include <cmath>
#include <numbers>

// Begins the function
double calculate_satellite_distance(const double angle, const double radius_LEO)
{
	// Calculates the position of the satellite in polar coordinates centred on the Earth's core
	double earth_frame_distance = 6371000.0 + radius_LEO;
	double earth_frame_angle = angle + std::asin( (6371000.0 * std::sin((std::numbers::pi/2.0) + angle)) / (6371000.0 + radius_LEO) );

	// Calculates the position of the satellite in Cartesian coordinates centred on the ground station (Alice)
	double alice_x_distance = earth_frame_distance * std::cos(earth_frame_angle);
	double alice_y_distance = (earth_frame_distance * std::sin(earth_frame_angle)) - (2.0*6371000.0);

	// Returns the distance from the ground station to the satellite (via Pythagoras' theorem)
	return std::sqrt((alice_x_distance*alice_x_distance) + (alice_y_distance*alice_y_distance));
}
