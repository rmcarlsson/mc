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
#include "mash_profile.h"
#include "data_logger.h"





void init()
{


  read_profile();

  init_heater(100);
  control_init();
  gpio_init();
  mash_init();
  da_init();

}

void exec_one_hz()
{

  //static int sched_cnt = 0;
  temp_exec();
  //if ((sched_cnt % 5) == 0)
    {
    int p = control_exec();
    set_power_level(p);
    }

 // sched_cnt++;
}

