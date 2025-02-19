# Satellite Uplink CVQKD Parameter Calculation Program

This program was created for my masters project "Ground To Space Satellite Optical Quantum Communications". By specififying a setup for a satellite uplink CVQKD channel in the input file, the program will calculate important parameters related to CVQKD, such as the PLOB bound and the secret key rate. Contact hs1956@york.ac.uk for any questions.

## Installation

Install the following dependencies: g++ (https://gcc.gnu.org/), and the GNU Scientific Library (https://www.gnu.org/software/gsl/)

Simply clone the repository, open the terminal in that directory, and run 'make'. This will generate an executable which can be run using './exe'.

## Configuration

The default configuration in the file 'input_parameters' should be physically realistic, however this should be modified according to the needs of the user. Do not input physically unrealistic values or very large numbers of data points or the program may fail due to overflows, underflows, and/or the failure of integrals to converge.

| Input parameter | Description | Units |
|----------|----------|----------|
| aperture_laser   | The aperture of the laser's receiver   | m   |
| wavelength_laser   | The wavelength of the laser   | m   |
| inner_scale_size   | The size of the smallest turbulent eddies in the free space channel   | m   |
| satellite_altitude   | The height of the satellite above sea level   | m   |
| spot_size_laser   | The waist size of the laser after leaving the transmitter   | m   |
| pointing_error_laser   | The beam wander induced by errors in aiming the transmitter, squared   | m<sup>2</sup>   |
| detector_efficiency   | The proportion of photons registered by the detector   | Unitless   |
| setup_noise   | The photon noise intrinsic to the detector, (potentially) considered trusted   | Unitless   |
| spectral_filter   | The size of the wavelength range allowed by the filter   | m   |
| detection_time   | The time taken to make a single detection   | s   |
| fov_detector   | The field of view of the detector   | sr   |
| daytime_Cn2_0   | The refractive index structure constant at ground level during daytime   | m<sup>-2/3</sup>   |
| daytime_windspeed   | The high altitude wind speed near the transmitter during daytime   | m/s   |
| daytime_albedo_parameter   | Dimensionless parameter that depends on the geometry and albedos of the Earth and Moon, during the day   | Unitless   |
| daytime_spectral_irradiance   | Describes the number of photons from the Sun at the relevant wavelength   | photons m<sup>-2</sup> s<sup>-1</sup> nm<sup>-1</sup> sr<sup>-1</sup>   |
| nighttime_Cn2_0   | The refractive index structure constant at ground level during nighttime   | m<sup>-2/3</sup>   |
| nighttime_windspeed   | The high altitude wind speed near the transmitter during nighttime   | m/s   |
| nighttime_albedo_parameter   | Dimensionless parameter that depends on the geometry and albedos of the Earth and Moon, during the day   | Unitless   |
| nighttime_spectral_irradiance   | Describes the number of photons from the Moon at the relevant wavelength   | photons m<sup>-2</sup> s<sup>-1</sup> nm<sup>-1</sup> sr<sup>-1</sup>   |
| data_points   | The number of data points per degree to calculate   | Unitless   |

## Outputs

After running the program, a file named 'output_data' is generated, containing the following tab separated collumns:

| Elevation Angle (degrees) | Satellite Distance (m) | Daytime Transmissivity (Unitless) | Nighttime Transmissivity (Unitless) | Daytime PLOB Bound (bits/use) | Nighttime PLOB Bound (bits/use) | Daytime SKR (bits/use) | Nighttime SKR (bits/use) |
|----------|----------|----------|----------|----------|----------|----------|----------|
| ...   | ...   | ...   | ...   | ...   | ...   | ...   | ...   |
| ...   | ...   | ...   | ...   | ...   | ...   | ...   | ...   |
| ...   | ...   | ...   | ...   | ...   | ...   | ...   | ...   |

These may be tabulated or plotted using software such as excel, matplotlib, or gnuplot.
