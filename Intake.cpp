#include "driver.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include <sys/_intsup.h>

//anti jam intake code
void anti_jamIntake(){
//if intake motor is running forward and torque is above threshold
    if((intakeMotor1.get_power() >= 11) && (abs(intakeMotor1.get_actual_velocity()) <= 50) && (DescoreState == false)){
        intakeMotor1.move_velocity(-600); //reverse intake motors
        intakeMotor2.move_velocity(-600);
        intakeMotor3.move_velocity(-600);
        pros::delay(500); //for 500 ms
    } else if ((intakeMotor2.get_power() >= 5.5) && (abs(intakeMotor2.get_actual_velocity()) <= 25) && (DescoreState == false)) {
        intakeMotor1.move_velocity(-600); //reverse intake motors
        intakeMotor2.move_velocity(-600);
        intakeMotor3.move_velocity(-600);
        pros::delay(500); //for 500 ms
    } else if ((intakeMotor3.get_power() >= 5.5) && (abs(intakeMotor3.get_actual_velocity()) <= 25) && (DescoreState == false)) {
        intakeMotor1.move_velocity(-600); //reverse intake motors
        intakeMotor2.move_velocity(-600);
        intakeMotor3.move_velocity(-600);
        pros::delay(500); //for 500 ms
    } else {
        return; //else exit function
    }
    pros::delay(10);
}

// function that stops 3rd stage intake while loading blocks into the intake
void indexerControl(){
    //checks if descore pistons are active and R1 is pressed (intake is running)

    if ((DescoreState == true) && (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))) {
        //slows 2nd and 3rd stage motor
        intakeMotor2.move_velocity(64);
        intakeMotor3.move_velocity(64);
    } else {
        //otherwise run normally
        intakeMotor3.move_velocity(600);
    }
    pros::delay(10);
}

// intake motor controls
void intakeControl(){
    ColorSortFunction(); // call anti jam intake function     
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        intakeMotor1.move_velocity(600);
        intakeMotor2.move_velocity(600);
        indexerControl(); // call indexer control function
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        intakeMotor1.move_velocity(-600);
        intakeMotor2.move_velocity(-600);
        intakeMotor3.move_velocity(-600);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        intakeMotor1.move_velocity(600);
        intakeMotor2.move_velocity(600);
        intakeMotor3.move_velocity(-600);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        intakeMotor1.move_velocity(-600);
        intakeMotor2.move_velocity(-600);
        intakeMotor3.move_velocity(600);
    } else {
        intakeMotor1.move_velocity(0);
        intakeMotor2.move_velocity(0);
        intakeMotor3.move_velocity(0);
    }
    pros::delay(10);
}

void IntakeSpin(int velocity){
    anti_jamIntake(); // call anti jam intake function

    intakeMotor1.move_velocity(velocity);
    intakeMotor2.move_velocity(velocity);
    intakeMotor3.move_velocity(velocity);
    pros::delay(10);

}