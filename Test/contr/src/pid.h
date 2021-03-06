/*
 * pid.h
 *
 *  Created on: 19 nov 2015
 *      Author: rmcar
 */

#include <stdint.h>
#include "global_types.h"
#include <stdbool.h>

#ifndef REG_INCLUDE_PID_H_
#define REG_INCLUDE_PID_H_

//Constants used in some of the functions below
#define AUTOMATIC	1
#define MANUAL	0
#define DIRECT  0
#define REVERSE  1

//commonly used functions **************************************************************************
void PID (double* Input, double* Output, double* Setpoint, double Kp, double Ki, double Kd, int ControllerDirection);
 // * constructor.  links the PID to the Input, Output, and
//   Setpoint.  Initial tuning parameters are also set here

void
SetMode (int Mode);           // * sets PID to either Manual (0) or Auto (non-0)

void
Compute ();                     // * performs the PID calculation.  it should be
//   called every time loop() cycles. ON/OFF and
//   calculation frequency can be set using SetMode
//   SetSampleTime respectively

void
SetOutputLimits (double, double); //clamps the output to a specific range. 0-255 by default, but
//it's likely the user will want to change this depending on
//the application

//available but not commonly used functions ********************************************************
void
SetTunings (double, double, // * While most users will set the tunings once in the
	    double);   //   constructor, this function gives the user the option
//   of changing tunings during runtime for Adaptive control
void
SetControllerDirection (int);// * Sets the Direction, or "Action" of the controller. DIRECT
//   means the output will increase when error is positive. REVERSE
//   means the opposite.  it's very unlikely that this will be needed
//   once it is set in the constructor.
void
SetSampleTime (int);        // * sets the frequency, in Milliseconds, with which
//   the PID calculation is performed.  default is 100

//Display functions ****************************************************************
double
GetKp ();		// These functions query the pid for interal values.
double
GetKi ();		//  they were created mainly for the pid front-end,
double
GetKd ();			// where it's important to know what is actually
int
GetMode ();						  //  inside the PID.
int
GetDirection ();					  //

#endif /* REG_INCLUDE_PID_H_ */
