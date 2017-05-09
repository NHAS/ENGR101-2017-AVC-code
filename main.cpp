#include <iostream>
# include <time.h>
#include <csignal>
# include "E101.h"
using namespace std;
#define CAMERA_WIDTH 320 //Control Resolution from Camera
#define CAMERA_HEIGHT 240 //Control Resolution from Camera

#define LEFT_MOTOR 1
#define RIGHT_MOTOR 2


bool done = false;
void betterStop(int i) { // 
	cout << "Signal killed. " << endl;
	set_motor(LEFT_MOTOR, 0);
	set_motor(RIGHT_MOTOR, 0);
	stop(0);
	close_screen_stream();
	done = true;
	
}

int main() {
	init();
	
	signal(2, betterStop);
	
	

	
	open_screen_stream();
	while(!done) {
		
		take_picture();
		
		int HalfWayThere = CAMERA_WIDTH/2;

		int LineDetect[80] = {0}; // The line detection pixels are directly in the center
		
		int TotalSidePixels = HalfWayThere-40; // The total number of pixels of the right and left side sample
		
		int Right[TotalSidePixels] = {0}; // = {0} means set all the elements of the array as 0
		int Left[TotalSidePixels] = {0};
		
		for(int i = 0; i < TotalSidePixels; i++) {
			Right[i] = get_pixel(CAMERA_HEIGHT/2, TotalSidePixels-i, 3); // Be warned this is right from the ROBOTS perspective
			set_pixel(CAMERA_HEIGHT/2,  TotalSidePixels-i, 255,0,0);
			
			Left[i] = get_pixel(CAMERA_HEIGHT/2, i+TotalSidePixels+80, 3); //+TotalSidePixels is for skipping what we already put into Right[] above. +80 is to skip the middle pixels
			set_pixel(CAMERA_HEIGHT/2,  i+TotalSidePixels+80, 255,0,0);
		}	
		
		for(int i = 0; i < 80; i++) {
			LineDetect[i] = get_pixel(CAMERA_HEIGHT/2, i+TotalSidePixels, 3); // Skip the right hand detection pixels and just have the middle 80 pixels
			set_pixel(CAMERA_HEIGHT/2+1,  i+TotalSidePixels, 0,255,0); 
		}
		
		convert_camera_to_screen();
		update_screen();
		
		
		int rightError = 0, leftError = 0; // How much whiteness and how far away it is for both right and left directions.
		
		double factor = 1;
		for(int i = 0; i < TotalSidePixels; i++) {
			rightError += (int)((double)((Right[i] > 135)? i : 0)*factor); // convert the element of the Right array to a double (number with decimal) and then scale it by a factor
			leftError += (int)((double)((Left[i] > 135)? i : 0)*factor);
		}
		
		
		
		double sum = 0;
		for(int i = 0; i < 80; i++) //For every pixel in the line detection if its above the threshold add a 1, or if its below set as 0
			sum += (LineDetect[i] > 130)? 1 : 0;

		sum = (sum/80)*100; // Percentages
		
		bool middle = (sum > 55); // If 55% of the pixels are white, then assume that the line is in the middle

		string indicator = (middle)? "ON" : "OFF";
		cout << "[" << indicator << " LINE] Right error: " << rightError << " Left error: " << leftError << endl; // Cout is a function "<<" means put this stuff in the function. Endl means new line.

	}

	return 0;
}

