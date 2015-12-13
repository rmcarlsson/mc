/*
 * heater.c
 *
 *  Created on: 22 nov 2015
 *      Author: rmcar
 */

#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

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
    _curr_power = (power_t) (int32_t) ((_power_max * power_percent)
	/ MAX_PERCENT );

  if (_curr_power != last_power)
    {
      printf ("Setting power %u\n", _curr_power);
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
  struct timespec rem =
    { .tv_nsec = 0, .tv_sec = 0 };


  struct timeval tm1, tm2;
  gettimeofday(&tm1, NULL);


  if (_curr_power > 0)

    gpio_write_val (1);

  if (_curr_power < 95)
    {
      long ms = (5000 * _curr_power) / MAX_PERCENT;
      sp.tv_nsec = (ms % 1000) * 1000000L;
      sp.tv_sec = ms / 1000;
      nanosleep (&sp, &rem);
      if (rem.tv_nsec != 0 || rem.tv_sec != 0)
	{
	  printf ("Time left in delay, nsec = %lu, sec = %lu\n", rem.tv_nsec,
		  rem.tv_sec);
	}
      gpio_write_val (0);
      gettimeofday(&tm2, NULL);
      unsigned long long t = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
     // printf("%llu ms\n", t);

    }



}

