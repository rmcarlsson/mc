/*
 * tasks_1hz.c
 *
 *  Created on: 19 nov 2015
 *      Author: rmcar
 */
#include <stdint.h>


#include "gpio.h"
#include "temp.h"
#include "control.h"
#include "heater.h"





void init()
{


  init_heater(100);
  control_init();
  gpio_init();

}

void exec_one_hz()
{

  int p = control_exec();
  set_power_level(p);
}

