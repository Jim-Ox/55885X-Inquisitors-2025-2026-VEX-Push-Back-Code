#include "liblvgl/llemu.hpp"
#include "main.h"
#include "auton.hpp"
#include "pros/llemu.hpp"
#include "pros/rtos.h"
#include <cstdint>
#include <cstdlib>

/* Autonomous Selector screen page
0 - Test Auton
1 - Right Red
2 - Left Red
3 - Right Blue
4 - Left Blue
5 - Skills
6 - No Auton Selected (will always be last int if more autons are added)
*/
int screenstate = 0;

using AutonFunc = void(*)();

constexpr AutonEntry autonTable[] = { 
    {"Test Auton", testAuton}, 
    {"Right Red", rightRed}, 
    {"Left Red", leftRed}, 
    {"Right Blue", rightBlue}, 
    {"Left Blue", leftBlue}, 
    {"Skills", skills} 
}; 

constexpr int AUTON_COUNT = sizeof(autonTable) / sizeof(AutonEntry);

//display selected auton on LCD
void Screen_Display(){
    if (screenstate >= 0 && screenstate < AUTON_COUNT) {
        pros::lcd::print(3, "Selected Auton:");
        pros::lcd::print(4, "%s", autonTable[screenstate].name);
    } else {
        pros::lcd::print(3, "Selected Auton:");
        pros::lcd::print(4, "No Auton");
    }
}

// Autonomous Selector screen page
void AutonSelector(){
    uint8_t state = 0;
    uint8_t kdown = 0;
    uint8_t last_state = 0;
    int maxState = 0;
    pros::lcd::clear_line(4-6);

    while(true) {
        // read the buttons
        uint8_t state = pros::lcd::read_buttons(); // get current buttons pressed
        uint8_t kdown = state & (~last_state); // detect newly pressed buttons
        last_state = state; // update last state
        int maxState = AUTON_COUNT + 1; // +1 for "No Auton Selected" option (prevents screen getting stuck on "no auton" option)

        if (kdown & LCD_BTN_LEFT)
            screenstate = (screenstate - 1 + maxState) % maxState;

        if (kdown & LCD_BTN_RIGHT)
            screenstate = (screenstate + 1) % maxState;
        
        // center button to confirm selection
        if (kdown & LCD_BTN_CENTER) {
            // confirm selection
            if (screenstate == AUTON_COUNT) { 
                pros::lcd::print(6, "No Auton Selected");
                break;
            } else { 
                pros::lcd::print(6, "Auton Selected!"); 
                break; 
            }
        }

        // update display
        Screen_Display();

        pros::delay(100);
    }
}

void runAuton(int auton) {
    if (auton < AUTON_COUNT) { 
        
        if (auton >= 0 && auton < AUTON_COUNT) { 
        autonTable[auton].func(); 
        
    }
    } else { // No auton selected 
        pros::lcd::print(6, "No Auton Running"); 
    }
    
}