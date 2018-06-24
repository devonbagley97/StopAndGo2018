// lab3.c CMPE 240
// Larry Kiser April 22, 2018

#include <stdlib.h>
#include <stdio.h>

#include "lab3.h"

static bool stop_ready_state = false ;	// make this persistent protected storage

// The input struct pointer has all of the current states of the inputs.
// This function returns true when all inputs are true except that the
// in_stop_and_go must be false. It returns false otherwise.
bool lab3_part1_inputs( struct lab3_part1_inputs *p_inputs )
{
	stop_ready_state = false ;	// set default to false

	if (p_inputs->is_stationary == true && 
p_inputs->is_normal == true && 
p_inputs->external_temp_ok == true && 
p_inputs->in_stop_and_go == false && 
p_inputs->stop_start_enabled == true && 
p_inputs->in_neutral == true && 
p_inputs->clutch_engaged == true ){

	stop_ready_state = true ;

	}
	return stop_ready_state ;
}

// Now check for the clutch and manual transmission position (ignore other inputs)
bool lab3_part1_auto_restart( struct lab3_part1_inputs *p_inputs )
{
	bool do_restart = false ;
	if (p_inputs->in_neutral == true && p_inputs->clutch_engaged == false && 
stop_ready_state == true ){
	do_restart = true ;
	stop_ready_state = false;
	}

	return do_restart ;
}

// read-only accessor to stop_ready_state
bool lab3_get_stop_ready_state()
{
	return stop_ready_state ;		
}
