#include <stdio.h>
#include <time.h>
#include "E101.h"

int main(){
	init();
	
	//Variables for the sensor readings.
	int left_sensor_reading;
	int right_sensor_reading;
	
	double sensor_difference;
	
	bool mazeDone = false;
	

	while(!mazeDone){ //Looping whilst 'done' condition is not met.
		//Reading sensors.
		left_sensor_reading = read_analog(1);
		right_sensor_reading = read_analog(2);
		
		// If left reading is greater (further away), turn left.
		if(left_sensor_reading > right_sensor_reading){
			
			sensor_difference = (left_sensor_reading - right_sensor_reading) * factor;
			
			set_motor(LEFT_MOTOR, 50+sensor_difference);
			set_motor(RIGHT_MOTOR, 50-1*sensor_difference);
		}
		// If right reading is greater, turn right.
		else if(right_sensor_reading > left_sensor_reading){
			
			sensor_difference = (right_sensor_reading - left_sensor_reading) * factor;
			
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
