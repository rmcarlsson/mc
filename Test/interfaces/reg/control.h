/*
 * control.h
 *
 *  Created on: 12 dec 2015
 *      Author: carltmik
 */

#ifndef INTERFACES_REG_CONTROL_H_
#define INTERFACES_REG_CONTROL_H_

#include "temp.h"


typedef enum
{
  CONTROL_STABLE,
  CONTROL_HEATING
} control_state_t;

void
control_init();

int
control_exec();

control_state_t
control_get_state();


void
control_set_target(temperature_t t);



#endif /* INTERFACES_REG_CONTROL_H_ */
