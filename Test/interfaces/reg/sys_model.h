/*
 * sys_model.h
 *
 *  Created on: 29 nov 2015
 *      Author: rmcar
 */

#include "global_types.h"

#ifndef INTERFACES_REG_SYS_MODEL_H_
#define INTERFACES_REG_SYS_MODEL_H_

void
init_model();

void
exec_model(double e);

temperature_t
get_temp_model ();

#endif /* INTERFACES_REG_SYS_MODEL_H_ */
