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

const int DEF_SPD = 50; // Default speed for motors
const int THRESHOLD = 90; // Threshold value for proximity to left wall - Change with testing.

double mazeFactor = 0.15; // To be changed with testing.

bool mazeDone = false;


int main(){
	init();
	
	//Variables for the sensor readings.
	int left_reading = 0;
	int right_reading = 0;
	//int mid_reading = 0;

	while(!mazeDone){ //Looping whilst 'done' condition is not met.
		
		//Reading sensors
		left_reading = read_analog(LEFT_SENSOR); 
		right_reading = read_analog(RIGHT_SENSOR);
		
		std::cout << "Left: " << left_reading << " Right: " << right_reading << std::endl; //Printing sensor values.
		
		double wall_distance = (THRESHOLD - right_reading) * mazeFactor;

			
		// If right reading is too great, turn left.
		if(right_reading > THRESHOLD){
			
			set_motor(LEFT_MOTOR, DEF_SPD + 1*wall_distance);
			set_motor(RIGHT_MOTOR, DEF_SPD - wall_distance);
		}
		
		// If right reading too small, turn right.
		else if(right_reading < THRESHOLD) {			
			set_motor(LEFT_MOTOR, DEF_SPD - 1*wall_distance);
			set_motor(RIGHT_MOTOR, DEF_SPD + wall_distance);
		}
		
		// Otherwise, continue forward.
		else{
			set_motor(LEFT_MOTOR, DEF_SPD);
			set_motor(RIGHT_MOTOR, DEF_SPD);
		}
		//Wait for a lil'
		sleep1(0, 5000);
	}
return 0;
}
