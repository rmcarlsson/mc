/*
 * mash_profile.c
 *
 *  Created on: 19 nov 2015
 *      Author: rmcar
 */

#include "stdint.h"

#define MASH_TIME 0
#define MASH_TEMP 1

typedef struct mash_profile_ta
{
	uint16_t time;
	uint16_t temp;

} mash_profile_t;

const mash_profile_t mp[10] =
{
	{.time = 40, .temp = 64},
	{.time = 10, .temp = 64},
};
