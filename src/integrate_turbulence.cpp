#include <cmath>
#include <gsl/gsl_integration.h>
#include "header.hpp"

struct Params{
	int D;
	int R;
	float theta;
};

float calculate_Cn2(float x, float angle, float radius_earth, float satellite_distance){

	int H = calculate_height(angle, radius_earth, satellite_distance);
	float Cn0 = 1e-13;
	float Cn2;

	Cn2 = Cn0 * std::exp(-x/H);

	return Cn2;
}

double integrand(double x, void *p){

	Params *params = (Params *)p;
	int D = params->D;
	int R = params->R;
	float theta = params->theta;

	double temp = 1 - (x / D);
	return temp * temp * calculate_Cn2(x, theta, R, D);
}

float integrate_turbulence(float angle, int radius_earth, int satellite_distance){

	double integrated_turbulence, error;
	Params params = {satellite_distance, radius_earth, angle};
	
	gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(1000);
	gsl_function F;

	F.function = &integrand;
	F.params = &params;

	gsl_integration_qags(&F, 0.0, satellite_distance, 0, 1e-4, 1000, workspace, &integrated_turbulence, &error);
	gsl_integration_workspace_free(workspace);

	integrated_turbulence /= satellite_distance;

	return integrated_turbulence;
}
