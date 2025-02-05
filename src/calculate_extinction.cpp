// Imports the neccesary libraries
#include "header.hpp"
#include <cmath>
#include <gsl/gsl_integration.h>

// Defines a struct to store the parameters used in integration
struct IntegrationParameters{
	double angle;
};

// Defines the integrand function
double extinction_integrand(double x, void *p){

	// Extracts the integration parameters from the struct
	IntegrationParameters *int_params = (IntegrationParameters *)p;
	double angle = int_params->angle;

	// Calculates the integrand
	return std::exp(-1.0 * calculate_height(x, angle)/6600.0);
}

// Begins the function
double calculate_extinction(const double angle, const double satellite_distance){

	// Defines the variables to store the integration result
	double transmissivity, error;	

	// Creates the struct containing the variables which need to be passed into the integral
	IntegrationParameters int_params = {angle};

	// Creates the integration worksspace
	gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(500000);
	gsl_function F;

	// Defines the integrand and parameters to be used in the integration
	F.function = &extinction_integrand;
	F.params = &int_params;

	// Integrates the function
	gsl_integration_qags(&F, 1, satellite_distance, 1e-4, 1e-4, 50000, workspace, &transmissivity, &error);
	gsl_integration_workspace_free(workspace);

	// Returns the result and ends the function
	return std::exp((-5e-6)*transmissivity);
}
