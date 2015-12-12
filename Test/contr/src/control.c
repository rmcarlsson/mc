/*
 * control.c
 *
 *  Created on: 12 dec 2015
 *      Author: carltmik
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "pid.h"
#include "pid_atune.h"
#include "temp.h"

double i = 20;
double o = 80;
double setpoint = 24;
int tuning = 0;

void
control_init ()
{

  double kp = 2, ki = 0.5, kd = 2;

  double aTuneStep = 50, aTuneNoise = 1;
  unsigned int aTuneLookBack = 500;

  PID_ATune (&i, &o);
  SetNoiseBand (aTuneNoise);
  SetOutputStep (aTuneStep);
  SetLookbackSec ((int) aTuneLookBack);

  PID (&i, &o, &setpoint, kp, ki, kd, DIRECT);
  SetSampleTime (5000);
  SetOutputLimits (0, 100);
  SetMode (AUTOMATIC);

}

int
control_exec ()
{

  i = get_temp ();

  if (tuning == 0)
    {
      tuning = Runtime ();
      if (tuning != 0)
	{
	  SetTunings (pidat_GetKp (), pidat_GetKi (), pidat_GetKd ());
	}
    }
  else
    { //we're done, set the tuning parameters
      Compute ();
    }

  int set = 0;
  if (o < 0)
    set = 0;
  else
    set = (uint16_t) (o);

  return set;
}

bool
get_pid_params (double* kp, double* ki, double* kd)
{

  char filename[] = "pid.par";
  char line[100];
  size_t len = 100;
  bool ret = false;
  double d = 0;
  FILE* fd;

  fd = fopen (filename, "r");
  if (fd == NULL)
    {
      perror ("Couldn't open the pid.par.");
      return 1;
    }

  for (int i = 0; i < 3; i++)
    {
      char* ok = fgets(line, len, fd);
      if (ok == NULL)
	break;
      else
	{
	  sscanf (line, "%lf", &d);
	  switch (i)
	    {
	    case 0:
	      *kp = d;
	      break;
	    case 1:
	      *ki = d;
	      break;
	    case 2:
	      *kd = d;
	      ret = true;
	      break;
	    }
	}
    }
  (void)fclose (fd);

  return ret;
}

void
save_pid_params (double kp, double ki, double kd)
{

  char filename[] = "pid.par";
  FILE* fp;
  char buf[256];

  fp = fopen (filename, "w");
  if (fp == NULL)
    {
      perror ("Couldn't open the pid.par.");
      assert(0);
    }
  sprintf (buf, "%.5lf\n", kp);
  fwrite (buf, sizeof(buf), 1, fp);

  sprintf (buf, "%.5lf\n", ki);
  fwrite (buf, sizeof(buf), 1, fp);

  sprintf (buf, "%.5lf\n", kd);
  fwrite (buf, sizeof(buf), 1, fp);

  (void)fclose (fp);

}

