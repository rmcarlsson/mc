/*
 * data_logger.h
 *
 *  Created on: 19 dec 2015
 *      Author: carltmik
 */

#ifndef INTERFACES_REG_DATA_LOGGER_H_
#define INTERFACES_REG_DATA_LOGGER_H_

void
da_log_val (temperature_t t, power_t p);

void
da_init();

void
da_dump();

#endif /* INTERFACES_REG_DATA_LOGGER_H_ */
