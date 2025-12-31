#include "main.h"
#include "pros/misc.h"
#include <sys/_intsup.h>

    //pneumatic states
    bool DescoreState = false;
    bool ParkState = false;
    bool LTCState = false;
    bool PistonliftState = false;

 // descore pneumatics
void descore(){
    if (DescoreState == false) {
            Descore1.set_value(true);
            Descore2.set_value(true);
            DescoreState = true;
        } else {
            Descore1.set_value(false);
            Descore2.set_value(false);
            DescoreState = false;
        }
    pros::delay(10);
}

void park(){
    if (ParkState == false) {
            Park.set_value(true);
            ParkState = true;
        } else {
            Park.set_value(false);
            ParkState = false;
        }
}

void LTCdeploy(){
    if (LTCState == false) {
            LTC.set_value(true);
            LTCState = true;
        } else {
            LTC.set_value(false);
            LTCState = false;
        }
}

void PistonliftControl(){
    if (PistonliftState == false) {
            Pistonlift.set_value(true);
            PistonliftState = true;
        } else {
            Pistonlift.set_value(false);
            PistonliftState = false;
        }
}

void pneumaticsControl(){
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
        descore(); // call descore function
    }

    // park pneumatics
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        park(); // call park function
    }

    // LTC pneumatic
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
        LTCdeploy(); // call LTC deploy function
    }

    //Piston lift pneumatic
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        PistonliftControl(); // call piston lift control function
    }

    pros::delay(10);
}
