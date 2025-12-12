#include "main.h"
#include "pros/misc.h"
#include <sys/_intsup.h>

/**
 * Runs in driver control
 */
double driveHeading = 0;

void ChassisDrive(){
    // static so they persist between iterations but are local to this function
    static double driveHeading = 0.0;
    static bool headingLocked = false;

    // joystick read
    int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

    // =====================================================================
    // ======================= IMU STRAIGHT LINE ASSIST =====================
    // =====================================================================

    double currentHeading = imu.get_rotation(); // 0..360 range

    // driving straight if both sticks pushed similarly (threshold adjustable)
    bool drivingStraight = (std::abs(leftY) > 20 && std::abs(rightY) > 20);

    // lock heading ONCE when we start driving straight
    if (drivingStraight && !headingLocked) {
        driveHeading = currentHeading;
        headingLocked = true;
    }
    // unlock heading when not driving straight
    else if (!drivingStraight) {
        headingLocked = false;
    }

    double correction = 0.0;

    // apply correction only when heading is locked
    if (headingLocked) {
        double error = driveHeading - currentHeading;

        // wraparound fix (-180..180)
        if (error > 180.0) error -= 360.0;
        if (error < -180.0) error += 360.0;

        double kP = 1.2; // tune this
        correction = error * kP;

        // clamp correction so driver still retains control
        if (correction > 20.0) correction = 20.0;
        if (correction < -20.0) correction = -20.0;
    }

    // apply correction to tank drive (right, left)
    chassis.arcade(leftY - (int)correction, -(rightY + (int)correction));
    
    pros::delay(10);
}