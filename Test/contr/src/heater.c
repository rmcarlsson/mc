/*
 * heater.c
 *
 *  Created on: 22 nov 2015
 *      Author: rmcar
 */

#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "global_types.h"
#include "gpio.h"
#if defined(SIMULATED)
#include "sys_model.h"
#endif

/*
 * Private date
 */
power_t _curr_power;
power_t _power_max;

#define POUT 23 /* P1-18 */
#define OUT 1

void
init_heater (percent_t max_power)
{
  _power_max = max_power;
#if defined(SIMULATED)
  init_model();
#endif

}

void
set_power_level (percent_t power_percent)
{
  static power_t last_power = 0;

  if (power_percent > MAX_PERCENT)
    _curr_power = _power_max;

  else if (power_percent == MIN_PERCENT)
    _curr_power = 0;

  else
    _curr_power = (power_t) (int32_t) ((_power_max * power_percent) / MAX_PERCENT);

  if (last_power != _curr_power )
    {
    printf("Setting power is %d W\n", _curr_power);

  last_power = _curr_power;
    }
#if defined(SIMULATED)
#define T (1)
  exec_model(_curr_power * T);
#endif /* defined(SIMULATED) */

}

void
heater_exec ()
{
  struct timespec sp;
  struct timespec rem;


  if (_curr_power > 0)
    gpio_write_val(1);

  if (_curr_power < 95)
    {
      long ms = (5000 * _curr_power)/MAX_PERCENT;
      sp.tv_nsec = (ms % 1000) * 1000;
      sp.tv_sec = ms/1000;
      printf("s = %lu, ns = %lu\n", sp.tv_sec, sp.tv_nsec);
      nanosleep(&sp, &rem);
      gpio_write_val(0);
    }




}

