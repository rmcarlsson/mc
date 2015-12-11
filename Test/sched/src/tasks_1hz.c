/*
 * tasks_1hz.c
 *
 *  Created on: 19 nov 2015
 *      Author: rmcar
 */
#include <stdint.h>

#include "pid.h"
#include "pid_atune.h"
#include "gpio.h"
#include "temp.h"



double i = 20;
double o = 80;
double setpoint=24;
int tuning = 0;

void init()
{

  double kp=2,ki=0.5,kd=2;

  double kpmodel=1.5, taup=100, theta[50];
  double outputStart=5;
  double aTuneStep=50, aTuneNoise=1, aTuneStartValue=100;
  unsigned int aTuneLookBack=500;

  PID_ATune(&i, &o);
  PID(&i, &o, &setpoint, kp,ki,kd, DIRECT);
  SetMode(AUTOMATIC);

  SetNoiseBand(aTuneNoise);
  SetOutputStep(aTuneStep);
  SetLookbackSec((int)aTuneLookBack);

  gpio_init();

}

void exec_one_hz()
{
  i = get_temp();

  if (tuning == 0)
  {
      tuning = Runtime();
      if (tuning != 0) {
	  SetTunings(pidat_GetKp(), pidat_GetKi(), pidat_GetKd());
      }
  }
  else
  { //we're done, set the tuning parameters
      Compute();
  }

  int set = 0;
  if (o<0)
    set = 0;
  else
    set = (uint16_t)(o);
  set_power_level(set);
}

