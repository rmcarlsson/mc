/*
 * mash_profile.c
 *
 *  Created on: 19 nov 2015
 *      Author: rmcar
 */

#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "control.h"
#include "data_logger.h"
#include "mash_profile.h"
#include "profile_reader.h"

/*
 * Tw = (.41/r)(T2 - T1) + T2
 *
 * r = 4 [kg water] / 1.3 [kg grains] = 2.8
 *
 * thermodynamic constant = 0.41
 *
 * T1 = grain temp
 * T2 = target mash temp
 *
 * Tw = strike water temp
 *
 */
#define mash_water_volume ((double)(3.9))
#define grain_weight  ((double)(1.3))

const double r = (mash_water_volume/grain_weight);
const double grain_temp = 20;
const double tdc = 0.41;

int state;

enum state_t {
	INIT, WAIT_FOR_GRAINS, MASHING, HEATING, DONE
};

mash_profile_t mp[MAX_MASH_STEPS];

int mash_step = 0;
int nrof_mash_steps = 10;
time_t step_start = 0;

void handle_heating() {
	if (control_get_state() == CONTROL_STABLE) {
		state = MASHING;
		printf(
				"Heating done, for %s at %.1lf C. Will stay for %.0lf minutes.\n",
				mp[mash_step].name, mp[mash_step].temp, mp[mash_step].time);
	}
}

void handle_done() {

}

void handle_mashing() {
	time_t now = time(NULL);

	if (step_start == 0)
		step_start = time(NULL);

	double diff_time = difftime(now, step_start);
	if (diff_time >= (double) (mp[mash_step].time * 60)) {
		step_start = 0;

		mash_step++;
		if (mash_step >= nrof_mash_steps) {
			state = DONE;
			printf("Mash done\n");
			da_dump();
			control_set_target(0);
		} else {
			state = HEATING;
			control_set_target(mp[mash_step].temp);
			printf("Starting next step, %s. Heating to %.1lf C\n",
					mp[mash_step].name, mp[mash_step].temp);
		}
	}
}

void handle_wait_for_grains() {
	char *line = NULL;
	size_t size;

	/* Reset temperature control from strike water temperature to first mash step temperature */
	control_set_target(mp[0].temp);

	printf(
			"Strike water temperature reached, targeting first mash step at %.1lf C. Please add grains.\n",
			mp[0].temp);
	printf("Hit any key when done.\n");

	(void) getline(&line, &size, stdin);

	printf("Starting first mash step, %s, at %.lf C\n", mp[mash_step].name,
			mp[mash_step].temp);
	state = MASHING;

}

void handle_init() {
	if (control_get_state() == CONTROL_STABLE)
		state = WAIT_FOR_GRAINS;
}

void mash_init() {

	memset(&mp[0], 0, (sizeof(mash_profile_t) * MAX_MASH_STEPS));

	read_mash_profile(&mp[0], &nrof_mash_steps);

	temperature_t strike_water_temp = (tdc / r) * (mp[0].temp - grain_temp)
			+ mp[0].temp;

	printf(
			"Heating to strike water temperature at %.1lf C to hit first mash step, %s, at %.1lf C\n",
			strike_water_temp, mp[0].name, mp[0].temp);

	control_set_target(strike_water_temp);
	state = INIT;
}

void mash_exec() {

	switch (state) {
	case INIT:
		handle_init();
		break;
	case WAIT_FOR_GRAINS:
		handle_wait_for_grains();
		break;
	case HEATING:
		handle_heating();
		break;
	case MASHING:
		handle_mashing();
		break;
	case DONE:
		handle_done();
		break;
	default:
		assert(0);
		break;
	}

}

