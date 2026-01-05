#pragma once
#include <tuple>
#include <vector>
#include "pros/llemu.hpp"
#include "pros/apix.h"

using namespace std;
class Auton;

/*
This will have all the variables and functions related to autonomous
*/

extern int screenstate;
extern uint8_t last_state;
extern uint8_t state;

// Type alias for auton functions using 
using AutonFunc = void(*)();

// Struct for auton metadata 
struct AutonEntry { 
    const char* name; 
    AutonFunc func; 
};

// Declare globals variables
extern int screenstate;
extern const AutonEntry autonTable[];

// Function declarations 
void runAuton(int auton); 
void Screen_Display();
void AutonSelector();

//auton routines
void testAuton();
void rightRed();
void leftRed(); 
void rightBlue();
void leftBlue();
void skills();

// intake functions
void Intake(int velocity);
void Center_Intake(int velocity);