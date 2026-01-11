#pragma once
#include <tuple>
#include <vector>
#include "pros/llemu.hpp"

using namespace std;
class Auton;

/*
This will have all the variables and functions related to autonomous
*/

extern int screenstate;
extern uint8_t last_state;
extern uint8_t state;

void Screen_Display();
void AutonSelector();

void testAuton();
void rightRed();
void leftRed(); 
void rightBlue();
void leftBlue();
void skills();
void runSelectedAuton();

void Intake(int velocity);
void Center_Intake(int velocity);