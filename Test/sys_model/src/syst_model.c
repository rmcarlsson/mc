/*
 * syst_model.c
 *
 *  Created on: 26 nov 2015
 *      Author: rmcar
 */

#include <math.h>
#include <time.h>
#include <float.h>
#include <stdlib.h>
#include <stdbool.h>

#include "global_types.h"

#define AMBIENT_TEMPERATURE (20.0)

/* a = e^(–T/RC)
 * T is sampling period
 * R is thermal resistance
 * C is Specific heat
 * */

double y = 43;

const double weight_g = 8000.0; /* Unit [g] */
const double specific_heat = 4.1855; /* Unit [J/g*K] - Water */
const double tR_hob_kettle = 1.0; /* Unit [K/W] */
double tR_kettle_amb; /* Unit [K/W] */
const double T = 1;

#define RC (tR_hob_kettle * specific_heat * weight_g)
#define POWER (-T/RC)


double
generate_noise ()
{

  static bool stiring = false;
  static int stiring_time = false;

#define STIR_INTERVAL (160) /* Unit [s] */
#define STIR_TIME (10) /* Unit [s] */

  if (stiring)
    {
      stiring_time++;
      if (stiring_time >= STIR_TIME)
	{
	  stiring = false;
	}

      /* <########## RETURN ##########> */
      printf("Someone ist stiring, dropping %f.3 C\n", (stiring_time / 2));
      return stiring_time / 4;
    }

  if ((rand () % STIR_INTERVAL) == 0)
    {;
      stiring = true;
      stiring_time = 0;
    }

  return 0;

}

void
init_model ()
{
  double temp_drop = 1; /* Unit [K] */
  double t_drop = 60 * 3; /* Unit [s], like 3 minutes, sortof */
  time_t t;

  /* Powerloss at 70 degrees C */
  double p = (specific_heat * weight_g * temp_drop) / t_drop;
  tR_kettle_amb = (70 - AMBIENT_TEMPERATURE) / p;

  srand ((unsigned) time (&t));

}

void
exec_model (double energy)
{
  static double last_y = 0;


  y = (exp (POWER) * y) + ((1 - exp (POWER)) * energy);

  /* Reduce energy due to ambient temperature	 */
  //double p_heatloss = (y - AMBIENT_TEMPERATURE) / tR_kettle_amb;
 // y = y - (((p_heatloss * T) / (specific_heat)) / weight_g);

  /* Add misc. noise */
 // y = y - generate_noise();

  if (fabs (last_y - y) > 1.0)
    {
      printf ("Temp (model) is %.3f\n", y);
      //printf("Temp (model) is %.3f, heatloss %.3f causing %.3f dropp.\n", y, p_heatloss, (((p_heatloss * T) / (specific_heat)) / weight_g));
      last_y = y;
    }

  model_analytics();


}

static double y_max = DBL_MIN;
static double y_min = DBL_MAX;

void
model_analytics ()
{
  static int16_t iv = 0;
#define IV_PERIOD (100000)
  static bool tune = true;
  static int max_ch = 0;
  static int min_ch = 0;

  if (y > y_max)
    {
      y_max = y;
      max_ch++;
    }
  if (y < y_min)
    {
      y_min = y;
      min_ch++;
    }

  if (( iv++ > IV_PERIOD) && tune)
    {
      if (max_ch > 10)
	tune = false;
      pid_set_p (1);
      iv = 0;
      max_ch = 0;
      min_ch = 0;
      y_max = DBL_MIN;
      y_min = DBL_MAX;
    }

}

temperature_t
get_temp_model ()
{

  return (float) (y);


}



