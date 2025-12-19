#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "pros/adi.hpp"

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
pros::MotorGroup leftMotors({1, 2, -3}, pros::MotorGearset::blue); // left motor group - ports 3 (reversed), 4, 5 (reversed)
pros::MotorGroup rightMotors({-4, -6, 7}, pros::MotorGearset::blue); // right motor group - ports 6, 7, 9 (reversed)

//Intake motors
pros::Motor intakeMotor1(8, pros::MotorGearset::blue); // Intake motor on port 8
pros::Motor intakeMotor2(-9, pros::MotorGearset::green); // Intake motor on port 9
pros::Motor intakeMotor3(10, pros::MotorGearset::green); // Indexer motor on port 10

// pneumatics
pros::adi::DigitalOut Descore1('A', false); // Descore Piston left on port A
pros::adi::DigitalOut Descore2('B', false); // Descore Piston right on port B
pros::adi::DigitalOut Park('C', false); // Park Piston left on port C
pros::adi::DigitalOut Pistonlift('D', false); // Intake lift Piston right on port
pros::adi::DigitalOut LTC('H', false); // LTC deploy piston on port H

//optical sensor
pros::Optical opticalSensor(13); // Optical sensor on port 13

// Inertial Sensor on port 10
pros::Imu imu(20);

// Create motor objects
// note its important to include the motor gearset otherwise the gear ratio will be incorrect
/*pros::Motor lF(1, pros::E_MOTOR_GEARSET_06); // left front motor. port 5, reversed
pros::Motor lM(2, pros::E_MOTOR_GEARSET_06); // left middle motor. port 4
pros::Motor lB(-3, pros::E_MOTOR_GEARSET_06); // left back motor. port 3, reversed
pros::Motor rF(-4, pros::E_MOTOR_GEARSET_06); // right front motor. port 6, reversed
pros::Motor rM(-6, pros::E_MOTOR_GEARSET_06); // right middle motor. port 7, reversed
pros::Motor rB(7, pros::E_MOTOR_GEARSET_06); // right back motor. port 9

// create a motor group for the left side of the drivetrain
pros::MotorGroup leftMotorsOdom({lF, lM, lB});
pros::MotorGroup rightMotorsOdom({rF, rM, rB});
// Create a new tracking wheel using the left motor group
// it's using an new 3.25 wheel
// and its distance is half the track width of the drivetrain
// distance is also negative because the left drive side is to the left of the tracking center
// if it was to the right of the tracking center, we would use a positive distance
// the rpm is 450
*/
lemlib::TrackingWheel horizontal(&leftMotors, lemlib::Omniwheel::NEW_325, -4.961101, 450);
lemlib::TrackingWheel vertical(&rightMotors, lemlib::Omniwheel::NEW_325, 4.961101, 450);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              9.922202, // 9.922202 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              450, // drivetrain rpm is 450
                              8 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
lemlib::ControllerSettings linearController(10, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            3, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            20 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(2, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             10, // derivative gain (kD)
                                             3, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

// sensors for odometry
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel
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

    /*
    horizontalEnc.reset_position();
    verticalEnc.reset_position();
*/
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

    // Set optical sensor LED to maximum brightness
    opticalSensor.set_led_pwm(100);
}
void disabled() {
    //makes sure park is activated when disabled
    if (ParkState==true){
        Park.set_value(true);
    }else{
        Park.set_value(false);
    }
}

void competition_initialize() {
}

// get a path used for pure pursuit
// this needs to be put outside a function
//ASSET(example_txt); // '.' replaced with "_" to make c++ happy

/**
 * Runs during auto
 *
 * This is an example autonomous routine which demonstrates a lot of the features LemLib has to offer
 */
void autonomous() {
      // 1️Drive forward 
    leftMotors.move_velocity(300);
    rightMotors.move_velocity(300);
	LTC.set_value(true); //deploy LTC
    pros::delay(800);

    // Stop
    leftMotors.move_velocity(0);
    rightMotors.move_velocity(0);
    pros::delay(150);

	leftMotors.move_velocity(-200);
    rightMotors.move_velocity(200);
    pros::delay(50); //turn right 90 degrees

    leftMotors.move_velocity(0);
    rightMotors.move_velocity(0);

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
    pros::delay(1500);

	// Stop intake before reversing
	intakeMotor2.move_velocity(150);
	intakeMotor3.move_velocity(150);
	pros::delay(50);


    // 5️ Move backward
    leftMotors.move_velocity(-250);
    rightMotors.move_velocity(-250);
    pros::delay(1500);

    // Stop
	Descore1.set_value(false); //deactivate descore pistons
	Descore2.set_value(false);

    leftMotors.move_velocity(0);
    rightMotors.move_velocity(0);

    // 6️ Outtake
    intakeMotor1.move_velocity(-600);
    intakeMotor2.move_velocity(-600);
    intakeMotor3.move_velocity(-600);
    pros::delay(500);

    // Stop outake
    intakeMotor1.move_velocity(0);
    intakeMotor2.move_velocity(0);
    intakeMotor3.move_velocity(0);  // 1️Drive forward
    pros::c::delay(1000);

    leftMotors.move_velocity(300);
    rightMotors.move_velocity(300);
	LTC.set_value(true); //deploy LTC
    pros::delay(2000);

    // Stop
    leftMotors.move_velocity(0);
    rightMotors.move_velocity(0);
    pros::delay(150);

	chassis.turnToHeading(90, 1000); //face forward

    // Stop
	Descore1.set_value(true); //activate descore pistons
	Descore2.set_value(true);
	pros::delay(500);

    leftMotors.move_velocity(0);
    rightMotors.move_velocity(0);
    pros::delay(150);

    //  Drive forward a little
    leftMotors.move_velocity(300);
    rightMotors.move_velocity(300);

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

void opcontrol() {
    
    while (true) {
        // =====================================================================
        // ============================ CHASSIS DRIVE ===========================
        // =====================================================================
        ChassisDrive();
       
        // =====================================================================
        // ============================ INTAKE & PNEU ===========================
        // =====================================================================

        intakeControl(); // call intake control function
        pneumaticsControl(); // call pneumatics control function
        
        pros::delay(10);
    }
}
