#include "auton.hpp"
#include "driver.hpp"
#include "main.h"

/* Autonomous Selector screen page
0 - live odometry display
1 - Test Auton
2 - Right Red
3 - Left Red
4 - Right Blue
5 - Left Blue
6 - Skills
*/
int screenstate = 0;

void runSelectedAuton(){
    if (last_state == 1){
        testAuton();
    }
    else if (last_state == 2){
        rightRed();
    }
    else if (last_state == 3){
        leftRed();
    }
    else if (last_state == 4){
        rightBlue();
    }
    else if (last_state == 5){
        leftBlue();
    }
    else if (last_state == 6){
        skills();
    }
}

void testAuton(){	
    //chassis.turnToHeading(90, 10000);
    chassis.moveToPoint(12, 12,  3000);

    chassis.moveToPose(0,0,0,3000);

    
    chassis.swingToPoint(12, 12, lemlib::DriveSide::RIGHT, 3000);

    chassis.swingToHeading(270, lemlib::DriveSide::LEFT, 3000);

    chassis.moveToPose(-2, 0, 0, 3000);
}

void rightRed(){
    chassis.turnToHeading(90, 3000);
}

void leftRed(){
	chassis.turnToHeading(180, 3000);
}

void rightBlue(){
	chassis.turnToHeading(270, 3000);
}

void leftBlue(){
	chassis.turnToHeading(360, 3000);
}

void skills(){
	chassis.moveToPoint(0, 24,  5000);
}