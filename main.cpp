#include <iostream>
#include <stdlib.h>
#include <chrono>
#include "E101.h"

using std::cout;
using std::endl;
using std::string;
using std::flush;

const double CAMERA_WIDTH = 320; //Control Resolution from Camera
const double CAMERA_HEIGHT = 240; //Control Resolution from Camera

const int LEFT_MOTOR = 1;
const int RIGHT_MOTOR = 2;

double kP = 0.011; // Scaling value to be determined by experiments
const int threshold = 120;

//////////////////////////////// Helper functions ///////////////////////////////////////

bool isOnLine(int height) {
	const int numberCenterPixels = 40;
    double averagePixelWhiteness = 0;
    for(int i = 0; i < numberCenterPixels; i++)  //80 Pixels in the center are our detector
		averagePixelWhiteness += get_pixel(height, i+(CAMERA_WIDTH/2)-20, 3);   // Go from -20 from the center to +20 of the center

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
		leftError += ( get_pixel(CAMERA_HEIGHT/2, i+TotalLeftSidePixels+80, 3) > threshold ) ? i : 0; //+TotalSidePixels is for skipping what we already put into Right[] above. +80 is to skip the middle 
		
	return leftError;
}

int getTopErrorSignal() {
	int topError = 0;
	for(int i = 0; i < 150; i++) {
		topError += ( get_pixel(150, CAMERA_WIDTH/2-75+i, 3) > threshold ) ? i : 0;
	}
	return topError;
}

//////////////////////////////// Helper functions ///////////////////////////////////////



void QuandrantOne() {
	cout << "Running quandrant 1... " << flush;
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
	cout << "Done!" << endl;
}

void QuandrantTwo() {
	
	cout << "Running quandrant 2..." << flush;
	int left_velocity = 0;
	int right_velocity = 0;
	while(true) {
		take_picture();
				double rightError = ((double)getRightSideErrorSignal());
				double leftError  = ((double)getLeftSideErrorSignal()); 

				if(!isOnLine(CAMERA_HEIGHT/2) && rightError <= 0 && leftError <= 0) { // If we're off the line and cant see the line
						
						if(left_velocity < right_velocity) { //If we were last turning left
							
							set_motor(LEFT_MOTOR, -60);
							set_motor(RIGHT_MOTOR, -30);
							sleep1(0,2000);
						} else { // If we were last turning right
							
							set_motor(LEFT_MOTOR, -30);
							set_motor(RIGHT_MOTOR, -60);
							sleep1(0,2000);
						}
					continue; // Skip the rest of the instructions in this loop
				}

				if(rightError > 1500 && leftError > 1500) {// If both are high values then its most probably a junction
					break; // Die out of quad 2
				}
				
				double error_signal = (rightError - leftError);
				double proportional_signal = error_signal*kP;

				int final_signal = proportional_signal;
				right_velocity = 80+final_signal;
				left_velocity = 80-1*final_signal;

				set_motor(RIGHT_MOTOR, right_velocity);
				set_motor(LEFT_MOTOR, left_velocity);   
	}
	
	cout << "Done!" << endl;
}


void QuandrantThree() {
	cout << "Running Quandrant 3..." << endl;
	int right_velocity = 0;
	int left_velocity = 0;
	const double kP = 0.011;	
	while(true) {
		take_picture();

				
			        double rightError = ((double)getRightSideErrorSignal());
                                double leftError  = ((double)getLeftSideErrorSignal());


				if(getTopErrorSignal() < 2 && rightError > 1500 && leftError > 1500) {
					rightError = 0;
					leftError = 2000;
				}


                                double error_signal = (rightError - leftError);
                                double proportional_signal = error_signal*kP;

                                int final_signal = proportional_signal;
                                right_velocity = 80+final_signal;
                                left_velocity = 80-1*final_signal;

                                set_motor(RIGHT_MOTOR, right_velocity);
                                set_motor(LEFT_MOTOR, left_velocity);

				
	}
	
	
	cout << "Done!" << endl;
}

void QuandrantFour() {
	cout << "Quandrant four not implemented" << endl;
}


int main() {
	init();
	
	QuandrantOne();
	QuandrantTwo();
	QuandrantThree();
	QuandrantFour();

	
	stop(0);
return 0;
}
