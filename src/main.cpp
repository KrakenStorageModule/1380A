#pragma once
#include "main.h"
#include "lemlib/api.hpp"
#include "robodash/api.h"
#include "robodash/views/console.hpp"
#include "robodash/views/image.hpp"
#include "devices.h"
#include "autons.h"
using pros::delay;
using std::string;
using fmt::to_string;

//controller temp display
void controllerHUD(){
        while (true) {
            currentTime = pros::millis();
            //This block averages the temp of the left side of the drive
            avgTempLeft = (left_motor_group.get_temperature(0) + 
              left_motor_group.get_temperature(1) + 
              left_motor_group.get_temperature(2))/3;
            
            //This block averages the temp of the right side of the drive
            avgTempRight = (right_motor_group.get_temperature(0) +
              right_motor_group.get_temperature(1) + 
              right_motor_group.get_temperature(2))/3;
           
            //Generates the avg of the drivetrain in Fahrenheit
            avgTempTotal =  int ((((avgTempLeft + avgTempRight)*1.8 )/ 2) + 32);
            
            //Casts avgTempTotal to a String
            //Because the controller set_text function only accepts strings
            tempReturn =to_string(avgTempTotal);
            
            // Safe/Too hot display
            // rumble feature  also :) (Commented out cuz useless)
            if(avgTempTotal < 99){
                warnTag = "Cool";
                // rumbleOnce = false;
                // rumbleTwice = false;
            }else if (avgTempTotal > 100) {
                warnTag = " Normal";
                // rumbleOnce = false;
                // rumbleTwice = false;
            } else if(avgTempTotal > 120){
                warnTag = "Caution";
                    // if (!rumbleOnce && (currentTime - rumbleOnceTimer > rumbleCooldown)) {
                    //     controller.rumble(".");
                    //     rumbleOnce = true;
                    //     rumbleOnceTimer = currentTime;
                    //     rumbleTwice = false;
                    // }
            } if(avgTempTotal > 130){
                warnTag = "Danger"; 
                //   if (!rumbleTwice && (currentTime - rumbleTwiceTimer > rumbleCooldown)) {
                //         controller.rumble("_");
                //         rumbleTwice = true;
                //         rumbleTwiceTimer = currentTime;
                //         rumbleOnce = false;
                //  }
            }
        //Text Display
        controller.set_text(0, 0, "DT: " + tempReturn + "F " /*+ warnTag*/);

        //Because this is run as a task, you need a delay to protect the Brain's CPU
        //Normally this is like 10-25 ms, but the controller screen updates every 110 ms
        //So having the delay be smaller (more frequent updates) will brick ur controller
        delay(110);
    }
}

void hang(){
        if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1) && controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
            pto.retract();
            left_motor_group.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
            right_motor_group.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

            //gonna try to get the logic down here. gonna be painful as hell sadly
            while(tier <= 3){
                if ((hook.get_value() != 1) &&(hook2.get_value() != 1) ) {
                    
                }
                hang1.set_value(true);
                hang2.set_value(true);



                tier++;

            }
        }


    }



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
utilArm.set_value(false);

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
            hang();

         // get left y and right y positions from the joysticks
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

        // This is the control scheme ->  rn it's tank
        chassis.tank(leftY, rightY);

        // delay to save resources
        pros::delay(25);
    }

}