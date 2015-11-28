/*
 * syst_model.c
 *
 *  Created on: 26 nov 2015
 *      Author: rmcar
 */


#include <math.h>


#define AMBIENT_TEMPERATURE (20.0)

/* a = e^(–T/RC)
 * T is sampling period
 * R is thermal resistance
 * C is Specific heat
 * */


double y = AMBIENT_TEMPERATURE;

const double weight_g = 8000.0; /* Unit [g] */
const double specific_heat = 4.1855; /* Unit [J/g*K] - Water */
const double tR_hob_kettle = 1.0; /* Unit [K/W] */
const double T = 1;

#define RC (tR_hob_kettle * specific_heat * weight_g)
#define POWER (-T/RC)

void
model_exec(double energy) {

	y = (exp(POWER) * y) + ((1 - exp(POWER)) * energy);
}

float get_model_temp()
{
	return (float)(y);
}
