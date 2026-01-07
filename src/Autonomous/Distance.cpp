#include "auton.hpp"
#include "driver.hpp"
#include "main.h"

const double Field_wall = 144;  // distance between field walls parallel to each other (inches)
const double Sensor_offset = 1.285478; // distance from the distance sensor to the robot's front (inches)

//since we only have 1 distance sensor, we need 2 functions to reset X and Y separately

// reset Y position using distance sensor reading
void resetYFromWall(lemlib::Chassis& chassis) {
    int readingMM = distanceSensor.get(); // mm from sensor 
    double readingIN = readingMM / 25.4; // convert to inches

    // sanity check
    if (readingIN > 1 && readingIN < 80) {
        double robotY = Field_wall - (readingIN + Sensor_offset);

        chassis.setPose(
            chassis.getPose().x,
            robotY,
            chassis.getPose().theta
        );
    }
}

// reset X position using distance sensor reading
void resetXFromWall(lemlib::Chassis& chassis) {
    int readingMM = distanceSensor.get(); // mm from sensor 
    double readingIN = readingMM / 25.4; // convert to inches

    // sanity check
    if (readingIN > 1 && readingIN < 80) {
        double robotX = Field_wall - (readingIN + Sensor_offset);

        chassis.setPose(
            robotX,
            chassis.getPose().y,
            chassis.getPose().theta
        );
    }
}
