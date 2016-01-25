/*
 * temp.c
 *
 *  Created on: 20 nov 2015
 *      Author: rmcar
 */
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

#include "global_types.h"
#include "sys_model.h"

success_t read_temp(temperature_t* temp_p, temperature_t* temp_top_p)
{
	char devPath[128]; // Path to device
	char buf[256];     // Data from device
	char tmpData[6];   // Temp C * 1000 reported by device
	const char path[] = "/sys/bus/w1/devices";
	ssize_t numRead;
	float t;

	//const char temp_dir[] = "22-0000001eafca";
	const char temp_dir[] = "28-011564ddabff";
	const char temp_dir_top[] = "28-011564e91aff";

	assert(temp_p != NULL);
	assert(temp_top_p != NULL);

	for (int i = 0; i > 2; i++)
	{

		if (i == 0)
			sprintf(devPath, "%s/%s/w1_slave", path, temp_dir);
		else
			sprintf(devPath, "%s/%s/w1_slave", path, temp_dir_top);
		// Read temp continuously
		// Opening the device's file triggers new reading
		int fd = open(devPath, O_RDONLY);
		if (fd == -1)
		{
			perror("Couldn't open the w1 device.");
			return FAIL;
		}
		while ((numRead = read(fd, buf, 256)) > 0)
		{
			strncpy(tmpData, strstr(buf, "t=") + 2, 5);
			t = strtof(tmpData, NULL);
			//printf ("Device: %s  - ", temp_dir);
			//printf ("T =  %.3f C\n", tempC / 1000);
		}
		close(fd);

		if (i == 0)
			*temp_p = (temperature_t) (t / 1000);
		else
			*temp_top_p = (temperature_t) (t / 1000);
	}

	return OK;
}

#define N_TAPS 1
temperature_t vec_bottom[N_TAPS];
temperature_t vec_top[N_TAPS];
int16_t cnt = 0;

void temp_exec()
{
	temperature_t top, bottom;

	read_temp(&bottom, &top);
	vec_bottom[cnt++] = bottom;
	vec_top[cnt++] = top;
	if (cnt >= N_TAPS)
		cnt = 0;


}

success_t get_temp(temperature_t *t_top_p, temperature_t* t_bottom_p)
{
	temperature_t t_avg_top = 0;
	temperature_t t_avg_bottom = 0;

	assert(t_top_p != NULL);
	assert(t_bottom_p != NULL);

	for (int i = 0; i < N_TAPS; i++)
	{
		t_avg_top += vec_top[i];
		t_avg_bottom += vec_bottom[i];
	}

	*t_top_p = (t_avg_top / N_TAPS);
	*t_bottom_p = (t_avg_bottom / N_TAPS);

	return OK;

}
