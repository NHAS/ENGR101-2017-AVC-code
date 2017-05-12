#include <iostream>
#include <time.h>
#include <csignal>
#include <stdlib.h>
#include "E101.h"
using namespace std;

const double CAMERA_WIDTH = 320; //Control Resolution from Camera
const double CAMERA_HEIGHT = 240; //Control Resolution from Camera

const int LEFT_MOTOR = 1;
const int RIGHT_MOTOR = 2;

double factor = 0.011; // Scaling value to be determined by experiments
const int threshold = 120;

bool done = false;
void betterStop(int i) { 
        cout << "Signal killed. " << endl;
        set_motor(LEFT_MOTOR, 0);
        set_motor(RIGHT_MOTOR, 0);
        close_screen_stream();

        stop(0);
        done = true;
}

bool isOnLine() {
        double sum = 0;
        for(int i = 0; i < 80; i++) //80 Pixels in the center are our detector
			if(get_pixel(CAMERA_HEIGHT/2, i+(int)(CAMERA_WIDTH/2)-40, 3) > threshold)  // Go from -40 from the center to +40 of the center
				sum++;

        sum = (sum/80)*100; // Percentages
        cout << "Percentage sum: " << sum << endl;
        return (sum > 55); // If 55% of the pixels are white, then assume that the line is in the middle
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

        while(!done) {

			take_picture();

			double rightError = ((double)getRightSideErrorSignal());
			double leftError = ((double)getLeftSideErrorSignal()); // How much whiteness and how far away it is for both right and left directions.

			double error_signal = (rightError - leftError) * factor;
		
			int right_velocity = 60+error_signal;
			int left_velocity = 60-1*error_signal;
			cout << "Velocity L: " << left_velocity << " Velocity R: " << right_velocity << endl;
			set_motor(RIGHT_MOTOR, right_velocity);
			set_motor(LEFT_MOTOR, left_velocity);   
        }

return 0;
}

