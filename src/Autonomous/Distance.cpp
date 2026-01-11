#include "auton.hpp"
#include "driver.hpp"
#include "main.h"
#include <sys/_intsup.h>

const double Field_wall = 144;  // distance between field walls parallel to each other (inches)
const double Sensor_offset = 1.285478; // distance from the distance sensor to the robot's front (inches)

// temporary variable for averaging
double a = 0;

// function to get average distance sensor reading over a period of time
double getAvgDistance(int milliseconds) {
    int sum = 0;
    int count = 0;

    uint32_t start = pros::millis();

    while (pros::millis() - start < milliseconds) {
        int reading = distanceSensor.get(); // mm
        if (reading > 0) {        // ignore invalid readings
            sum += reading;
            count++;
        }
        pros::delay(5); // sample every 5 ms
    }

    if (count == 0) return 0; // fallback if no valid readings

    return static_cast<double>(sum) / count; // average in mm
}

// function to set the origin based on distance sensor reading over the span of 250 ms
void resetOrigin() {
    double avgMM = getAvgDistance(250);
    a = avgMM / 25.4; // convert to inches
}


//since we only have 1 distance sensor, we need 2 functions to reset X and Y separately

// reset Y position using distance sensor reading
void resetYFromWall(lemlib::Chassis& chassis, int originIN) {
    int readingMM = distanceSensor.get(); // mm from sensor 
    double readingIN = readingMM / 25.4; // convert to inches

    // sanity check
    if (readingIN > 1 && readingIN < 80) {
        double robotY = originIN - (readingIN + Sensor_offset);

        chassis.setPose(
            chassis.getPose().x,
            robotY,
            chassis.getPose().theta
        );
    }
}

// reset X position using distance sensor reading
void resetXFromWall(lemlib::Chassis& chassis, int originIN) {
    int readingMM = distanceSensor.get(); // mm from sensor
    double readingIN = readingMM / 25.4; // convert to inches

    // sanity check
    if (readingIN > 1 && readingIN < 80) {
        double robotX = originIN - (readingIN + Sensor_offset);

        chassis.setPose(
            robotX,
            chassis.getPose().y,
            chassis.getPose().theta
        );
    }
}
