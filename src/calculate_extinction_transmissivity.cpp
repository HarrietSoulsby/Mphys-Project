// Imports the neccesary libraries
#include <cmath>
#include <gsl/gsl_integration.h>
#include "header.hpp"

// Creates a struct to store the parameters used in integration
struct Params{
	double R;
	double theta;
};

// Defines the form of the integrand
double extinction_integrand(double x, void *p){

	// Defines the variables being used, extracting the constants from the struct
	double output;
	Params *params = (Params *)p;
	double R = params->R;
	double theta = params->theta;

	// Performs the calculations that define the integrand and returns the result
	output = std::exp(-1.0 * calculate_height(theta, R, x)/6600.0);
	return output;
}

// Defines the main function for integrating over the distance
double calculate_extinction_transmissivity(double angle, double radius_earth, double satellite_distance){

	// Defines the variables to be used in the integration
	double transmissivity, error;
	Params params = {radius_earth, angle};
	
	// Sets up the integration workspace
	gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(1000000);
	gsl_function F;

	// Defines the equation and parameters to be used in the integration
	F.function = &extinction_integrand;
	F.params = &params;

	// Integrates the function
	gsl_integration_qag(&F, 1, satellite_distance, 1e-4, 1e-4, 1000000, 6, workspace, &transmissivity, &error);
	gsl_integration_workspace_free(workspace);

	// Calculates the final result
	transmissivity = std::exp((-5e-6)*transmissivity);

	// Returns the result and ends the function
	return transmissivity;
}
