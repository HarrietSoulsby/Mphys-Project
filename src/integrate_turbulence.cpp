// Imports the neccesary libraries
#include <cmath>
#include <gsl/gsl_integration.h>
#include "header.hpp"

// Defines a struct to store the parameters used in integration
struct IntegrationParameters
{
	double satellite_distance;
	double angle;
	double wind_speed;
	double Cn2_0;
};

// Defines the integrand function
double turbulence_integrand(double x, void *p)
{
	// Extracts the integration parameters from the struct
	IntegrationParameters *int_params = (IntegrationParameters *)p;
	double satellite_distance = int_params->satellite_distance;
	double angle = int_params->angle;
	double wind_speed = int_params->wind_speed;
	double Cn2_0 = int_params->Cn2_0;

	// Calculates the integrand
	double temp = 1.0 - (x / satellite_distance);
	return std::pow(temp, 5.0/3.0) * calculate_Cn2(x, angle, satellite_distance, wind_speed, Cn2_0);
}

// Begins the function
double integrate_turbulence(const double angle, const double satellite_distance, const TurbulenceParameters atmosphere_params)
{
	// Defines the variables to store the integration result
	double integrated_turbulence, error;

	// Creates the struct containing the variables which need to be passed into the integral
	IntegrationParameters int_params = {satellite_distance, angle, atmosphere_params.wind_speed, atmosphere_params.Cn2_0};

	// Creates the integration worksspace
	gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(50000);
	gsl_function F;

	// Defines the integrand and parameters to be used in the integration
	F.function = &turbulence_integrand;
	F.params = &int_params;

	// Numerically integrates the function
	gsl_integration_qags(&F, 1.0, satellite_distance, 1e-14, 1e-14, 50000, workspace, &integrated_turbulence, &error);
	gsl_integration_workspace_free(workspace);

	// Returns the integrated turbulence divided by a prefactor
	return integrated_turbulence / satellite_distance;
}
