/*
 * This is a demonstration of periodic threads using POSIX timers and signals.
 * Each periodic thread is allocated a signal between SIGRTMIN to SIGRTMAX: we
 * assume that there are no other uses for these signals.
 *
 * All RT signals must be blocked in all threads before calling make_periodic()
 */

/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Copyright (C) 2009 Chris Simmonds (chris@2net.co.uk)                      */
/*                                                                           */
/* This program is free software; you can redistribute it and/or modify      */
/* it under the terms of the GNU General Public License as published by      */
/* the Free Software Foundation; either version 2 of the License, or         */
/* (at your option) any later version.                                       */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU          */
/* General Public License for more details.                                  */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with this program; if not, write to the Free Software               */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA   */
/*                                                                           */
/* ------------------------------------------------------------------------- */

#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#include "heater.h"
#include "tasks.h"
#include "mash_profile.h"

struct periodic_info
{
  int sig;
  sigset_t alarm_sig;
};

static int
make_periodic (int unsigned period, struct periodic_info *info)
{
  static int next_sig;
  unsigned int ns;
  unsigned int sec;
  struct sigevent sigev;
  timer_t timer_id;
  struct itimerspec itval;
  int ret;

  /* Initialise next_sig first time through. We can't use static
   initialisation because SIGRTMIN is a function call, not a constant */
  if (next_sig == 0)
    next_sig = SIGRTMIN;
  /* Check that we have not run out of signals */
  if (next_sig > SIGRTMAX)
    return -1;
  info->sig = next_sig;
  printf ("Some thread allocated RTSIG number %d\n", next_sig);
  next_sig++;
  /* Create the signal mask that will be used in wait_period */
  sigemptyset (&(info->alarm_sig));
  sigaddset (&(info->alarm_sig), info->sig);

  /* Create a timer that will generate the signal we have chosen */
  sigev.sigev_notify = SIGEV_SIGNAL;
  sigev.sigev_signo = info->sig;
  sigev.sigev_value.sival_ptr = (void *) &timer_id;
  ret = timer_create (CLOCK_MONOTONIC, &sigev, &timer_id);
  if (ret == -1)
    return ret;

  /* Make the timer periodic */
  sec = period / 1000000;
  ns = (period - (sec * 1000000)) * 1000;
  itval.it_interval.tv_sec = sec;
  itval.it_interval.tv_nsec = ns;
  itval.it_value.tv_sec = sec;
  itval.it_value.tv_nsec = ns;
  ret = timer_settime (timer_id, 0, &itval, NULL);
  return ret;
}

static void
wait_period (struct periodic_info *info)
{
  int sig;
  sigwait (&(info->alarm_sig), &sig);
}

static void *
base_thread_1s (void *arg)
{
  (void) (arg);
  struct periodic_info info;

  make_periodic (5000000, &info);
  while (1)
    {
      exec_one_hz ();
      wait_period (&info);
    }
  return NULL;
}


static void *
base_thread_5s (void *arg)
{
  (void) (arg);
  struct periodic_info info;

  make_periodic (5000000, &info);
  while (1)
    {
      heater_exec();
      wait_period (&info);
    }
  return NULL;
}

static void *
base_thread_60s (void *arg)
{
  (void) (arg);
  struct periodic_info info;

  make_periodic (60000000, &info);
  while (1)
    {
      mash_exec();
      wait_period (&info);
    }
  return NULL;
}


int
main (int argc, char* argv[])
{
  pthread_t pt1;
  pthread_t pt2;
  pthread_t pt3;
  sigset_t alarm_sig;
  int i;

  (void)

  /* Block all real time signals so they can be used for the timers.
   Note: this has to be done in main() before any threads are created
   so they all inherit the same mask. Doing it later is subject to
   race conditions */
  sigemptyset (&alarm_sig);
  for (i = SIGRTMIN; i <= SIGRTMAX; i++)
    sigaddset (&alarm_sig, i);
  sigprocmask (SIG_BLOCK, &alarm_sig, NULL);

  init ();

  pthread_create (&pt1, NULL, base_thread_1s, NULL);
  pthread_create (&pt2, NULL, base_thread_5s, NULL);
  pthread_create (&pt3, NULL, base_thread_60s, NULL);


  (void)pthread_join (pt1, NULL);
  return 0;
}


