/*
 * heater.c
 *
 *  Created on: 22 nov 2015
 *      Author: rmcar
 */

#include <assert.h>

#include "global_types.h"

/*
 * Private date
 */
static power_t _curr_power;
static power_t _power_max;

#define POUT 24 /* P1-18 */
#define OUT 1

void
init_power (percent_t max_power)
{
  _power_max = max_power;

  /*
   * Export digital pin for "slow-PWM"
   */
  if (-1 == GPIOExport (POUT))
    assert (0);

  /*
   * Set GPIO directions
   */
  if (-1 == GPIODirection (POUT, OUT))
    assert (0);
}

void
set_power_level (percent_t power_percent)
{
  printf("Setting power to %d %\n", power_percent);
  if (power_percent > MAX_PERCENT)
    _curr_power = _power_max;

  else if (power_percent == MIN_PERCENT)
    _curr_power = 0;

  else
    _curr_power = (power_t) ((int32_t) (_power_max * MAX_PERCENT )
	/ (int32_t) (power_percent)) / MAX_PERCENT;

}

void
exec_heater ()
{
  static int repeat;

  if (-1 == GPIOWrite(POUT, repeat % 2))
    assert(0);

}

