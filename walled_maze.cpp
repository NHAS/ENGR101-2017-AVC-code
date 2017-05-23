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
const int THRESHOLD = 310; // Threshold value for proximity to left wall - Change with testing.

double mazeFactor = 0.15; // To be changed with testing.

bool mazeDone = false;


int main(){
	init();
	
	//Variables for the sensor readings.
	int left_reading = 0;
	int right_reading = 0;
	
	while(!mazeDone){ //Looping whilst 'done' condition is not met.
		
		//Reading sensors
		left_reading = read_analog(LEFT_SENSOR); 
		right_reading = read_analog(RIGHT_SENSOR);
		
		std::cout << "Left: " << left_reading << " Right: " << right_reading << std::endl; //Printing sensor values.
		
		double wall_distance = (THRESHOLD - right_reading) * mazeFactor; //Distance from the wall multiplied by a constant.
			
		// If closer to the right wall, turn left.
		if(right_reading > THRESHOLD){
			if(right_reading > THRESHOLD+100){
				set_motor(LEFT_MOTOR, DEF_SPD + 1*wall_distance);
				set_motor(RIGHT_MOTOR, DEF_SPD - wall_distance);
				sleep1(0, 300000);
			}
			else{
			//wall_distance will be positive, so add to left wheel and remove from right.
			set_motor(LEFT_MOTOR, DEF_SPD + 1*wall_distance);
			set_motor(RIGHT_MOTOR, DEF_SPD - wall_distance);
			}
		}
		
		// If closer to the left wall, turn right.
		else if(right_reading < THRESHOLD) {	
			if(right_reading < THRESHOLD-165){
				sleep1(0, 250000);
				set_motor(LEFT_MOTOR, DEF_SPD + 1*wall_distance);
				set_motor(RIGHT_MOTOR, DEF_SPD - wall_distance);
				sleep1(0, 700000);
			}
			else{
			//wall_distance will be negative, so add to left wheel and remove from right.
			set_motor(LEFT_MOTOR, DEF_SPD + 1*wall_distance);
			set_motor(RIGHT_MOTOR, DEF_SPD - wall_distance);
			}
		}
		
		// Otherwise, continue forward.
		else{
			set_motor(LEFT_MOTOR, DEF_SPD);
			set_motor(RIGHT_MOTOR, DEF_SPD);
		}
		//Wait for a lil'
		//sleep1(0, 5000);
	}
return 0;
}
