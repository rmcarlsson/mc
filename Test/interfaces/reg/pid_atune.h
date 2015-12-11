/*
 * pid_atune.h
 *
 *  Created on: 11 dec 2015
 *      Author: rmcar
 */

#ifndef INTERFACES_REG_PID_ATUNE_H_
#define INTERFACES_REG_PID_ATUNE_H_

//commonly used functions **************************************************************************
PID_ATune (double*, double*); // * Constructor.  links the Autotune to a given PID

int
Runtime ();	// * Similar to the PID Compue function, returns non 0 when done

void
Cancel ();						// * Stops the AutoTune

void
SetOutputStep (double);	// * how far above and below the starting value will the output step?

double
GetOutputStep ();							   //

void
SetControlType (int); // * Determies if the tuning parameters returned will be PI (D=0)

int
GetControlType ();				//   or PID.  (0=PI, 1=PID)

void
SetLookbackSec (int);	// * how far back are we looking to identify peaks

int
GetLookbackSec ();							//

void
SetNoiseBand (double);// * the autotune will ignore signal chatter smaller than this value

double
GetNoiseBand ();			//   this should be acurately set

double
pidat_GetKp ();	// * once autotune is complete, these functions contain the

double
pidat_GetKi ();					//   computed tuning parameters.

double
pidat_GetKd ();								//

#endif /* INTERFACES_REG_PID_ATUNE_H_ */
