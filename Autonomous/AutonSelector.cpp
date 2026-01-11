#include "main.h"
#include "auton.hpp"
#include "pros/llemu.hpp"
#include "pros/rtos.h"
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
    if (screenstate == AUTON_COUNT) { 
        pros::lcd::print(4, "No Auton Selected"); 
    } else { 
        pros::lcd::print(4, autonTable[screenstate].name); 
    }
}

// Autonomous Selector screen page
void AutonSelector(){
    uint8_t last_state = 0;

    while(true) {
        // read the buttons
        uint8_t state = pros::lcd::read_buttons();
        uint8_t kdown = state & (~last_state);
        last_state = state;

        if (kdown & LCD_BTN_LEFT) { 
            screenstate--; 
            if (screenstate < 0) 
            screenstate = AUTON_COUNT; // wrap to "No Auton" 
        } 
        
        if (kdown & LCD_BTN_RIGHT) { 
            screenstate++; 
            if (screenstate > AUTON_COUNT) screenstate = 0; 
        }
        
        // center button to confirm selection
        if (kdown & LCD_BTN_CENTER) {
            // confirm selection
            if (screenstate == AUTON_COUNT) { 
                pros::lcd::print(6, "No Auton Selected");
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