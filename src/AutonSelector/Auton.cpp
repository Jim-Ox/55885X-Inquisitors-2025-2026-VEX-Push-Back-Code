#include "auton.hpp"
#include "driver.hpp"
#include "main.h"

void testAuton(){	
    //chassis.turnToHeading(90, 10000);
    chassis.moveToPoint(12, 12,  3000);

    chassis.moveToPose(0,0,0,3000);

    
    chassis.swingToPoint(12, 12, lemlib::DriveSide::RIGHT, 3000);

    chassis.swingToHeading(270, lemlib::DriveSide::LEFT, 3000);

    chassis.moveToPose(0, 0, 0, 3000);
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