#include <iostream>
#include <time.h>
#include <csignal>
#include <stdlib.h>
#include <chrono>
#include "E101.h"

using std::cout;
using std::endl;
using std::string;
using std::flush;
using std::chrono::high_resolution_clock;

const double CAMERA_WIDTH = 320; //Control Resolution from Camera
const double CAMERA_HEIGHT = 240; //Control Resolution from Camera

const int LEFT_MOTOR = 1;
const int RIGHT_MOTOR = 2;

double kP = 0.011; // Scaling value to be determined by experiments
const int threshold = 120;

bool done = false;

void betterstop(int x) {
	cout << "\nSignal killed. " << endl;
	set_motor(LEFT_MOTOR, 0);
	set_motor(RIGHT_MOTOR, 0);
	
	done = true;
}

bool isOnLine(int height) {
	const int numberCenterPixels = 40;
    double averagePixelWhiteness = 0;
    for(int i = 0; i < numberCenterPixels; i++)  //80 Pixels in the center are our detector
		averagePixelWhiteness += get_pixel(height, i+(CAMERA_WIDTH/2)-20, 3) > threshold)   // Go from -20 from the center to +20 of the center

	averagePixelWhiteness /= numberCenterPixels;
		
	return (averagePixelWhiteness >= threshold);  //If the value of the average greater than or = to white
}

int getRightSideErrorSignal() {
    const int TotalRightSidePixels = (int)(CAMERA_WIDTH/2)-40; //The -40 is so it doesnt read the 40 pixels that are used for line detection in the middle
	
    int rightError = 0;
    for(int i = 0; i < TotalRightSidePixels; i++) 
		rightError += ( get_pixel(CAMERA_HEIGHT/2, TotalRightSidePixels-i, 3) > threshold) ? i : 0; // Be warned this is right from the ROBOTS perspective
		
	return rightError;
}

int getLeftSideErrorSignal() {
    const int TotalLeftSidePixels = (int)(CAMERA_WIDTH/2)-40; //The -40 is so it doesnt read the 40 pixels that are used for line detection in the middle
	
	int leftError = 0;
	for(int i = 0; i < TotalLeftSidePixels; i++) 
		leftError += ( get_pixel(CAMERA_HEIGHT/2, i+TotalLeftSidePixels+80, 3) > threshold ) ? i : 0; //+TotalSidePixels is for skipping what we already put into Right[] above. +80 is to skip the middle p$
		
	return leftError;
}


int main() {
	init();
	signal(2, betterstop);
	
	connect_to_server("130.195.6.196", 1024);
	
	send_to_server("Please");
	
	char returnMessage[24] = {'\0'};
	receive_from_server(returnMessage);
	string StringReturnMessage(returnMessage);
	StringReturnMessage+="Please";

	for(int i = 0; i < StringReturnMessage.size(); i++) {
		returnMessage[i] = StringReturnMessage[i];
	}

	send_to_server(returnMessage);
	
	sleep1(1,0);

	int left_velocity = 0;
	int right_velocity = 0;
	while(!done) {
		take_picture();
				double rightError = ((double)getRightSideErrorSignal());
				double leftError  = ((double)getLeftSideErrorSignal()); 

				if(!isOnLine(CAMERA_HEIGHT/2) && rightError <= 0 && leftError <= 0) { // If we're off the line and cant see the line
						cout << "Lost line" << endl;
						cout << "\tTurning " << flush;
						if(left_velocity < right_velocity) { //If we were last turning left
							cout << "Left to find it." << endl;
							set_motor(LEFT_MOTOR, -60);
							set_motor(RIGHT_MOTOR, -30);
							sleep1(0,2000);
						} else { // If we were last turning right
							cout << "Right to find it." << endl;
							set_motor(LEFT_MOTOR, -30);
							set_motor(RIGHT_MOTOR, -60);
							sleep1(0,2000);
						}
					continue; // Skip the rest of the instructions in this loop
				}

				if(rightError > 3500 && leftError > 3500 && isOnLine(CAMERA_HEIGHT/2+40)) { 
					set_motor(RIGHT_MOTOR, 60);
					set_motor(LEFT_MOTOR, 20);
					
					sleep1(0,500000);
					continue;
				}

				double error_signal = (rightError - leftError);
				double proportional_signal = error_signal*kP;

				int final_signal = proportional_signal;
				right_velocity = 80+final_signal;
				left_velocity = 80-1*final_signal;

				set_motor(RIGHT_MOTOR, right_velocity);
				set_motor(LEFT_MOTOR, left_velocity);   



	}
	stop(0);
return 0;
}
