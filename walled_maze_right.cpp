#include <iostream>
#include <time.h>
#include <csignal>
#include <stdlib.h>
#include <stdio.h>
#include "E101.h"

const int LEFT_SENSOR = 0;
const int RIGHT_SENSOR = 1;
//const int MID_SENSOR = 2;
	
const int LEFT_MOTOR = 1;
const int RIGHT_MOTOR = 2;

const int DEF_SPD = 50; // Default speed for motors
//const int THRESHOLD = 80; // Threshold value to allow turning - Change with testing.

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
		mid_reading = read_analog(MID_SENSOR);
		
		std::cout << "Left: " << left_reading << " Right: " << right_reading << std::endl; //Printing sensor values.
		
		double sensor_difference = (right_reading - left_reading) * mazeFactor;
		
		// If the front value is lower than a threshold level.
		//if(mid_reading < THRESHOLD){
			
			// If left reading is greater (further away), turn left.
			if(left_reading > right_reading){
			
				set_motor(LEFT_MOTOR, DEF_SPD + 1*sensor_difference);
				set_motor(RIGHT_MOTOR, DEF_SPD - sensor_difference);
			}
		
			// If right reading is greater, turn right.
			else if(right_reading > left_reading) {			
				set_motor(LEFT_MOTOR, DEF_SPD - 1*sensor_difference);
				set_motor(RIGHT_MOTOR, DEF_SPD + sensor_difference);
			}
		//}
		
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
