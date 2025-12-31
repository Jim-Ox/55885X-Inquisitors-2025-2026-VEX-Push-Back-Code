#pragma once
/*
This will have all the variables and functions related to driver control
*/


// joystick values for driver control
extern int rightY;
extern int leftY;

// IMU straight line assist variables
extern double currentHeading;
extern bool drivingStraight;
extern double correction;

// heading lock variables
extern double driveHeading;
extern bool headingLocked;

//pneumatics variables
extern bool DescoreState;
extern bool ParkState;
extern bool LTCState;
extern bool PistonliftState;

// color sort variables
extern int ColorSortMode;
extern bool ColorSortState;

extern const int PROX_THRESHOLD;
extern const int FILTER_FRAMES;

extern bool discLocked;
extern bool isBlue;
extern bool isRed;

extern int redCount;
extern int blueCount;

extern int prox;
extern int hue;



// straight line assist function
void ChassisDrive();

// intake functions
void anti_jamIntake();
void intakeControl();
void indexerControl();

// pneumatics functions
void descore();
void park();
void LTCdeploy();
void PistonliftControl();
void pneumaticsControl();

// color sort function
void ColorSortFunction();