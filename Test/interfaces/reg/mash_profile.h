/*
 * mash_profile.h
 *
 *  Created on: 17 dec 2015
 *      Author: rmcar
 */

#include "temp.h"

#ifndef INTERFACES_REG_MASH_PROFILE_H_
#define INTERFACES_REG_MASH_PROFILE_H_

#define MAX_MASH_STEPS (10)
#define MAX_LEN_STEP_NAME (256)
typedef struct mash_profile_ta
{
  double time;
  temperature_t temp;
  char name[MAX_LEN_STEP_NAME];

} mash_profile_t;

void
mash_init();

void
mash_exec();

#endif /* INTERFACES_REG_MASH_PROFILE_H_ */
