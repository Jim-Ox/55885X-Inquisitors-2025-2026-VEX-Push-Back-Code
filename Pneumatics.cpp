#include "main.h"
#include <sys/_intsup.h>

    //pneumatic states
    bool DescoreState = false;
    bool ParkState = false;
    bool LTCState = false;

 // descore pneumatics
void pneumaticsControl(){
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
        if (DescoreState == false) {
            Descore1.set_value(true);
            Descore2.set_value(true);
            DescoreState = true;
        } else {
            Descore1.set_value(false);
            Descore2.set_value(false);
            DescoreState = false;
        }
    }

    // park pneumatics
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
        if (ParkState == false) {
            Park1.set_value(true);
            Park2.set_value(true);
            ParkState = true;
        } else {
            Park1.set_value(false);
            Park2.set_value(false);
            ParkState = false;
        }
    }

    // LTC pneumatic
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
        if (LTCState == false) {
            LTC.set_value(true);
            LTCState = true;
        } else {
            LTC.set_value(false);
            LTCState = false;
        }
    }
    pros::delay(10);
}
