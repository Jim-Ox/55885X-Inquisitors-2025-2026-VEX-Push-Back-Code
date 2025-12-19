#pragma once
#include <tuple>
#include <vector>

using namespace std;
class Auton;

class AutonSelector {
 public:
  std::vector<Auton> Autons;
  int current_auton_page;
  int auton_count;
  AutonSelector();
  AutonSelector(std::vector<Auton> autons);
  void call_selected_auton();
  void print_selected_auton();
  void add_autons(std::vector<Auton> autons);
};

/*
This will have all the variables and functions related to autonomous
*/

extern int autonState;


void test();
void rightRed();
void leftRed(); 
void rightBlue();
void leftBlue();
void skills();
void runSelectedAuton();