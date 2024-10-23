//Declare everything here before you define it in devices.cpp 
#ifndef DEVICES_H
#define DEVICES_H

#include "pros/motors.hpp"
#include "pros/imu.hpp"
#include "lemlib/api.hpp"
#include "robodash/api.h"
#include "pros/optical.hpp"
//High Stakes Pistons
extern pros::adi::DigitalOut hang1; //hang pistons
//extern pros::adi::DigitalOut hang2; //hang pistons
extern pros::adi::DigitalOut utilArm; //corner/mogo arm
extern pros::adi::DigitalOut mogo1; // mogo clamp 1
extern pros::adi::DigitalOut mogo2; // mogo clamp 2
extern pros::adi::DigitalOut colorSort; // colorsort mech
extern pros::adi::DigitalOut intakeLift; // intake lift

//Motors
//extern pros::Motor basket;
extern pros::Motor intakeHook;
extern pros::Motor intakeFront;
//controller display stuff
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
  extern std::string autonName;
  //Toggles
  extern bool mogoToggle;
  extern bool hangToggle;
  extern bool utilToggle;
  extern bool intakeToggle;

//driver control functions 
void intakeControl();
void pneumaticsControl();
void controllerAutonHUD();
// void armPID();
// void segregation();
// void basketControl();
//controller
extern pros::Controller controller;

//sensors

// extern pros::Rotation armTrack;
extern pros::Rotation odomH;
extern pros::Rotation odomV;
extern pros::Optical vision;

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
