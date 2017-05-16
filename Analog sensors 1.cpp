#include <iostream>
#include <time.h>
#include <csignal>
#include <stdlib.h>
#include "E101.h"

const int LEFT_SENSOR = 0;
const int RIGHT_SENSOR = 1;
	
const int LEFT_MOTOR = 1;
const int RIGHT_MOTOR = 2;

double mazeFactor = 0.1; // To be changed with testing.

bool mazeDone = false;


int main(){
	init();

	
	//Variables for the sensor readings.
	int left_reading = 0;
	int right_reading = 0;
	
	double sensor_difference = 0;
	
	
	

	while(!mazeDone){ //Looping whilst 'done' condition is not met.
		
		//Reading sensors - will need to be set to analog sensor pins.
		left_reading = read_analog(LEFT_SENSOR); 
		right_reading = read_analog(RIGHT_SENSOR);
		
		// If left reading is greater (further away), turn left.
		if(left_reading > right_reading){
			
			sensor_difference = (left_reading - right_reading) * mazeFactor;
			
			set_motor(LEFT_MOTOR, 50+sensor_difference);
			set_motor(RIGHT_MOTOR, 50-1*sensor_difference);
		}
		
		// If right reading is greater, turn right.
		else if(right_reading > left_reading){
			
			sensor_difference = (right_reading - left_reading) * mazeFactor;
			
			set_motor(LEFT_MOTOR, 50+sensor_difference);
			set_motor(RIGHT_MOTOR, 50-1*sensor_difference);
		}
		
		// Otherwise, continue forward.
		
		else{
			set_motor(LEFT_MOTOR, 70);
			set_motor(RIGHT_MOTOR, 70);
		}
		//Wait before re-looping.
		sleep1(0, 5000);
		
	}
	
return 0;}
