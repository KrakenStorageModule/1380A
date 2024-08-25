#pragma once
#include "main.h"
#include "lemlib/api.hpp"
#include "robodash/api.h"
#include "robodash/views/console.hpp"
#include "robodash/views/image.hpp"
#include "devices.h"
#include "autons.h"
#include "globals.h"
using pros::delay;
using std::string;




// initialize function. Runs on program startup
void initialize() {
    chassis.calibrate(); // calibrate sensors
    intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    basket.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    selector.focus();
}

void competition_initialize(){

}

//This Runs After Both The Driver Control and Autonomous Period
void disable(){
//Setting pistons to false in here closes them after a match ends ;)
mogo1.set_value(false);
mogo2.set_value(false);
//utilArm.set_value(false);

}

//Run your auton selector here
void autonomous(){
    //Runs the controller display as a task
    pros::Task controllerHUDTask(controllerHUD);
    //Shifts to the selector screen
    selector.focus();
    //Calls the auton selector
    selector.run_auton();
}

//This Runs During The Driving Portion of a Match
void opcontrol(){
    //Runs the controller temp display as a task
    pros::Task controllerHUDTask(controllerHUD);
    while (true) {
        //Run driver control functions below this line
            intakeControl();
            pneumaticsControl();
            

         // get left y and right y positions from the joysticks
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        // This is the control scheme ->  rn it's tank
        chassis.curvature(leftY, rightX);

        // delay to save resources
        pros::delay(25);
    }

}