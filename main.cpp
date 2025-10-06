#include "main.h"
#include "lemlib/api.hpp" // LemLib functionality

// --------- CONTROLLER ---------
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// --------- MOTORS ---------
pros::MotorGroup leftMotors({-5, 4, -3}, pros::MotorGearset::blue);
pros::MotorGroup rightMotors({6, -9, 7}, pros::MotorGearset::blue);

// --------- SENSORS ---------
pros::Imu imu(10);

// Tracking wheels
pros::Rotation horizontalEnc(20);
pros::Rotation verticalEnc(-11);
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_275, -5.75);
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_275, -2.5);

// --------- DRIVETRAIN ---------
lemlib::Drivetrain drivetrain(
    &leftMotors,
    &rightMotors,
    10,               // Track width
    lemlib::Omniwheel::NEW_4,
    360,              // Max RPM
    2                 // Horizontal drift
);

// --------- CONTROLLERS (PID) ---------
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
    nullptr, // We will use lambda-based exponential drive instead of LemLib curves
    nullptr
);

// --------- INITIALIZATION ---------
void initialize() {
    pros::lcd::initialize();  // Brain LCD
    chassis.calibrate();       // Calibrate IMU and odometry

    // Thread to display robot position
    pros::Task screenTask([]() {
        while (true) {
            auto pose = chassis.getPose();
            pros::lcd::print(0, "X: %.2f", pose.x);
            pros::lcd::print(1, "Y: %.2f", pose.y);
            pros::lcd::print(2, "Theta: %.2f", pose.theta);
            pros::delay(50); // 20 FPS
        }
    });
}

void disabled() {}
void competition_initialize() {}

// --------- DRIVER CONTROL (OPCONTROL) ---------
void opcontrol() {
    static int lastLeft = 0;
    static int lastRight = 0;
    const int rampStep = 8;          // Smooth acceleration step
    const double imuCorrectionGain = 0.05; // IMU straight-line correction

    while (true) {
        // --- READ JOYSTICKS ---
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

        // --- DEADZONE ---
        if (abs(leftY) < 5) leftY = 0;
        if (abs(rightY) < 5) rightY = 0;

        // --- RAMP FILTER ---
        leftY = lastLeft + std::clamp(leftY - lastLeft, -rampStep, rampStep);
        rightY = lastRight + std::clamp(rightY - lastRight, -rampStep, rampStep);
        lastLeft = leftY;
        lastRight = rightY;

        // --- SPEED MODE ---
        double speedMultiplier = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1) ? 0.5 : 1.0;

        // --- EXPONENTIAL DRIVE CURVE ---
        auto applyExpo = [](int input) -> int {
            double sign = (input >= 0) ? 1.0 : -1.0;
            double absInput = abs(input);
            double output = pow(absInput / 127.0, 1.5) * 127.0; // smooth start, strong top
            return static_cast<int>(output * sign);
        };
        leftY = applyExpo(leftY);
        rightY = applyExpo(rightY);

        // --- IMU DRIFT CORRECTION ---
        double headingError = imu.get_rotation() - chassis.getPose().theta;
        double correction = headingError * imuCorrectionGain;
        leftY -= static_cast<int>(correction);
        rightY += static_cast<int>(correction);

        // --- DRIVE TANK ---
        chassis.tank(leftY * speedMultiplier, rightY * speedMultiplier);

        // --- TELEMETRY ---
        pros::lcd::print(0, "Left: %d | Right: %d", leftY, rightY);
        pros::lcd::print(1, "Speed Mode: %s", speedMultiplier == 1.0 ? "Turbo" : "Precision");

        pros::delay(10);
    }
}

// --------- AUTONOMOUS---------
void autonomous() {
}
