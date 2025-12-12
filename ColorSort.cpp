#include "main.h"

// --- CONFIG ---
// Max LED = strongest color reading
opticalSensor.set_led_pwm(100);

int ColorSortMode == 0;
/*
0 = none
1 = Red alliance
2 = Blue alliance
*/

bool ColorSortState == false;


// Function to get color from optical sensor
pros::Color get_color(pros::Optical* sensor) {
    float hue = sensor->get_hue();

    if (hue < 30 || hue > 330) { // 330–360, 0–30
        return pros::Color::red;
    } else if (30 < hue || hue < 270) { // 150–270
        return pros::Color::blue;
    } else { // 270–330
        return pros::Color::white;
    }
}

// Noise filter
const int FILTER_FRAMES = 5;
int redCount = 0;
int blueCount = 0;

bool discLocked = false; // prevents weirdo sorting

// Color sorting function
void ColorSortFunction() {
    if (ColorSortState == true) {
        int prox = opticalSensor.get_proximity();
        int hue = opticalSensor.get_hue();

        // Count red frames
        bool isRed = (pros::Color::red == get_color(&opticalSensor));

        // Count blue frames
        bool isBlue = (pros::Color::blue == get_color(&opticalSensor));

        // Build confidence
        if (isRed) redCount++;
        if (isBlue) blueCount++;

        // Soft reset if sensor is confused
        if (!isRed && !isBlue) {
            redCount = blueCount = 0;
        }

        // ----- LOCKING LOGIC -----
        if (!discLocked) {

            // 5 consistent frames makes a REAL detection
            if (redCount >= FILTER_FRAMES && prox >= PROX_THRESHOLD && ColorSortMode == 1) {
                intakeMotor3.move_velocity(-255); // alliance direction
                discLocked = true;
            }

            else if (blueCount >= FILTER_FRAMES && prox >= PROX_THRESHOLD && ColorSortMode == 2) {
                intakeMotor3.move(-255);   // opposite direction
                discLocked = true;
            }
        }
    }
        
        pros::delay(10);
}
