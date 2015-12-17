/*
 * mash_profile.c
 *
 *  Created on: 19 nov 2015
 *      Author: rmcar
 */

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#include "control.h"

int state;

enum state_t
{
  INIT, WAIT_FOR_GRAINS, MASHING, HEATING, DONE
};

typedef struct mash_profile_ta
{
  uint16_t time;
  uint16_t temp;

} mash_profile_t;

#define NROF_MASH_STEPS (3)
const mash_profile_t mp[NROF_MASH_STEPS] =
  {
    { .time = 20, .temp = 55 },
    { .time = 60, .temp = 63 },
    { .time = 15, .temp = 75 } };

int mash_step = 0;
time_t step_start = 0;

void
handle_heating ()
{
  if (control_get_state () == CONTROL_STABLE)
    {
      state = MASHING;
      printf ("Heating done, now at %d. Will stay for %d minutes",
	      mp[mash_step].temp, mp[mash_step].time);
    }
}

void
handle_done ()
{

}

void
handle_mashing ()
{
  time_t now = time (NULL);

  if (step_start == 0)
    step_start = time (NULL);

  if (difftime (now, step_start) >= (mp[mash_step].time * 60))
    {
      step_start = 0;

      mash_step++;
      if (mash_step >= NROF_MASH_STEPS)
	{
	  state = DONE;
	  printf ("Mash done\n");
	  control_set_target(0);
	}
      else
	{
	  state = HEATING;
	  control_set_target (mp[mash_step].temp);
	  printf ("Next step. Heating to %d C\n", mp[mash_step].temp);
	}
    }
}

void
handle_wait_for_grains ()
{
  char ch;
  printf ("Mash-in reached, at %d C. Please add grains.\n", mp[0].temp);
  printf ("Hit any key when done.\n");
  read (STDIN_FILENO, &ch, 1);
  state = MASHING;

}

void
handle_init ()
{
  if (control_get_state () == CONTROL_STABLE)
    state = WAIT_FOR_GRAINS;
}

void
mash_init ()
{
  control_set_target (mp[0].temp);
  state = INIT;
}

void
mash_exec ()
{

  switch (state)
    {
    case INIT:
      handle_init ();
      break;
    case WAIT_FOR_GRAINS:
      handle_wait_for_grains ();
      break;
    case HEATING:
      handle_heating ();
      break;
    case MASHING:
      handle_mashing ();
      break;
    case DONE:
      handle_done ();
      break;
    default:
      assert (0);
      break;
    }

}

