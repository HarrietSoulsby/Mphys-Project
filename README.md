# Satellite Uplink CVQKD Parameter Calculation Program

This program was created for my masters project "Ground To Space Satellite Optical Quantum Communications". By specififying a setup for a satellite uplink CVQKD channel in the input file, the program will calculate important parameters related to CVQKD, such as the PLOB bound and the secret key rate.

## Installation

Simply clone the repository, open the terminal in that directory, and run 'make'. This will generate an executable which can be run using './exe'.

## Configuration

The default configuration in the file 'input_parameters' should be physically realistic, however this should be modified according to the needs of the user. Do not input physically unrealistic values or very large numbers of data points or the program may fail due to overflows, underflows, and/or the failure of integrals to converge.

| Input parameter | Description | Units |
|----------|----------|----------|
| aperture_laser   | Data 2   | m   |
| wavelength_laser   | Data 5   | m   |
| inner_scale_size   | Data 8   | m   |
| satellite_altitude   | Data 8   | m   |
| spot_size_laser   | Data 8   | m   |
| pointing_error_laser   | Data 8   | Data 9   |
| detector_efficiency   | Data 8   | Unitless   |
| setup_noise   | Data 8   | Data 9   |
| spectral_filter   | Data 8   | m   |
| detection_time   | Data 8   | s   |
| fov_detector   | Data 8   | Data 9   |
| daytime_Cn2_0   | Data 8   | Data 9   |
| daytime_windspeed   | Data 8   | m/s   |
| daytime_albedo_parameter   | Data 8   | Data 9   |
| daytime_spectral_irradiance   | Data 8   | Data 9   |
| nighttime_Cn2_0   | Data 8   | Data 9   |
| nighttime_windspeed   | Data 8   | m/s   |
| nighttime_albedo_parameter   | Data 8   | Data 9   |
| nighttime_spectral_irradiance   | Data 8   | Data 9   |
| data_points   | Data 8   | Unitless   |

## Outputs

After running the program, a file named 'output_data' is generated, containing the following tab separated collumns:

| Elevation Angle (degrees) | Satellite Distance (m) | Daytime Transmissivity | Nighttime Transmissivity | Daytime PLOB Bound () | Nighttime PLOB Bound () | Daytime SKR () | Nighttime SKR () |
|----------|----------|----------|----------|----------|----------|----------|----------|
| ...   | ...   | ...   | ...   | ...   | ...   | ...   | ...   |
| ...   | ...   | ...   | ...   | ...   | ...   | ...   | ...   |
| ...   | ...   | ...   | ...   | ...   | ...   | ...   | ...   |
