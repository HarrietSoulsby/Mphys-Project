// Imports neccesary libraries
#include <cmath>
#include <numbers>

// Begin defining the function
double calculate_satellite_distance(double angle, double radius_earth, double radius_LEO){

	// Defines the variables used in the calculation
	double satellite_distance, earth_frame_distance, alice_x, alice_y, earth_frame_angle;

	// Calculates the polar coordinates of the satellite in the frame with origin at the centre of the Earth
	earth_frame_distance = radius_earth + radius_LEO;
	earth_frame_angle = angle + std::asin((radius_earth * std::sin((std::numbers::pi/2) + angle)) / (radius_earth + radius_LEO));

	// Transforms the coordinates of the satellite to cartesians in the reference frame of Alice on Earth's surface
	alice_x = earth_frame_distance * std::cos(earth_frame_angle);
	alice_y = (earth_frame_distance * std::sin(earth_frame_angle)) - radius_earth;

	// Uses Pythagoras' theorem to find the distance from Alice to the satellite
	satellite_distance = std::sqrt((alice_x*alice_x) + (alice_y*alice_y));

	// Outputs the distance between Alice and the satellite
	return satellite_distance;
}
