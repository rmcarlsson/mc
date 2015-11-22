/*
 * pid.h
 *
 *  Created on: 19 nov 2015
 *      Author: rmcar
 */

#include <stdint.h>
#include "global_types.h"

#ifndef REG_INCLUDE_PID_H_
#define REG_INCLUDE_PID_H_



int16_t pid_controller(int16_t reference, int16_t process_value);
void pid_reset_integrator();
void pid_init(int16_t p_factor, int16_t i_factor, int16_t d_factor);

temperature_t get_temp();

#endif /* REG_INCLUDE_PID_H_ */
