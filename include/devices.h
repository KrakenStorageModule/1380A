#ifndef DEVICES_H
#define DEVICES_H

#include "pros/motors.hpp"
#include "pros/imu.hpp"
#include "lemlib/api.hpp"
#include "robodash/api.h"

//controller
extern pros::Controller controller;

//pistons n shii
extern pros::adi::DigitalOut backWings;
extern pros::adi::DigitalOut wingsL;
extern pros::adi::DigitalOut wingsR;
extern pros::adi::DigitalOut hang1;
extern pros::adi::DigitalOut hang2;

//Motors
extern pros::MotorGroup intake;

//temperature stuff
  extern bool rumbleOnce;
  extern  bool rumbleTwice;
  extern    int rumbleCooldown;
  extern  int rumbleOnceTimer;
  extern  int rumbleTwiceTimer;
  extern  int currentTime;
  extern  double avgTempLeft;
  extern  double avgTempRight;
  extern   int avgTempTotal;
  extern    std::string tempReturn;
  extern    std::string warnTag;

//bools for toggles
  extern bool backToggle;
  extern bool rightToggle;
  extern bool leftToggle ;

//driver control functions 
void intakeControl();
 void hang();
void pneumaticsControl();

#endif //DEVICES_H
