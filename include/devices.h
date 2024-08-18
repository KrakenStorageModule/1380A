//Declare everything here before you define it in devices.cpp 
#ifndef DEVICES_H
#define DEVICES_H

#include "pros/motors.hpp"
#include "pros/imu.hpp"
#include "lemlib/api.hpp"
#include "robodash/api.h"

//High Stakes Pistons
extern pros::adi::DigitalOut hang1; //hang pistons
extern pros::adi::DigitalOut hang2; //hang pistons
extern pros::adi::DigitalOut utilArm; //corner/mogo arm
extern pros::adi::Pneumatics pto;
extern pros::adi::DigitalOut mogo1; // mogo clamp
extern pros::adi::DigitalOut mogo2; // mogo clamp

//Motors
extern pros::Motor intake;
extern pros::Motor basket;

//temperature stuff
void controllerHUD();
  extern bool rumbleOnce;
  extern  bool rumbleTwice;
  extern    int rumbleCooldown;
  extern  int rumbleOnceTimer;
  extern  int rumbleTwiceTimer;
  extern  int currentTime;
  extern  double avgTempLeft;
  extern  double avgTempRight;
  extern int avgTempTotal;
  extern    std::string tempReturn;
  extern    std::string warnTag;

  //Toggles
  extern bool hangToggle;
  extern bool ptoToggle;
  extern bool mogoToggle;
  extern bool utilToggle;

//a check for hang
extern int tier;

//driver control functions 
void intakeControl();
 void hang();
void pneumaticsControl();

//controller
extern pros::Controller controller;

//sensors

//hang sensors
extern pros::adi::DigitalIn hook;
extern pros::adi::DigitalIn hook2;
extern pros::Rotation dr4bTrack;


#endif //DEVICES_H
