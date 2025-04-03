// Imports the neccesart libraries
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
	double d;
	double Q_l;
	double wavenumber;
};

// Defines the integrand function
double integrand(double eta, void* params)
{
	// Extracts the variables and constants from the struct
	auto* p = static_cast<std::pair<double, IntegrationParameters*>*>(params);
	double epsilon = p->first;
	IntegrationParameters* const_params = p->second;

	// Calculates the propagation distance corresponding to the current value of epsilon
	double propagation_distance = epsilon * const_params->satellite_distance;

	// Calculates the Rytov variance
	double rytov_variance = 1.23 * std::pow(const_params->wavenumber, 7.0/6.0) * std::pow(const_params->satellite_distance, 11.0/6.0) * calculate_Cn2(propagation_distance, const_params->angle, const_params->satellite_distance, const_params->wind_speed, const_params->Cn2_0);

	// Calculates the quantity eta_x
	double eta_x = 2.61 / (1 + (0.45 * rytov_variance * std::pow(const_params->Q_l, 1.0/6.0)));

	// Returns the integrand function
	return 1.06 * rytov_variance * std::pow(eta, -11.0/6.0) * std::exp(-(eta/const_params->Q_l) - (eta/eta_x) - (eta * const_params->d * const_params->d / 4.0)) * (1.0 + (1.802 * std::sqrt(eta / const_params->Q_l)) - (0.254 * std::pow(eta/const_params->Q_l, 7.0/12.0))) * (1.0 - std::cos(eta * epsilon));
}

// Defines the function to perform the inner part of the integral (the integral over eta)
double integrate_inner(double epsilon, IntegrationParameters* const_params)
{
	// Sets up the integral
	gsl_integration_workspace* w = gsl_integration_workspace_alloc(10000);
	gsl_function F;
	std::pair<double, IntegrationParameters*> param_wrapper(epsilon, const_params);
	F.function = &integrand;
	F.params = &param_wrapper;

	// Performs the numerical integration
	double result, error;
	gsl_integration_qawo_table* qawo_table = gsl_integration_qawo_table_alloc(epsilon, 0.000001, GSL_INTEG_COSINE, 10000);
	gsl_integration_qawo(&F, 0.0, 1e-2, 1e-2, 10000, w, qawo_table, &result, &error);
	gsl_integration_qawo_table_free(qawo_table);

	// Frees up the workspace and returns the result of the integration
	gsl_integration_workspace_free(w);
	return result;
}

// Defines the function to perform the outer part of the integral (the integral over epsilon)
double integrate_outer(IntegrationParameters* const_params)
{
	// Sets up the integral
	gsl_integration_workspace* w = gsl_integration_workspace_alloc(50000);
	gsl_function F;
	F.function = [](double epsilon, void* params) -> double {
		return integrate_inner(epsilon, static_cast<IntegrationParameters*>(params));
	};
	F.params = const_params;

	// Performs the numerical integration
	double result, error;
	gsl_integration_qag(&F, 0.0, 1.0, 1e-7, 1e-7, 50000, GSL_INTEG_GAUSS61, w, &result, &error);

	// Frees up the workspace and returns the result of the integration
	gsl_integration_workspace_free(w);
	return result;
}

// Defines the function to calculate the scintillation
double calculate_scintillation(const double angle, const double satellite_distance, const SystemParameters system_params)
{
	// Calculates two quantities used in the integrand, d and Q_l, which do not depend on either integration variable
	double d = std::sqrt(system_params.wavenumber_laser * system_params.aperture_laser * system_params.aperture_laser / (4.0 * satellite_distance));
	double Q_l = 10.89 * satellite_distance / (system_params.wavenumber_laser * system_params.inner_scale_size * system_params.inner_scale_size);

	// Places the constants used in the integral into a struct
	IntegrationParameters params = {satellite_distance, angle, system_params.wind_speed, system_params.Cn2_0, d, Q_l, system_params.wavenumber_laser};

	// Returns the result of the integration
	return integrate_outer(&params);
}
