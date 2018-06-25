// unit_tests.c
// Larry L. Kiser Oct. 22, 2015

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdbool.h>

#include "unit_tests.h"
#include "lab3.h"

// Simple boolean assert function for unit testing
int assert( int test_result, char error_format[], ... ) {
	va_list arguments ;
	static int test_number = 1 ;
	int result = 1 ;	// return 1 for test passed or 0 if failed
	
	if ( ! test_result ) {
		va_start( arguments, error_format ) ;
		printf( "Test # %d failed: ", test_number ) ;
		vprintf( error_format, arguments ) ;
		printf( "\n" ) ;
		result = 0 ;
	}
	test_number++ ;
	return result ;
}

//////////////////////////////////////////////////////////////////////////
// Begin unit tests //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Write your unit tests below. I have provided some simple tests to get started.
// Add, rearrange, delete, modify as you see fit to get a good set of tests for
// both boundry conditions and normal operation.
int test()
{
	int passcount = 0 ;
	int failcount = 0 ;
	struct lab3_part1_inputs part1_inputs ;
	
	memset( &part1_inputs, false, sizeof( part1_inputs ) ) ;		// easily set everything to false

	// lab3_part1_inputs tests
	printf( "\nStarting lab3_part1_inputs unit tests---------------------------------------------------\n" ) ;
	
	// Test 1 verify false output if all inputs are false
	assert( lab3_part1_inputs( &part1_inputs ) == false,
		"Expected false from lab3_part1_inputs with all inputs false but got true" )
		? passcount++ : failcount++ ;

	// Fault coverage -- Test 2 and Test 3 prove that that requirement 4 is implemented (stop_and_go requirement).
	// Test 2 check with all inputs true (verifies that in_stop_and_go being true keeps output false.
	memset( &part1_inputs, true, sizeof( part1_inputs ) ) ;		// easily set everything to true
	assert( lab3_part1_inputs( &part1_inputs ) == false,
		"Expected false from lab3_part1_inputs with all inputs true but got true" )
		? passcount++ : failcount++ ;
	
	// Test 3 check with all inputs true except in_stop_and_go being false sets output true.
	part1_inputs.in_stop_and_go = false ;	// change just the in_stop_and_go state
	assert( lab3_part1_inputs( &part1_inputs ) == true,
		"Expected true from lab3_part1_inputs with all inputs true but got false" )
		? passcount++ : failcount++ ;
	
	// Add Tests 4, 5, and 6 to verify that requirements 1, 2, and 3 when false set the output to false.
	part1_inputs.is_stationary = false ;
	assert( lab3_part1_inputs( &part1_inputs ) == false,
		"Vehicle is not stationary, output should be false" )
		? passcount++ : failcount++ ;
	part1_inputs.is_stationary = true ;
	
	part1_inputs.is_normal = false ;
        assert( lab3_part1_inputs( &part1_inputs ) == false,
                "Vehicle temp is not normal, output should be false" )
                ? passcount++ : failcount++ ;
        part1_inputs.is_normal = true ;
	
	part1_inputs.external_temp_ok = false ;
        assert( lab3_part1_inputs( &part1_inputs ) == false,
                "Vehicle external temp is not ok, output should be false" )
                ? passcount++ : failcount++ ;
        part1_inputs.external_temp_ok = true ;
	
        // Add Tests 7, 8, and 9 to verify that requirements 5, 6, and 7 when false set 

	part1_inputs.stop_start_enabled = false ;
        assert( lab3_part1_inputs( &part1_inputs ) == false,
                "Vehicle stop/start system disabled, output should be false" )
                ? passcount++ : failcount++ ;
        part1_inputs.stop_start_enabled = true ;

	part1_inputs.in_neutral = false ;
        assert( lab3_part1_inputs( &part1_inputs ) == false,
                "Vehicle is not in neutral, output should be false" )
                ? passcount++ : failcount++ ;
        part1_inputs.in_neutral = true ;

	part1_inputs.clutch_engaged = false ;
        assert( lab3_part1_inputs( &part1_inputs ) == false,
                "Vehicle clutch disengaged, output should be false" )
                ? passcount++ : failcount++ ;
        part1_inputs.clutch_engaged = true ;

	
	// lab3_get_stop_ready_state tests
	printf( "\nStarting lab3_get_stop_ready_state unit tests--------------------------------------------\n" ) ;

	// Test 10 verify false output (this should always pass but let's be sure!)
	// This duplicates previous testing.
	part1_inputs.clutch_engaged = false ;
	assert( lab3_part1_inputs( &part1_inputs ) == false,
		"Expected false from lab3_part1_inputs with clutch_engaged false but got true" )
		? passcount++ : failcount++ ;
	
	// Test 11 confirm that the accessor returns false.
	assert( lab3_get_stop_ready_state() == false,
		"Expected false from lab3_get_stop_ready_state with clutch_engaged false but got true" )
		? passcount++ : failcount++ ;
		
	// Test 12 see that we get true when we expect true for the stop/ready state
	// Now set up for state to be true. This should always pass since it duplicates previous tests.
	memset( &part1_inputs, true, sizeof( part1_inputs ) ) ;		// easily set everything to true
	part1_inputs.in_stop_and_go = false ;
	assert( lab3_part1_inputs( &part1_inputs ) == true,
		"Expected true from lab3_part1_inputs but got false" )
		? passcount++ : failcount++ ;
	
	// Test 13 confirm that the accessor returns true.
	assert( lab3_get_stop_ready_state() == true,
		"Expected true from lab3_get_stop_ready_state but got false" )
		? passcount++ : failcount++ ;
	
	// lab3_part1_auto_restart tests
	printf( "\nStarting lab3_part1_auto_restart unit tests----------------------------------------------\n" ) ;
	
	// Since we are in the stop/ready state from Test 13 test the other two inputs.
	// Test 14 Confirm that with the clutch engaged while in neutral that it will not auto restart.
	part1_inputs.in_neutral = true ;
	part1_inputs.clutch_engaged = true ;		// set clutch to engaged
	assert( lab3_part1_auto_restart( &part1_inputs ) == false,
		"Expected false from lab3_part1_auto_restart (clutch engaged) but got true" )
		? passcount++ : failcount++ ;
	
	// Test 15 Confirm that with the clutch disengaged and NOT in neutral that it will not auto restart.
	part1_inputs.in_neutral = false ;
	part1_inputs.clutch_engaged = false ;		// set clutch to engaged
	assert( lab3_part1_auto_restart( &part1_inputs ) == false,
		"Expected false from lab3_part1_auto_restart (NOT in neutral) but got true" )
		? passcount++ : failcount++ ;
	
	// Test 16 See that we can now do an auto restart.
	// We are in the stop/ready state, set to neutral and disengage the clutch.
	part1_inputs.in_neutral = true ;
	part1_inputs.clutch_engaged = false ;		// set clutch to disengaged
	assert( lab3_part1_auto_restart( &part1_inputs ) == true,
		"Expected true from lab3_part1_auto_restart but got false" )
		? passcount++ : failcount++ ;

	// Test 17 After doing an auto restart we are no longer in the stop/ready state.
	assert( lab3_get_stop_ready_state() == false,
		"Expected false from lab3_get_stop_ready_state because we did an auto restart but got true" )
		? passcount++ : failcount++ ;
	
	// Test 18 Confirm that the auto restart function only returns true when we are in the stop/ready state
	assert( lab3_part1_auto_restart( &part1_inputs ) == false,
		"Expected false from lab3_part1_auto_restart because not in stop/ready state but got true" )
		? passcount++ : failcount++ ;

	// Test 19 go back to the stop/ready state
	// Now set up for state to be true. This should always pass since it duplicates previous tests.
	memset( &part1_inputs, true, sizeof( part1_inputs ) ) ;		// easily set everything to true
	part1_inputs.in_stop_and_go = false ;
	assert( lab3_part1_inputs( &part1_inputs ) == true,
		"Expected true from lab3_part1_inputs but got false" )
		? passcount++ : failcount++ ;
	
	// Test 20 Create test to confirm that lab3_part1_auto_restart returns false
	// when the clutch is engaged and we are in neutral.
	part1_inputs.clutch_engaged = true ;
        part1_inputs.in_neutral = true ;
	assert( lab3_part1_auto_restart( &part1_inputs ) == false,
		"Expected false due to being in stop/ready state, clutch engaged, and in neutral" )
		? passcount++ : failcount++ ;
	
	// Test 21 Create test to confirm that lab3_part1_auto_restart returns false
	// when the clutch is disengaged and we are NOT in neutral.
	part1_inputs.clutch_engaged = false ;
	part1_inputs.in_neutral = false ;
	assert( lab3_part1_auto_restart( &part1_inputs ) == false,
                "Expected false due to being in stop/ready state, clutch engaged, and in neutral" )
                ? passcount++ : failcount++ ;
	
	// Now test the Part 2 functions
	printf( "\nStarting lab3_part2 add_stop_event and get_active_stop_events unit tests-----------------\n" ) ;
	// Test 22 Check that get_active_stop_events count is 0
	assert( get_active_stop_events() == 0,
			"Initial stop events must be 0" )
			? passcount++ : failcount++ ;
	
	// Test 23 add one stop and get the count
	add_stop_event() ;
	assert( get_active_stop_events() == 1,
			"Stop events must now be 1" )
			? passcount++ : failcount++ ;
	
	// Test 24 add second stop and get the count
	add_stop_event() ;
	assert( get_active_stop_events() == 2,
			"Stop events must now be 2" )
			? passcount++ : failcount++ ;

	// Test 25 add third stop and get the count
	add_stop_event() ;
	assert( get_active_stop_events() == 3,
			"Stop events must now be 3" )
			? passcount++ : failcount++ ;
			
	// Test 26 attempt to add a fourth stop and get the count
	add_stop_event() ;		// this will do nothing
	assert( get_active_stop_events() == 3,
			"Stop events must still be 3 despite 4th add attempt" )
			? passcount++ : failcount++ ;

	printf( "\nStarting record_one_minute_elapsed and sag_traffic_detected function unit tests----------\n" ) ;
	// Test 27 first test of sag_traffic_detected and expect true.
	assert( sag_traffic_detected() == true,
			"Expect true due to three stop events in less than a minute but got false" )
			? passcount++ : failcount++ ;

	record_one_minute_elapsed() ;	// wait 4 minutes
	record_one_minute_elapsed() ;
	record_one_minute_elapsed() ;
	record_one_minute_elapsed() ;
	// Test 28 second test of sag_traffic_detected after 4 minutes have elasped and still expect true.
	assert( sag_traffic_detected() == true,
			"Expect true due to three stop events in last 5 minutes but got false" )
			? passcount++ : failcount++ ;
			
	record_one_minute_elapsed() ;
	// Test 29 third test of sag_traffic_detected after 5 minutes have elasped and now expect false.
	assert( sag_traffic_detected() == false,
			"Expect false due to 5 minutes since last stop event but got true" )
			? passcount++ : failcount++ ;
	
	record_one_minute_elapsed() ;
	// Test 30 make sure record_one_minute_elapsed works OK with no stops in array.
	assert( sag_traffic_detected() == false,
			"Expect false due to 5 minutes since last stop event but got true" )
			? passcount++ : failcount++ ;
	
	// Now test a more involved combination of stops and minutes elapsing.
	// Test 31 add two events separated by a minute and then record one more minute
	add_stop_event() ;
	record_one_minute_elapsed() ;
	add_stop_event() ;
	record_one_minute_elapsed() ;
	assert( get_active_stop_events() == 2,
			"Stop events must 2 because only a little time has elapsed but got %d", get_active_stop_events() )
			? passcount++ : failcount++ ;

	// Test 32 Now have 3 events in the last 5 minutes so SAG is true.
	add_stop_event() ;
	record_one_minute_elapsed() ;		// buffer now has 4, 3, and 2 minutes in it.
	assert( sag_traffic_detected() == true,
			"Expect true due to three stop events in last 5 minutes (at 4, 3, 2) but got false" )
			? passcount++ : failcount++ ;
	
	// Test 33 Still have 3 events in the last 5 minutes with oldest one replace so SAG is true.
	add_stop_event() ;		// this should replace the 2 minutes so we now have 5, 4, 3 in the array
	assert( sag_traffic_detected() == true,
			"Expect true due to three stop events in last 5 minutes (now at 5, 4, 3) but got false" )
			? passcount++ : failcount++ ;
	
	// Test 34 Still have 3 events in last five
	record_one_minute_elapsed() ;		// buffer now has 4, 3, and 2 minutes in it.
	record_one_minute_elapsed() ;		// buffer now has 3, 2, and 1 minutes in it.
	assert( sag_traffic_detected() == true,
			"Expect true due to three stop events in last 5 minutes (now at 3, 2, 1) but got false" )
			? passcount++ : failcount++ ;
	
	
	printf( "\nSummary of unit tests:\n%d tests passed\n%d tests failed\n\n", passcount, failcount ) ;
	
	return failcount ;
}
