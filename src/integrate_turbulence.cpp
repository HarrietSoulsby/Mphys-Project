#include <cmath>
#include <gsl/gsl_integration.h>
#include "header.hpp"
#include <iostream>

struct Params{
	int D;
	int R;
	double theta;
};

double calculate_Cn2(double x, double angle, int radius_earth, int satellite_distance){

	int H = calculate_height(angle, radius_earth, satellite_distance);
	double Cn0 = 1e-13;
	double Cn2;

	Cn2 = Cn0 * std::exp(-x/H);

	return Cn2;
}

double integrand(double x, void *p){

	double output;
	Params *params = (Params *)p;
	int D = params->D;
	int R = params->R;
	float theta = params->theta;

	double temp = 1 - (x / D);
	output = temp * temp * calculate_Cn2(x, theta, R, D);
	return output;
}

double integrate_turbulence(double angle, int radius_earth, int satellite_distance){

	double integrated_turbulence, error;
	Params params = {satellite_distance, radius_earth, angle};
	
	gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(10000);
	gsl_function F;

	F.function = &integrand;
	F.params = &params;

	gsl_integration_qags(&F, 0, satellite_distance, 0, 1e-8, 10000, workspace, &integrated_turbulence, &error);
	gsl_integration_workspace_free(workspace);

	integrated_turbulence /= satellite_distance;

	return (1 / integrated_turbulence);
}
