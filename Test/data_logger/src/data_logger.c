/*
 * data_logger.c
 *
 *  Created on: 19 dec 2015
 *      Author: carltmik
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "temp.h"
#include "heater.h"

typedef struct da_t
{
  temperature_t temp;
  temperature_t temp_top;
  power_t power;
} da_t;

int log_ix = 0;
da_t* log_buf_p;
#define MAX_NROF_LOG_VALUES (10000)

void
da_init ()
{

  log_buf_p = (da_t*) malloc (sizeof(da_t) * MAX_NROF_LOG_VALUES);
  (void) memset (log_buf_p, 0, sizeof(da_t) * MAX_NROF_LOG_VALUES);

}

void
da_log_val (temperature_t t_top, temperature_t t_bottom, power_t p)
{
  log_buf_p[log_ix].power = p;
  log_buf_p[log_ix].temp = t_bottom;
  log_buf_p[log_ix].temp_top = t_top;

  if (log_ix < MAX_NROF_LOG_VALUES)
    log_ix++;

}

void
da_dump ()
{
  int ix = 0;
  int fd = 0;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

#define MAX_LOG_LEN 1000
  char buf[MAX_LOG_LEN];
  char header[] = "power, temperature\n";
  time_t t = time (NULL);
  struct tm tm = *localtime (&t);

  snprintf (buf, MAX_LOG_LEN, "%d-%d-%d_%d%d%d.csv", tm.tm_year + 1900,
	    tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

  fd = open (buf, O_RDWR|O_CREAT, mode);

  write (fd, header, strlen (header));

  for (ix = 0; ix <= log_ix; ix++)
    {
      snprintf (buf, MAX_LOG_LEN, "%d, %.1lf\n", log_buf_p[ix].power,
		log_buf_p[ix].temp);
      write (fd, buf, strlen (buf));
    }
  close (fd);
}
