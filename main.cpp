#include  "E101.h"
#include <iostream>
#include <time.h>
using namespace std;

#define CAM_HEIGHT  240
#define CAM_WIDTH   320

int main() {
    init();
    
    while(true) {
        take_picture();
        char c = get_pixel(CAM_WIDTH/2, CAM_HEIGHT/2, 3);
        if( c <  50) {
          cout << "Data: " << c << endl;
          set_motor(1, 200);
          set_motor(2, 200);
          sleep1(2,0);
          
          set_motor(1, 0);
          set_motor(2, 0);
        }
    }
    return 0;
    
}
