/*
 * heater.h
 *
 *  Created on: 12 dec 2015
 *      Author: carltmik
 */

#include "global_types.h"

#ifndef INTERFACES_REG_HEATER_H_
#define INTERFACES_REG_HEATER_H_

void
init_heater (percent_t max_power);

void
set_power_level (percent_t power_percent);

void
heater_exec ();

#endif /* INTERFACES_REG_HEATER_H_ */
