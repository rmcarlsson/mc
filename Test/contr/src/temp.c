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

#include "global_types.h"
#include "sys_model.h"

#if defined(SIMULATED)
temperature_t
get_temp ()
{
  return (temperature_t) get_temp_model ();

}
#else
temperature_t
read_temp ()
{
  char devPath[128]; // Path to device
  char buf[256];     // Data from device
  char tmpData[6];   // Temp C * 1000 reported by device
  const char path[] = "/sys/bus/w1/devices";
  ssize_t numRead;
  float tempC;

  //const char temp_dir[] = "22-0000001eafca";
  const char temp_dir[] = "28-011564ddabff";

  sprintf (devPath, "%s/%s/w1_slave", path, temp_dir);
  // Read temp continuously
  // Opening the device's file triggers new reading
  int fd = open (devPath, O_RDONLY);
  if (fd == -1)
    {
      perror ("Couldn't open the w1 device.");
      return 1;
    }
  while ((numRead = read (fd, buf, 256)) > 0)
    {
      strncpy (tmpData, strstr (buf, "t=") + 2, 5);
      tempC = strtof (tmpData, NULL);
      //printf ("Device: %s  - ", temp_dir);
      //printf ("T =  %.3f C\n", tempC / 1000);
    }
  close (fd);

  return (temperature_t)(tempC/1000);

}
#endif /* defined(SIMULATED) */

#define N_TAPS 1
temperature_t vec[N_TAPS];
int16_t cnt = 0;

void
temp_exec()
{
  vec[cnt++] = read_temp();
  if (cnt >= N_TAPS)
    cnt = 0;

}

temperature_t
get_temp()
{
  static temperature_t t_last = 0;
  temperature_t t_avg = 0;
  for (int i = 0; i<N_TAPS; i++ )
    t_avg += vec[i];

  if (fabs(t_last-(t_avg/N_TAPS)) > 1)
    {
      printf("T=%.3lf\n",(t_avg/N_TAPS) );
      t_last = t_avg;
    }

  return (t_avg/N_TAPS);
}
