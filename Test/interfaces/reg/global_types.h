/*
 * global_types.h
 *
 *  Created on: 22 nov 2015
 *      Author: rmcar
 */

#include <stdint.h>

#ifndef INTERFACES_REG_GLOBAL_TYPES_H_
#define INTERFACES_REG_GLOBAL_TYPES_H_

typedef uint16_t percent_t;
#define MAX_PERCENT (percent_t)(100)
#define MIN_PERCENT (percent_t)(0)

typedef int16_t power_t;

typedef double temperature_t;

typedef enum {
	OK, FAIL
} success_t;

#endif /* INTERFACES_REG_GLOBAL_TYPES_H_ */
