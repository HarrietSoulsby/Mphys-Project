// Imports the neccesary libraries
#include "header.hpp"
#include <cmath>
#include <gsl/gsl_integration.h>

// Defines a struct for holding the parameters used in integration
struct IntegrationParameters
{
	double transmissivity;
	double beam_wander;
	double y;
	double r_0;
};

// Defines the integrand function
double delta_n_integrand(double x, void *p)
{
	// Extracts the integration parameters from the struct
	IntegrationParameters *int_params = (IntegrationParameters *)p;
	double transmissivity = int_params->transmissivity;
	double beam_wander = int_params->beam_wander;
	double y = int_params->y;
	double r_0 = int_params->r_0;

	// Returns the value of the integrand at x
	return std::exp((-r_0 * r_0 * std::pow(x, 2.0 / y) / (2.0 * beam_wander)) - x) / (1.0 - (std::exp(-x) * transmissivity));
}

// Begins the function
double calculate_delta_n(const double transmissivity, const double beam_wander, const double y, const double r_0)
{
	// Initialises the variable for storing the integration output
	double delta_n, error;

	// Places the parameters used in the integration into a struct
	IntegrationParameters int_params = {transmissivity, beam_wander, y, r_0};

	// Sets up the integration workspace, function, and parameters
	gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(50000);
	gsl_function F;
	F.function = &delta_n_integrand;
	F.params = &int_params;
	
	// Numerically integrates to find delta_n
	int status = gsl_integration_qagiu(&F, 0.0, 1e-14, 1e-14, 50000, workspace, &delta_n, &error);

	// Returns delta_n and ends the function
	return delta_n;
}
