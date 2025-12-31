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
extern int last_state;
extern int state;

void screendisplay();
void AutonSelector();

void testAuton();
void rightRed();
void leftRed(); 
void rightBlue();
void leftBlue();
void skills();
void runSelectedAuton();