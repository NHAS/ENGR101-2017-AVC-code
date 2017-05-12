#include <iostream>
#include <time.h>
#include <csignal>
#include <stdlib.h>
#include <chrono>
#include "E101.h"

using std::cout;
using std::endl
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
void betterStop(int i) { 
        cout << "Signal killed. " << endl;
        set_motor(LEFT_MOTOR, 0);
        set_motor(RIGHT_MOTOR, 0);

        stop(0);
        done = true;
}

bool isOnLine() {
		const int numberCenterPixels = 80;
        double totalPixelsWhite = 0;
        for(int i = 0; i < numberCenterPixels; i++) //80 Pixels in the center are our detector
			if(get_pixel(CAMERA_HEIGHT/2, i+(int)(CAMERA_WIDTH/2)-40, 3) > threshold)  // Go from -40 from the center to +40 of the center
				totalPixelsWhite++;

        totalPixelsWhite = (totalPixelsWhite/numberCenterPixels)*100; // Percentages
        cout << "Percentage sum: " << totalPixelsWhite << endl;
        return (totalPixelsWhite > 55); // If 55% of the pixels are white, then assume that the line is in the middle
}

int getRightSideErrorSignal() {
    int TotalRightSidePixels = (int)(CAMERA_WIDTH/2)-40; //The -40 is so it doesnt read the 40 pixels that are used for line detection in the middle
	
	int rightError = 0;
    for(int i = 0; i < TotalSidePixels; i++) {
		rightError += ( get_pixel(CAMERA_HEIGHT/2, TotalSidePixels-i, 3) > threshold) ? i : 0; // Be warned this is right from the ROBOTS perspective
	}

	return rightError;
}

int getLeftSideErrorSignal() {
    int TotalLeftSidePixels = (int)(CAMERA_WIDTH/2)-40; //The -40 is so it doesnt read the 40 pixels that are used for line detection in the middle
	
	int leftError = 0;
	for(int i = 0; i < TotalSidePixels; i++) {
		leftError += ( get_pixel(CAMERA_HEIGHT/2, i+TotalSidePixels+80, 3) > threshold ) ? i : 0; //+TotalSidePixels is for skipping what we already put into Right[] above. +80 is to skip the middle p$
	}

	return leftError;
}


int main() {
        init();

        signal(2, betterStop);
		
		time_point startTime, endTime; // Used for calculating kD
		
		double previous_error = 0;
        while(!done) {

			take_picture();

			double rightError = ((double)getRightSideErrorSignal());
			double leftError  = ((double)getLeftSideErrorSignal()); 

			double error_signal = (rightError - leftError);

			endTime = high_resolution_clock::now();
			
			duration<double> time_span = duration_cast<duration<double>>(endTime - startTime);
			double derivative_signal   =  (error_signal-previous_error/time_span.count())*kD;
			
			startTime = high_resolution_clock::now();
			previous_error = error_signal;

			double proportional_signal = error_signal*kP;
			
			int final_signal = proportional_signal + derivative_signal;
			int right_velocity = 60+error_signal;
			int left_velocity = 60-1*error_signal;
			
			
			cout << "Velocity L: " << left_velocity << " Velocity R: " << right_velocity << endl;
			set_motor(RIGHT_MOTOR, right_velocity);
			set_motor(LEFT_MOTOR, left_velocity);   
        }

return 0;
}

