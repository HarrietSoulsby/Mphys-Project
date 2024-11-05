#include <cmath>
#include <gsl/gsl_integration_h>

float integrand(double x, void *params){

	int D = *(int *)params;
	float temp = 1 - (x / D)

	return temp*temp*Cn2(x);
}

float integrate_turbulence(float angle, int radius_earth, int satellite_distance){

	float integrated_turbulence, distance, error;
	
	gsl_integration_workplace *workplace = gsl_integration_workplace_alloc(1000);
	gsl_function F;

	F.function = &integrand;
	F.params = &satellite_distance;

	gsl_integration_qags(&F, 0, satellite_distance, 0, 1e-6, 1000, workspace, &integrated_distance, &error);
	gsl_integration_workspace_free(workspace);

	integrated_turbulence /= satellite_distance;

	return integrated_turbulence;
}
