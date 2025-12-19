#include "driver.hpp"
#include "main.h"

void test(){
	
}

void rightRed(){

    // 1️Drive forward 
	chassis.turnToHeading(0, 1000); //face forward
    leftMotors.move_velocity(300);
    rightMotors.move_velocity(300);
	LTC.set_value(true); //deploy LTC
    pros::delay(2000);

    // Stop
    leftMotors.move_velocity(0);
    rightMotors.move_velocity(0);
    pros::delay(150);

	chassis.turnToHeading(75, 1000); //face forward

    // Stop
	Descore1.set_value(true); //activate descore pistons
	Descore2.set_value(true);
	pros::delay(500);

    leftMotors.move_velocity(0);
    rightMotors.move_velocity(0);
    pros::delay(150);

    //  Drive forward a little
    leftMotors.move_velocity(200);
    rightMotors.move_velocity(200);

    // 4️ Intake
    intakeMotor1.move_velocity(600);
    intakeMotor2.move_velocity(600);
    intakeMotor3.move_velocity(600);
    pros::delay(3000);

	// Stop intake before reversing
	intakeMotor2.move_velocity(300);
	intakeMotor3.move_velocity(300);
	pros::delay(50);


    // 5️ Move backward
	chassis.turnToHeading(90, 1000); //face backward
    leftMotors.move_velocity(-250);
    rightMotors.move_velocity(-250);
    pros::delay(4000);

    // Stop
	Descore1.set_value(false); //deactivate descore pistons
	Descore2.set_value(false);

    leftMotors.move_velocity(0);
    rightMotors.move_velocity(0);

    // 6️ Outtake
    intakeMotor1.move_velocity(-600);
    intakeMotor2.move_velocity(-600);
    intakeMotor3.move_velocity(-600);
    pros::delay(600);

    // Stop outake
    intakeMotor1.move_velocity(0);
    intakeMotor2.move_velocity(0);
    intakeMotor3.move_velocity(0);
	}

void leftRed(){
	// Code here
}

void rightBlue(){
	// Code here
}

void leftBlue(){
	// Code here
}

void skills(){
	// Code here
}

void runSelectedAuton(){
    
}