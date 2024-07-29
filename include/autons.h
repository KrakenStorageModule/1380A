#pragma once
#ifndef AUTONS_H
#define AUTONS_H
#include "main.h"
#include "pros/motors.hpp"
#include "pros/imu.hpp"
#include "lemlib/api.hpp"
#include "robodash/api.h"

//drivetrain motorgroups
extern pros::MotorGroup left_motor_group;
extern pros::MotorGroup right_motor_group;

//console
extern rd::Console console;
//lemlib stuff
extern lemlib::Chassis chassis;

//selector
extern rd::Selector selector;

//autons
void lateralPIDTune();
void turningPIDTune();
void trackOdom();
void fun();

//values for tracking position
extern float xValue;
extern float yValue;
extern float thetaValue;
#endif // AUTONS_H