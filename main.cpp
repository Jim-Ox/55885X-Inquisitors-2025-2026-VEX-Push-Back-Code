#include "main.h"
#include "lemlib/api.hpp" // LemLib functionality
#include <map>

// --------- CONTROLLER ---------
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// --------- DRIVE MOTORS ---------
pros::MotorGroup leftMotors({1, -2, 3}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({-11, 12, -13}, pros::MotorGearset::blue);

// --------- INTAKE MOTORS ---------
// Intake motors for stage 1 and stage 2
pros::Motor intakeStage1(8, pros::E_MOTOR_GEARSET_06, false); // normal direction
pros::Motor intakeStage2(9, pros::E_MOTOR_GEARSET_06, true);  // reversed direction
// --------- SENSORS ---------
pros::Imu imu(10);
pros::Rotation horizontalEnc(20);
pros::Rotation verticalEnc(-11);

// --------- TRACKING WHEELS ---------
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_275, -5.75);
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_275, -2.5);

// --------- DRIVETRAIN ---------
lemlib::Drivetrain drivetrain(
    &leftMotors,
    &rightMotors,
    10,               // Track width in inches
    lemlib::Omniwheel::NEW_4,
    360,              // Max RPM
    2                 // Horizontal drift
);

// --------- PID CONTROLLERS ---------
lemlib::ControllerSettings linearController(10, 0, 3, 3, 1, 100, 3, 500, 20);
lemlib::ControllerSettings angularController(2, 0, 10, 3, 1, 100, 3, 500, 0);

// --------- ODOMETRY ---------
lemlib::OdomSensors sensors(&vertical, nullptr, &horizontal, nullptr, &imu);

// --------- CHASSIS ---------
lemlib::Chassis chassis(
    drivetrain,
    linearController,
    angularController,
    sensors,
    nullptr,
    nullptr
);

// --------- CONFIGURABLE SETTINGS ---------
const int joystickDeadzone = 7;
const int rampStep = 8;
const double imuCorrectionGain = 0.05;
const double expoExponent = 1.5; // Exponential drive strength

// --------- HELPER FUNCTIONS ---------
inline int applyExponential(int input, double exponent = expoExponent) {
    double sign = (input >= 0) ? 1.0 : -1.0;
    double absInput = abs(input);
    double output = pow(absInput / 127.0, exponent) * 127.0;
    return static_cast<int>(output * sign);
}

// --------- INITIALIZATION ---------
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

    horizontalEnc.reset_position();
    verticalEnc.reset_position();

    // --- LemLib Calibration ---
    chassis.calibrate();

    //Rotate coordinate frame so forward = +X and right = +Y
    chassis.setPose(0, 0, -90);

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
}
void disabled() {}
void competition_initialize() {}

// ------------ DRIVER CONTROL -----------//
void opcontrol() {
    static int lastLeft = 0;
    static int lastRight = 0;

    while (true) {
        // --- READ JOYSTICKS ---
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

        // --- DEADZONE ---
        if (abs(leftY) < joystickDeadzone) leftY = 0;
        if (abs(rightY) < joystickDeadzone) rightY = 0;

        // --- RAMP FILTER ---
        leftY = lastLeft + std::clamp(leftY - lastLeft, -rampStep, rampStep);
        rightY = lastRight + std::clamp(rightY - lastRight, -rampStep, rampStep);
        lastLeft = leftY;
        lastRight = rightY;

        // --- SPEED MODES ---
        double speedMultiplier = 1.0;
       if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) 
    speedMultiplier = 1.27;     // Turbo priority
else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) 
    speedMultiplier = 0.5;      // Precision
else 
    speedMultiplier = 1.0;      // Normal

        // --- APPLY EXPONENTIAL CURVE ---
        leftY = applyExponential(leftY);
        rightY = applyExponential(rightY);

        // --- DRIVE TANK ---
        chassis.tank(leftY * speedMultiplier, rightY * speedMultiplier);

        // --- INTAKE CONTROL ---
        // Intake motors: intakeStage1 (port 8), intakeStage2 (port 9)
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            // Intake in
            intakeStage1.move(127);
            intakeStage2.move(-127); // opposite direction
        } 
        else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            // Outtake
            intakeStage1.move(-127);
            intakeStage2.move(127);
        } 
        else {
            // Stop intake
            intakeStage1.brake();
            intakeStage2.brake();
        }

        // --- TELEMETRY ---
        pros::lcd::print(0, "Left: %d | Right: %d", leftY, rightY);
        pros::lcd::print(1, "Speed Mode: %s", 
            controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) ? "TURBO" :
            (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1) ? "Precision" : "Normal"));
        pros::lcd::print(2, "Intake: %s",
            controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) ? "In" :
            (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2) ? "Out" : "Stopped"));

        pros::delay(10);
    }
}

// --------- AUTONOMOUS ---------
void autonomous() {}
