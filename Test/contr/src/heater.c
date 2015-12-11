/*
 * heater.c
 *
 *  Created on: 22 nov 2015
 *      Author: rmcar
 */

#include <assert.h>
#include <stdio.h>

#include "global_types.h"
#include "gpio.h"
#if defined(SIMULATED)
#include "sys_model.h"
#endif

/*
 * Private date
 */
static power_t _curr_power;
static power_t _power_max;

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
exec_heater ()
{
  static int repeat = 0;

  gpio_write_val(repeat % 2);
  repeat++;

}

