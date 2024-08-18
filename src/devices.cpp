//This file is for creating Devices (Motors, MotorGroups, Pneumatics, etc) \
//and functions using these devices to be called in OpControl

#include "autons.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "devices.h" //This references the header file associated with this .cpp file
#include "pros/abstract_motor.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motor_group.hpp"
#include "pros/motors.h"
#include "globals.h"
#include "pros/rtos.hpp"
#include <string>
using pros::Motor;
using pros::delay;
using std::to_string;
//Initialize Controller here
pros::Controller controller(pros::E_CONTROLLER_MASTER);

//Initialize Motors/MotorGroups Here
Motor intake(21, pros::v5::MotorGears::blue);
Motor basket(20, pros::v5::MotorGears::green);


//Initialize Pneumatics Here
 pros::adi::DigitalOut hang1('A'); //hang pistons
 pros::adi::DigitalOut hang2('B'); //hang pistons
 pros::adi::DigitalOut utilArm('C'); //corner/mogo arm
 pros::adi::DigitalOut arm1('D'); //basket pistons
 pros::adi::DigitalOut arm2('E'); //basket pistons
pros::adi::Pneumatics pto('F', true);
  pros::adi::DigitalOut mogo1('J'); // mogo clamp
 pros::adi::DigitalOut mogo2('K'); // mogo clamp
//Special Note -> This is a Pneumatics object
//I typically use digitalOut cuz it's easier, but the pto has to start extended.


 //Variables for ControllerHUD(); -> no touchies
    bool rumbleOnce = false;
    bool rumbleTwice = false;
    int rumbleCooldown = 30000; // cooldown period in milliseconds (e.g., 60 seconds)
     int rumbleOnceTimer = 0;
     int rumbleTwiceTimer = 0;
     int currentTime = 0;
    double avgTempLeft = 0;
    double avgTempRight = 0;
    int avgTempTotal = 0;
    std::string tempReturn = " ";
    std::string warnTag = " ";

//Booleans for Driver Control Toggles
   bool hangToggle;
   bool ptoToggle;
   bool mogoToggle;
   bool utilToggle;
   int tier = 0;
//sensors 
 pros::adi::DigitalIn hook('G');
 pros::adi::DigitalIn hook2('H');
 pros::Rotation dr4bTrack(10);


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

//Driver Control Functions Go Here -> Call them in the opControl(); while loop
void intakeControl(){
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            intake.move_voltage(12000);
        }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
            intake.move_voltage(-12000);
        }else{
            intake.move_voltage(0);
        }
    }

//hang code
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
    
//Driver Control Pneumatics Code
void pneumaticsControl(){
        //utility arm 
            if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){
                utilToggle = ! utilToggle;
                utilArm.set_value(utilToggle);
            }
        //mogo clamp
            if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
                mogoToggle = ! mogoToggle;
                mogo1.set_value(mogoToggle);
                mogo2.set_value(mogoToggle);
            }
        
    }