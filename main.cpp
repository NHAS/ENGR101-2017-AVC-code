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

double factor = 0.009; // Scaling value to be determined by experiments
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
        int LineDetect[80] = {0}; // The line detection pixels are directly in the center
        for(int i = 0; i < 80; i++) 
                LineDetect[i] = get_pixel(CAMERA_HEIGHT/2, i+(int)(CAMERA_WIDTH/2)-40, 3); // Skip the right hand detection pixels and just have the middle 80 pixels        }

        double sum = 0;
        for(int i = 0; i < 80; i++) { //For every pixel in the line detection if its above the threshold add a 1, or if its below set as 0
                sum += (LineDetect[i] > threshold)? 1 : 0;
        }
        sum = (sum/80)*100; // Percentages
        cout << "Percentage sum: " << sum << endl;
        return (sum > 55); // If 55% of the pixels are white, then assume that the line is in the middle
}

int getRightSideErrorSignal() {
	int HalfWayThere = (int)(CAMERA_WIDTH/2);
    	int TotalSidePixels = HalfWayThere-40; // The total number of pixels of the right and left side sample

    	int RightPixels[TotalSidePixels] = {0}; // = {0} means set all the elements of the array as 0
    	for(int i = 0; i < TotalSidePixels; i++) {
		RightPixels[i] = get_pixel(CAMERA_HEIGHT/2, TotalSidePixels-i, 3); // Be warned this is right from the ROBOTS perspective
	}

	int rightError = 0;
	for(int i = 0; i < TotalSidePixels; i++) {
		rightError += (RightPixels[i] > threshold)? i : 0;
	}

	return rightError;
}

int getLeftSideErrorSignal() {
	int HalfWayThere = (int)(CAMERA_WIDTH/2);
    	int TotalSidePixels = HalfWayThere-40; // The total number of pixels of the right and left side sample

	int LeftPixels[TotalSidePixels] = {0};

	for(int i = 0; i < TotalSidePixels; i++) {
		LeftPixels[i] = get_pixel(CAMERA_HEIGHT/2, i+TotalSidePixels+80, 3); //+TotalSidePixels is for skipping what we already put into Right[] above. +80 is to skip the middle p$
	}

	int leftError = 0;
	for(int i = 0; i < TotalSidePixels; i++) {
		leftError += (LeftPixels[i] > threshold)? i : 0;
	}

	return leftError;
}


int main() {
        init();

        signal(2, betterStop);

        int right_velocity = 0;
        int left_velocity = 0;

        while(!done) {

                take_picture();

                double rightError = ((double)getRightSideErrorSignal());
                double leftError = ((double)getLeftSideErrorSignal()); // How much whiteness and how far away it is for both right and left directions.

		double error_signal = (rightError - leftError) * factor;
		
		int right_velocity = 50+error_signal;
		int left_velocity = 50-1*error_signal;
                cout << "Velocity L: " << left_velocity << " Velocity R: " << right_velocity << endl;
                set_motor(RIGHT_MOTOR, right_velocity);
                set_motor(LEFT_MOTOR, left_velocity);
                sleep1(0,5000);
        }

return 0;
}

