// Imports the neccesary libraries
#include <cmath>
#include <gsl/gsl_integration.h>
#include "header.hpp"
#include <iostream>

// Creates a struct to store the parameters used in integration
struct Params{
	double D;
	double R;
	double theta;
};

// Defines the model for calculating the refractive index structure constant
double calculate_Cn2(double x, double angle, double radius_earth, double satellite_distance){

	// Calls the function to calculate the height above sea level
	double H = calculate_height(angle, radius_earth, x);
	// Defines the value of the refractive index structure constant at sea level
	double Cn0 = 1.5e-14;
	// Initialises the variable for storing the answer
	double temp1, temp2, Cn2;

	// Calculates the refractive index structure constant and returns the value
	temp1 = H/1000000.0;
	temp2 = (288.0 - (0.0065*H*0.001)) / 288.0;
	Cn2 = (Cn0 / (temp1*temp1*std::sqrt(temp1))) / (temp2*std::sqrt(temp2));
	std::cout << x << " " << Cn2 << std::endl;
	return Cn2;
}

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
