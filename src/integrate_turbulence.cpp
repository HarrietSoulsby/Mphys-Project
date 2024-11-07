// Imports the neccesary libraries
#include <cmath>
#include <gsl/gsl_integration.h>
#include "header.hpp"
#include <iostream>

// Creates a struct to store the parameters used in integration
struct Params{
	int D;
	int R;
	double theta;
};

// Defines the model for calculating the refractive index structure constant
double calculate_Cn2(double x, double angle, int radius_earth, int satellite_distance){

	// Calls the function to calculate the height above sea level
	int H = calculate_height(angle, radius_earth, satellite_distance);
	// Defines the value of the refractive index structure constant at sea level
	double Cn0 = 1e-13;
	// Initialises the variable for storing the answer
	double Cn2;

	// Calculates the refractive index structure constant and returns the value
	Cn2 = Cn0 * std::exp(-x/H);
	return Cn2;
}

// Defines the form of the integrand
double integrand(double x, void *p){

	// Defines the variables being used, extracting the constants from the struct
	double output;
	Params *params = (Params *)p;
	int D = params->D;
	int R = params->R;
	float theta = params->theta;

	// Performs the calculations that define the integrand and returns the result
	double temp = 1 - (x / D);
	output = temp * temp * calculate_Cn2(x, theta, R, D);
	return output;
}

// Defines the main function for integrating over the turbulence
double integrate_turbulence(double angle, int radius_earth, int satellite_distance){

	// Defines the variables to be used in the integration
	double integrated_turbulence, error;
	Params params = {satellite_distance, radius_earth, angle};
	
	// Sets up the integration workspace
	gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(10000);
	gsl_function F;

	// Defines the equation and parameters to be used in the integration
	F.function = &integrand;
	F.params = &params;

	// Integrates the function
	gsl_integration_qags(&F, 0, satellite_distance, 0, 1e-8, 10000, workspace, &integrated_turbulence, &error);
	gsl_integration_workspace_free(workspace);

	// Divides the result by the neccesary prefactor
	integrated_turbulence /= satellite_distance;

	// Returns the result and ends the function
	return integrated_turbulence;
}
