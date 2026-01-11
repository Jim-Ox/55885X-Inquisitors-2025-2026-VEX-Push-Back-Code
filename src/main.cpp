#include "main.h"
#include "auton.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "pros/adi.hpp"

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
pros::MotorGroup rightMotors({1, 2, 3}, pros::MotorGearset::blue); // left motor group - ports 1,2,3
pros::MotorGroup leftMotors({-4, -6, -7}, pros::MotorGearset::blue); // right motor group - ports 4,6,7 (reversed) (port 5 doesn't work)

//Intake motors
pros::Motor intakeMotor1(8, pros::MotorGearset::blue); // Intake motor on port 8
pros::Motor intakeMotor2(-9, pros::MotorGearset::green); // Intake half motor on port 9
pros::Motor intakeMotor3(10, pros::MotorGearset::green); // Indexer half motor on port 10

// pneumatics
pros::adi::DigitalOut Descore1('A', false); // Descore Piston left on port A
pros::adi::DigitalOut Descore2('B', false); // Descore Piston right on port B
pros::adi::DigitalOut Park('C', false); // Park Piston left on port C
pros::adi::DigitalOut Pistonlift('D', false); // Intake lift Piston right on port D
pros::adi::DigitalOut LTC('H', false); // LTC deploy piston on port H

//optical sensor
pros::Optical opticalSensor(13); // Optical sensor on port 13 (doesn't seem to work?)

//distance sensor
pros::Distance distanceSensor(18); // Distance sensor on port 18

// Inertial Sensor on port 20
pros::Imu imu(20);

// tracking wheels
// horizontal tracking wheel encoder. Rotation sensor, port 20, not reversed
//pros::Rotation horizontalEnc(21);
// vertical tracking wheel encoder. Rotation sensor, port 11, reversed
pros::Rotation verticalEnc(-21);
// horizontal tracking wheel. 2.75" diameter, 5.75" offset, back of the robot (negative)
//lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_275, -5);
// vertical tracking wheel. 2.75" diameter, 0.5" offset, left of the robot (negative)
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_275, 0.5);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              9.922202, // 9.922202 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              450, // drivetrain rpm is 450
                              8 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
lemlib::ControllerSettings linearController(10, // proportional gain (kP) 10
                                            0, // integral gain (kI)
                                            44, // derivative gain (kD) 15
                                            0, // anti windup
                                            0, // small error range, in inches 1
                                            0, // small error range timeout, in milliseconds 100
                                            0, // large error range, in inches 3
                                            0, // large error range timeout, in milliseconds 500
                                            0 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(9, // proportional gain (kP) 9
                                             0, // integral gain (kI)
                                             87, // derivative gain (kD) 87
                                             0, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

// sensors for odometry
lemlib::OdomSensors sensors(&vertical, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            nullptr, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);


// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    pros::lcd::initialize();  
    pros::lcd::set_text(0, "Initializing...");

    // --- Start-Up Delay ---
    pros::delay(1000);

    // --- Sensor Resets ---
    imu.reset();
    if (imu.is_calibrating()) {
        pros::lcd::set_text(0, "Calibrating IMU...");
        pros::delay(2000);
    }

    //horizontalEnc.reset_position();
    verticalEnc.reset_position();

    // --- LemLib Calibration ---
    chassis.calibrate();

    //Rotate coordinate frame so forward = +X and right = +Y
    chassis.setPose(0, 0, 0);

    pros::lcd::set_text(0, "Initialization Complete :)");

    // --- Live Odometry Display ---
    pros::Task screenTask([]() {
        while (true) {
            auto pose = chassis.getPose();
            pros::lcd::print(1, "X: %.2f", pose.x);
            pros::lcd::print(2, "Y: %.2f", pose.y);
            pros::lcd::print(3, "Theta: %.2f°", pose.theta);
            pros::delay(50);
        }
    });
    
    // --- Auton Selector ---
    AutonSelector();

    /* Set optical sensor LED to maximum brightness,
    tuned off for now due to battery drain and optical sensor not connecting properly
    */
    //opticalSensor.set_led_pwm(100);

    // Set origin based on distance sensor reading
    resetOrigin();
}

// This function is called when the robot is disabled.
void disabled() {
    //makes sure park is activated when disabled
    if (ParkState==true){
        Park.set_value(true);
    }else{
        Park.set_value(false);
    }
}

// This function is called when the robot is enabled in the field or
// during operator control.
void competition_initialize() {}

// get a path used for pure pursuit
// this needs to be put outside a function
//ASSET(example_txt); // '.' replaced with "_" to make c++ happy

/**
 * Runs during autonomous mode
 */
void autonomous() {
    runAuton(screenstate);
}

void opcontrol() {
    
    while (true) {
        //for testing auton (remove later)
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            autonomous();
        }

        // =====================================================================
        // ============================ CHASSIS DRIVE ===========================
        // =====================================================================
        //ChassisDrive(); //for arcade drive only

        // tank drive
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
        chassis.tank(leftY, rightY);
       
        // =====================================================================
        // ============================ INTAKE & PNEU ===========================
        // =====================================================================

        intakeControl(); // call intake control function
        pneumaticsControl(); // call pneumatics control function
        
        pros::delay(10);
    }
}
