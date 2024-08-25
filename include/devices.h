//Declare everything here before you define it in devices.cpp 
#ifndef DEVICES_H
#define DEVICES_H

#include "pros/motors.hpp"
#include "pros/imu.hpp"
#include "lemlib/api.hpp"
#include "robodash/api.h"

//High Stakes Pistons
extern pros::adi::DigitalOut hang1; //hang pistons
//extern pros::adi::DigitalOut hang2; //hang pistons
//extern pros::adi::DigitalOut utilArm; //corner/mogo arm
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
  extern std::string tempReturn;
  extern std::string warnTag;

  //Toggles
  extern bool mogoToggle;
  extern bool hangToggle;
  // extern bool utilToggle;

//driver control functions 
void intakeControl();
void pneumaticsControl();
void armPID();
//controller
extern pros::Controller controller;

//sensors

//hang sensors
extern pros::Rotation armTrack;

//pid variables
    extern float error;
    extern float prevError;
    extern float P;
    extern float I;
    extern float D;
    extern float F;
   extern  float kP;
   extern float kI;
   extern float kD;
   extern float kF;
   extern float integral;
   extern float deriv;
   extern float deadband;

#endif //DEVICES_H
