/*
 * gpio.c
 *
 *  Created on: 22 nov 2015
 *      Author: rmcar
 */

/* blink.c
 *
 * Raspberry Pi GPIO example using sysfs interface.
 * Guillermo A. Amaral B. <g@maral.me>
 *
 * This file blinks GPIO 4 (P1-07) while reading GPIO 24 (P1_18).
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "gpio.h"

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define POUT 23

int fd_wr = 0;



int
gpio_export (int pin)
{
#define BUFFER_MAX 3
  char buffer[BUFFER_MAX];
  ssize_t bytes_written;
  int fd;

  fd = open ("/sys/class/gpio/export", O_WRONLY);
  if (-1 == fd)
    {
      fprintf (stderr, "Failed to open export for writing!\n");
      assert(0);
    }

  bytes_written = snprintf (buffer, BUFFER_MAX, "%d", pin);
  write (fd, buffer, bytes_written);
  close (fd);
  return (0);
}

int
gpio_unexport (int pin)
{
  char buffer[BUFFER_MAX];
  ssize_t bytes_written;
  int fd;

  fd = open ("/sys/class/gpio/unexport", O_WRONLY);
  if (-1 == fd)
    {
      fprintf (stderr, "Failed to open unexport for writing!\n");
      assert(0);
    }

  bytes_written = snprintf (buffer, BUFFER_MAX, "%d", pin);
  write (fd, buffer, bytes_written);
  close (fd);
  return (0);
}

int
gpio_direction (int pin, int dir)
{
  static const char s_directions_str[] = "in\0out";

#define DIRECTION_MAX 35
  char path[DIRECTION_MAX];
  int fd;

  snprintf (path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
  fd = open (path, O_WRONLY);
  if (-1 == fd)
    {
      fprintf (stderr, "Failed to open gpio direction for writing!\n");
      assert(0);
    }

  if (-1 == write (fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3))
    {
      fprintf (stderr, "Failed to set direction!\n");
      assert(0);
    }

  close (fd);
  return (0);
}

int
gpio_read (int pin)
{
#define VALUE_MAX 30
  char path[VALUE_MAX];
  char value_str[3];
  int fd;

  snprintf (path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
  fd = open (path, O_RDONLY);
  if (-1 == fd)
    {
      fprintf (stderr, "Failed to open gpio value for reading!\n");
      return (-1);
    }

  if (-1 == read (fd, value_str, 3))
    {
      fprintf (stderr, "Failed to read value!\n");
      assert(0);
    }

  close (fd);

  return (atoi (value_str));
}

void
gpio_write_val(int value)
{
  static const char s_values_str[] = "01";

  char path[VALUE_MAX];

  snprintf (path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", POUT);
  //printf ("%s\n", path);
  if (1 != write (fd_wr, &s_values_str[LOW == value ? 0 : 1], 1))
    {
      fprintf (stderr, "Failed to write value!\n");
      assert (0);
    }
}
int
gpio_write(int pin, int value)
{
  static const char s_values_str[] = "01";

  char path[VALUE_MAX];
  int fd;

  snprintf (path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
  fd = open (path, O_WRONLY);
  if (-1 == fd)
    {
      fprintf (stderr, "Failed to open gpio value for writing!\n");
      assert(0);
    }

  if (1 != write (fd, &s_values_str[LOW == value ? 0 : 1], 1))
    {
      fprintf (stderr, "Failed to write value!\n");
      assert(0);
    }

  close (fd);
  return (0);
}

void
gpio_init()
{
#define VALUE_MAX 30
  char path[VALUE_MAX];
  /*
   * Export digital pin for "slow-PWM"
   */
  if (-1 == gpio_export (POUT))
    assert(0);

  /*
   * Set GPIO directions
   */
  if (-1 == gpio_direction (POUT, OUT))
    assert (0);

  snprintf (path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", POUT);
  fd_wr = open (path, O_WRONLY);
  if (-1 == fd_wr)
    {
      fprintf (stderr, "Failed to open gpio value for writing!\n");
      assert (0);

    }
}
