//Declare everything here before you define it in devices.cpp 
#ifndef DEVICES_H
#define DEVICES_H

#include "pros/motors.hpp"

#include "pros/imu.hpp"

#include "lemlib/api.hpp"

#include "robodash/api.h"

#include "pros/optical.hpp"
 //Pistons
extern pros::adi::DigitalOut utilArm; //doinker
extern pros::adi::DigitalOut mogo1; // mogo clamp 1
extern pros::adi::DigitalOut mogo2; // mogo clamp 2
extern pros::adi::DigitalOut wallArm1; //Wallstake 1
extern pros::adi::DigitalOut wallArm2; // Wallstake 2
extern pros::adi::DigitalOut intakeLift; // Wallstake 2

//Motors
extern pros::Motor intakeHood;
extern pros::Motor intakeFront;

//controller display stuff
void controllerHUD();
extern double avgTempLeft;
extern double avgTempRight;
extern int avgTempTotal;
extern std::string tempReturn;
extern std::string autonName;
extern int newIndex;
extern std::vector<int> ports;
extern std::string portsString;
//Toggles
extern bool mogoToggle;
extern bool wallToggle;
extern bool utilToggle;
extern bool intakePistonToggle;
//driver control functions 
void intakeControl();
void pneumaticsControl();
extern pros::Controller controller;

//sensors

// extern pros::Rotation armTrack;
extern pros::Rotation odomH;
extern pros::Rotation odomV;

//pid variables
extern float error;
extern float prevError;
extern float P;
extern float I;
extern float D;
extern float F;
extern float kP;
extern float kI;
extern float kD;
extern float kF;
extern float integral;
extern float deriv;
extern float deadband;

#endif //DEVICES_H