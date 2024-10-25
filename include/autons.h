//Declare everything here before you define it in autons.cpp =
#ifndef AUTONS_H
#define AUTONS_H

#include "main.h"

#include "pros/motors.hpp"

#include "pros/imu.hpp"

#include "lemlib/api.hpp"

#include "robodash/api.h"

//auton device funtions
void autonMogo();
void autonWall();
void autonLift();
void autonIntake();

//autons
void lateralPIDTune();
void turningPIDTune();
void trackOdom();
//values for tracking position
extern float xValue;
extern float yValue;
extern float thetaValue;

extern bool autonRan;

//drivetrain motorgroups
extern pros::MotorGroup left_motor_group;
extern pros::MotorGroup right_motor_group;

//console
extern rd::Console console;
//lemlib stuff
extern lemlib::Chassis chassis;

//selector
extern rd::Selector selector;

//boolean for intake "state machine"
extern bool intakeState;
#endif // AUTONS_H