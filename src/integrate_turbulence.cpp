// Imports the neccesary libraries
#include <cmath>
#include <gsl/gsl_integration.h>
#include "header.hpp"
#include "calculate_Cn2.cpp"

// Creates a struct to store the parameters used in integration
struct Params{
	double D;
	double R;
	double theta;
};

// Defines the form of the integrand
double integrand(double x, void *p){

	// Defines the variables being used, extracting the constants from the struct
	double output;
	Params *params = (Params *)p;
	double D = params->D;
	double R = params->R;
	double theta = params->theta;

	// Performs the calculations that define the integrand and returns the result
	double temp = 1 - (x / D);
	output = temp * temp * calculate_Cn2(x, theta, R, D);
	return output;
}

// Defines the main function for integrating over the turbulence
double integrate_turbulence(double angle, double radius_earth, double satellite_distance){

	// Defines the variables to be used in the integration
	double integrated_turbulence, error;
	Params params = {satellite_distance, radius_earth, angle};
	
	// Sets up the integration workspace
	gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(1000000);
	gsl_function F;

	// Defines the equation and parameters to be used in the integration
	F.function = &integrand;
	F.params = &params;

	// Integrates the function
	gsl_integration_qag(&F, 1, satellite_distance, 1e-4, 1e-4, 1000000, 6, workspace, &integrated_turbulence, &error);
	gsl_integration_workspace_free(workspace);

	// Divides the result by the neccesary prefactor
	integrated_turbulence /= satellite_distance;

	// Returns the result and ends the function
	return integrated_turbulence;
}
