#include "auton.hpp"
#include "driver.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include <algorithm>

void testAuton(){	
    //chassis.turnToHeading(90, 10000);
    chassis.moveToPoint(12, 12,  3000);

    chassis.moveToPose(0,0,0,3000);

    
    chassis.swingToPoint(12, 12, lemlib::DriveSide::RIGHT, 3000);

    chassis.swingToHeading(270, lemlib::DriveSide::LEFT, 3000);

    chassis.moveToPose(0, 0, 0, 3000);
}

void rightRed(){
    //mechanism prep
    Descore1.set_value(true);
    Intake(600);

    //intake center 3 blocks
	chassis.moveToPose(6, 30, 0, 1000);
    chassis.waitUntilDone();
    chassis.moveToPose(6, 35, 0, 2000, {.maxSpeed = 60});
    LTC.set_value(true);
    pros::delay(1000);
    LTC.set_value(false);


    //approach center lower goal
    chassis.moveToPose(4, 50, 315, 500, {.forwards = true});
    chassis.waitUntilDone();

    //score blocks
    Pistonlift.set_value(true);
    Intake(-600);
    pros::delay(3000);

    //going to loader
    chassis.moveToPose(30, 15, 180, 1000);
    chassis.waitUntilDone();
    Intake(600);
    LTC.set_value(true);
    Pistonlift.set_value(false);

    //unload blocks
    chassis.moveToPose(30,5,180,3000, {.forwards = true});
    chassis.waitUntilDone();
    
    //going to side goal to score
    chassis.moveToPose(30, 35, 180, 1000, {.forwards = false});
    LTC.set_value(false);
    Descore1.set_value(false);
    pros::delay(1500);

    //move to a better position to avoid side goal for descore
    chassis.moveToPose(15, 25, 255, 1000);

    //prepare to use wings
    Descore1.set_value(true);
    chassis.moveToPose(23, 45, 180, 1000, {.forwards = false});
    chassis.waitUntilDone();

    //pushes blocks into side goal control zone
    Descore1.set_value(false);
    chassis.swingToPoint(5, 52.5, lemlib::DriveSide::LEFT, 1000);
}

void leftRed(){
    //mechanism prep
    Descore1.set_value(true);
    Intake(600);

    //intake center 3 blocks
	chassis.moveToPose(6, 30, 0, 2000);
    chassis.waitUntilDone();
    chassis.moveToPose(6, 35, 0, 2000, {.maxSpeed = 60});
    LTC.set_value(true);
    pros::delay(1000);
    LTC.set_value(false);

    //approach center lower goal
    chassis.moveToPose(4, 50, 315, 1000, {.forwards = false});
    chassis.waitUntilDone();

    //score blocks
    Pistonlift.set_value(true);
    Intake(-600);
    pros::delay(3000);

    //going to loader
    chassis.moveToPose(-30, 15, 180, 1000);
    chassis.waitUntilDone();
    Intake(600);
    LTC.set_value(true);
    Pistonlift.set_value(false);

    //unload blocks
    chassis.moveToPose(30,5,180,3000, {.forwards = true});
    chassis.waitUntilDone();
    
    //going to side goal to score
    chassis.moveToPose(-30, 35, 180, 2000, {.forwards = false});
    Descore1.set_value(false);
    LTC.set_value(false);
    pros::delay(1500);

    //move to a better position to avoid side goal for descore
    chassis.moveToPose(-15, 25, 225, 2000);

    //prepare to use wings
    Descore1.set_value(true);
    chassis.moveToPose(-23, 45, 180, 2000, {.forwards = false});
    chassis.waitUntilDone();

    //pushes blocks into side goal control zone
    Descore1.set_value(false);
    chassis.swingToPoint(-5, 52.5, lemlib::DriveSide::RIGHT, 2000);
}

void rightBlue(){
//    chassis.turnToHeading(90, 10000);
    chassis.moveToPoint(0, 24, 10000);
}

void leftBlue(){
	chassis.turnToHeading(360, 3000);
}

void skills(){
	chassis.moveToPoint(0, 24,  5000);
}