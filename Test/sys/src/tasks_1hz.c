/*
 * tasks_1hz.c
 *
 *  Created on: 19 nov 2015
 *      Author: rmcar
 */
#include <stdint.h>

void exec_one_hz()
{
  int16_t ret = pid_controller(10, 10);
}

