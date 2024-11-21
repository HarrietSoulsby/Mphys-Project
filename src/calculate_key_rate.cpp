// Import neccesary functions
#include <cmath>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_integration.h>

// Creates a struct to hold the integration parameters
struct Params{
	double r_0;
	double sigma2;
	double y;
	double n;
};

// Defines the integrand that will be passed to the integration function
// 	x: integration variable
// 	p: pointer to structure holding integration parameters
double key_integrand(double x, void *p){

	// Extracts the integration parameters from the structure
	Params *params = (Params *)p;
	double r_0 = params->r_0;
	double sigma2 = params->sigma2;
	double y = params->y;
	double n = params->n;

	// Outputs the value of the integrand at x
	return std::exp(-1.0 * ((r_0*r_0)/(2.0*sigma2)) * std::pow(x, 2.0/y)) / (std::exp(x) - n);
}

// Begin defining the function to calculate the key rate, accounting for beam wander
// 	n_st: short timescale diffraction transmissibility
// 	n_st_far: short timescale diffraction transmissibility, far field approximation
// 	n: total transmissibility
// 	a_R: laser aperture
// 	sigma2: measure of beam wander and widening
double calculate_key_rate(double n_st, double n_st_far, double n, float a_R, double sigma2){

	// Defines variables to hold the calculated parameters
	// 	K: key rate
	// 	y and r_0: geometry dependent positive parameters
	// 	f_0 and f_1: functions in terms of the Bessel functions used to define y and r_0
	// 	delta_n: a correction factor to the key rate to account for beam wander
	// 	error: used in integration to ensure convergence, see https://www.gnu.org/software/gsl/doc/html/integration.html
	double K, y, r_0, f_0, f_1, delta_n, error;

	// Calculates f_0 and f_1
	f_0 = 1.0 / (1.0 - std::exp(-2.0 * n_st_far) * gsl_sf_bessel_In(0,2.0*n_st_far));
	f_1 = std::exp(-2.0*n_st_far) * gsl_sf_bessel_In(1,2.0*n_st_far);

	// Calculates y and r_0
	y = (4.0*n_st_far*f_0*f_1) / (std::log(2.0*n_st*f_0));
	r_0 = (a_R) / (std::pow(std::log(2.0*n_st*f_0),1.0/y));

	// Passes the integration parameters into the structure, such that they may be passed into the integral together
	Params params = {r_0, sigma2, y, n};

	// Defines the workspace, function, and parameters for the integral
	gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(1000);
	gsl_function F;
	F.function = &key_integrand;
	F.params = &params;
	
	// Numerically integrates the integrand function
	int status = gsl_integration_qagiu(&F, 0.0, 1e-8, 1e-8, 1000, workspace, &delta_n, &error);

	// Calculates the key rate
	K = -1.0 * std::log2(1.0 - n) * (1.0 + ((n*delta_n) / (std::log(1.0 - n))));

	// Returns the key rate and ends the function
	return K;
}
