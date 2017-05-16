#include <iostream>
#include <time.h>
#include <csignal>
#include <stdlib.h>
#include <chrono>
#include "E101.h"

using std::cout;
using std::endl;
using std::flush;
using std::chrono::high_resolution_clock;

const double CAMERA_WIDTH = 320; //Control Resolution from Camera
const double CAMERA_HEIGHT = 240; //Control Resolution from Camera

const int LEFT_MOTOR = 1;
const int RIGHT_MOTOR = 2;

double kP = 0.011; // Scaling value to be determined by experiments
double kD = 5.0;
const int threshold = 120;

bool done = false;

void betterstop(int x) {
	cout << "\nSignal killed. " << endl;
	set_motor(LEFT_MOTOR, 0);
	set_motor(RIGHT_MOTOR, 0);
	
	stop(0);
	done = true;
}

bool isOnLine() {
	const int numberCenterPixels = 40;
        double totalPixelsWhite = 0;
        for(int i = 0; i < numberCenterPixels; i++) { //80 Pixels in the center are our detector
			if(get_pixel(CAMERA_HEIGHT/2, i+(int)(CAMERA_WIDTH/2)-20, 3) > threshold) {  // Go from -40 from the center to +40 of the center
				totalPixelsWhite++;
			} 
	}
        return (totalPixelsWhite >  10); // If 55% of the pixels are white, then assume that the line is in the middle
}

int getRightSideErrorSignal() {
    int TotalRightSidePixels = (int)(CAMERA_WIDTH/2)-40; //The -40 is so it doesnt read the 40 pixels that are used for line detection in the middle
	
    int rightError = 0;
    for(int i = 0; i < TotalRightSidePixels; i++) {
		rightError += ( get_pixel(CAMERA_HEIGHT/2, TotalRightSidePixels-i, 3) > threshold) ? i : 0; // Be warned this is right from the ROBOTS perspective
    		set_pixel(CAMERA_HEIGHT/2, TotalRightSidePixels-i, 255, 0,0);
	}

	return rightError;
}

int getLeftSideErrorSignal() {
    int TotalLeftSidePixels = (int)(CAMERA_WIDTH/2)-40; //The -40 is so it doesnt read the 40 pixels that are used for line detection in the middle
	
	int leftError = 0;
	for(int i = 0; i < TotalLeftSidePixels; i++) {
		leftError += ( get_pixel(CAMERA_HEIGHT/2, i+TotalLeftSidePixels+80, 3) > threshold ) ? i : 0; //+TotalSidePixels is for skipping what we already put into Right[] above. +80 is to skip the middle p$
		set_pixel(CAMERA_HEIGHT/2, i+TotalLeftSidePixels+80, 255, 0, 0);
	}

	return leftError;
}


int main() {
        signal(2, betterstop);
	init();
	
	int left_velocity = 0;
	int right_velocity = 0;
	while(!done) {
		take_picture();
				
				double rightError = ((double)getRightSideErrorSignal());
				double leftError  = ((double)getLeftSideErrorSignal()); 

				if(!isOnLine() && rightError <= 0 && leftError <= 0) {
						if(left_velocity > right_velocity) { // We were turning right;
							set_motor(LEFT_MOTOR, -50);
							set_motor(RIGHT_MOTOR, -30);
							sleep1(0,5000);
						} else { // We were turning left
							set_motor(LEFT_MOTOR, -30);
							set_motor(RIGHT_MOTOR, -50);
							sleep(0,5000);
						}
						continue;
				}


				double error_signal = (rightError - leftError);
				double proportional_signal = error_signal*kP;

				int final_signal = proportional_signal;
				right_velocity = 60+final_signal;
				left_velocity = 60-1*final_signal;
				cout << isOnLine()  << " left: " << left_velocity << " right: " << right_velocity << endl; 

				set_motor(RIGHT_MOTOR, right_velocity);
				set_motor(LEFT_MOTOR, left_velocity);   



	}

return 0;
}
