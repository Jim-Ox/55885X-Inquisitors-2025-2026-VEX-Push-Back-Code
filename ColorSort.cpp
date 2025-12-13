#include "main.h"

int ColorSortMode = 0;
/*
0 = none
1 = red alliance (sort BLUE)
2 = blue alliance (sort RED)
*/

bool ColorSortState = false;

const int FILTER_FRAMES = 5;
const int DIST_THRESHOLD = 180;   // adjust depending on how close disc needs to be

int redCount = 0;
int blueCount = 0;

bool discLocked = false;

// Corrected color detection function
pros::Color get_color(pros::Optical* sensor) {
    float hue = sensor->get_hue();

    // RED: 0–30 OR 330–360
    if (hue < 30 || hue > 330) {
        return pros::Color::red;
    }
    // BLUE: 180–270
    else if (hue >= 180 && hue <= 270) {
        return pros::Color::blue;
    }

    return pros::Color::white;
}

void ColorSortFunction() {

    // If sorting is off → reset everything
    if (!ColorSortState || ColorSortMode == 0) {
        redCount = 0;
        blueCount = 0;
        discLocked = false;
        return;
    }

    int prox = opticalSensor.get_proximity();

    // DISC NOT CLOSE ENOUGH → reset filters & unlock
    if (prox < DIST_THRESHOLD) {
        redCount = 0;
        blueCount = 0;
        discLocked = false;
        return;
    }

    // Disc is close: check color
    pros::Color detected = get_color(&opticalSensor);

    // Frame smoothing
    if (detected == pros::Color::red) {
        redCount++;
        blueCount = 0;
    }
    else if (detected == pros::Color::blue) {
        blueCount++;
        redCount = 0;
    }
    else {
        redCount = 0;
        blueCount = 0;
    }

    // Sorting logic (only once per disc)
    if (!discLocked) {

        // MODE 1: RED ALLIANCE → eject BLUE
        if (ColorSortMode == 1 && blueCount >= FILTER_FRAMES) {
            intakeMotor3.move_velocity(-255);
            discLocked = true;
        }

        // MODE 2: BLUE ALLIANCE → eject RED
        else if (ColorSortMode == 2 && redCount >= FILTER_FRAMES) {
            intakeMotor3.move_velocity(-255);
            discLocked = true;
        }
    }

    pros::delay(10);
}
