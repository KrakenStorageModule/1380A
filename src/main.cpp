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
            avgTempLeft = (left_motor_group.get_temperature(0) + left_motor_group.get_temperature(1) + left_motor_group.get_temperature(2))/3;
            avgTempRight = (right_motor_group.get_temperature(0) + right_motor_group.get_temperature(1) + right_motor_group.get_temperature(2))/3;
            avgTempTotal = int ((((avgTempLeft + avgTempRight)*1.8 )/ 2) + 32);
            tempReturn = to_string(avgTempTotal);
            // Safe/Too hot display
            // rumble feature  also :)
            if(avgTempTotal < 100){
                warnTag = "Cool";
                rumbleOnce = false;
                rumbleTwice = false;
            }
             if (avgTempTotal > 100) {
                warnTag = " Normal";
                rumbleOnce = false;
                rumbleTwice = false;
            }
             if(avgTempTotal > 120){
                warnTag = "Caution";
                    if (!rumbleOnce && (currentTime - rumbleOnceTimer > rumbleCooldown)) {
                        controller.rumble(".");
                        rumbleOnce = true;
                        rumbleOnceTimer = currentTime;
                        rumbleTwice = false;
                    }
            }
             if(avgTempTotal > 130){
                warnTag = "Danger"; 
                if (!rumbleTwice && (currentTime - rumbleTwiceTimer > rumbleCooldown)) {
                        controller.rumble("_");
                        rumbleTwice = true;
                        rumbleTwiceTimer = currentTime;
                        rumbleOnce = false;
                    }
                
            }
        //Text Display
        controller.set_text(0, 0, "DT: " + tempReturn + "F" + " " + warnTag);
        delay(110);
    }
}

// initialize function. Runs on program startup
void initialize() {
    chassis.calibrate(); // calibrate sensors
    intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    selector.focus();
}

void autonomous(){
    pros::Task controllerHUDTask(controllerHUD);
    selector.focus();
    selector.run_auton();
}

void competition_initialize(){

}

void disable(){
hang1.set_value(false);
hang2.set_value(false);
backWings.set_value(false);
wingsL.set_value(false);
wingsR.set_value(false);
}





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

        // This is the control scheme -> rn it's tank
        chassis.tank(leftY, rightY);

        // delay to save resources
        pros::delay(25);
    }

}