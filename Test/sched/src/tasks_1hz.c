/*
 * tasks_1hz.c
 *
 *  Created on: 19 nov 2015
 *      Author: rmcar
 */
#include <stdint.h>

#include "pid.h"

void exec_one_hz()
{
  float temp = get_temp();
  int16_t ret = pid_controller(temp, 10);
  set_power_level((ret*100)/INT16_MAX);
  exec_heater();
}

