/*
 * init.c
 *
 *  Created on: 19 nov 2015
 *      Author: rmcar
 */

#include "pid.h"
#include "sys_model.h"



void init()
{
  pid_init(100, 0, 0);
#if !defined(SIMULATED)
  init_power(1500);
#endif
}

