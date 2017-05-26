#include <iostream>
#include <time.h>
#include <csignal>
#include <stdlib.h>
#include <stdio.h>
#include "E101.h"

const int LEFT_SENSOR = 0;
const int RIGHT_SENSOR = 1;
	
const int LEFT_MOTOR = 1;
const int RIGHT_MOTOR = 2;

double mazeFactor = 0.15; // To be changed with testing.

bool mazeDone = false;


int main(){
	init();

	
	//Variables for the sensor readings.
	int left_reading = 0;
	int right_reading = 0;
	
	
	
	

	while(!mazeDone){ //Looping whilst 'done' condition is not met.
		
		//Reading sensors - will need to be set to analog sensor pins.
		left_reading = read_analog(LEFT_SENSOR); 
		right_reading = read_analog(RIGHT_SENSOR);
		std::cout << "Left: " << left_reading << " Right: " << right_reading << std::endl;
		double sensor_difference = (right_reading - left_reading) * mazeFactor;

		
		// If left reading is greater (further away), turn right.
		if(left_reading > right_reading){
			//sensor_difference will be negative.
			set_motor(LEFT_MOTOR, 50-1*sensor_difference);
			set_motor(RIGHT_MOTOR, 50+sensor_difference);
		}
		
		// If right reading is greater, turn left.
		else if(right_reading > left_reading) {		
			//sensor_difference will be positive.
			set_motor(LEFT_MOTOR, 50-1*sensor_difference);
			set_motor(RIGHT_MOTOR, 50+sensor_difference);
		}
		
		// Otherwise, continue forward.
		
		else{
			set_motor(LEFT_MOTOR, 50);
			set_motor(RIGHT_MOTOR, 50);
		}
		//Wait for a lil'
		sleep1(0, 50000);
	}
return 0;
}
