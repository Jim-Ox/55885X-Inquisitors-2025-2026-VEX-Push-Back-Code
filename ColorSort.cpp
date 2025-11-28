#include "main.h"
#include <sys/_intsup.h>

/*
 Color sort modes:
 1 - Color sort for side goal (opposing color goes down the 3rd stage)
 2 - Color sort for center goal (opposing color goes up the 3rd stage)

 ColorSortState - whether color sort is active or not (true = active, false = inactive)


 */

int ColorSortMode = 1;
bool ColorSortState = true;
pros::Color SortColor = pros::Color::white; // default to white (no color)

// set LED brightness to max inside the function (calling at global scope was parsed as a declaration)
void ColorSortFunction(){
    while(true){
        pros::delay(10);
    }
}