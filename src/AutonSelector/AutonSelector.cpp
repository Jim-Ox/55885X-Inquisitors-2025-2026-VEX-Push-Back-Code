#include "main.h"
#include "auton.hpp"
#include "pros/llemu.hpp"
#include "pros/rtos.h"
#include <cstdlib>

void screendisplay(){
    if (last_state == 0){
        pros::lcd::print(4, "No Auton Selected");
    }
    else if (last_state == 1){
        pros::lcd::print(4, "Test Auton");
    }
    else if (last_state == 2){
        pros::lcd::print(4, "Right Red");
    }
    else if (last_state == 3){
        pros::lcd::print(4, "Left Red");
    }
    else if (last_state == 4){
        pros::lcd::print(4, "Right Blue");
    }
    else if (last_state == 5){
        pros::lcd::print(4, "Left Blue");
    }
    else if (last_state == 6){
        pros::lcd::print(4, "Skills");
    }
    pros::delay(100);
}

// Autonomous Selector screen page
void AutonSelector(){
    uint8_t last_state = 0;

    while(true) {
        uint8_t state = pros::lcd::read_buttons();
        uint8_t kdown = state & (~last_state);
        last_state = state;

        if (kdown & LCD_BTN_LEFT) {
            screenstate--;
            
            // wrap around
            if (screenstate < 0) {
            screenstate = 6;
            }

        } else if (kdown & LCD_BTN_RIGHT) {
            screenstate++;
            
            // wrap around
            if (screenstate > 6) {
            screenstate = 0;
            }

        } else if (last_state & LCD_BTN_CENTER) {
            pros::lcd::print(6, "Auton Selected!");
            break;
        }

        screendisplay();

        pros::delay(100);
    }
}