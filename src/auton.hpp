#pragma once
#include <tuple>
#include <vector>
#include "pros/llemu.hpp"
#include "pros/apix.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"

using namespace std;
class Auton;

// Type alias for auton functions using 
using AutonFunc = void(*)();

// Struct for auton metadata 
struct AutonEntry { 
    const char* name; 
    AutonFunc func; 
};

// Declaration of variables
extern int screenstate;
extern const AutonEntry autonTable[];
extern uint8_t last_state;
extern uint8_t state;

extern const double Field_wall;
extern const double Sensor_offset;

// Function declarations 
void runAuton(int auton); 
void Screen_Display();
void AutonSelector();

void resetYFromWall(lemlib::Chassis& chassis);
void resetXFromWall(lemlib::Chassis& chassis);

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