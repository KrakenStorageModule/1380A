//This file is for creating Devices (Motors, MotorGroups, Pneumatics, etc) 
//and functions using these devices to be called in OpControl
/*HELLA IMPORTANT =>
DRIVER CONTROLS =>
R1/R2 = Intake 
L2 = Doinker
A = Intake Lift
L1 = WallStake pistons
*/
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
#include "pros/optical.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <string>
#include "cmath"
#include "robodash/views/selector.hpp"
using pros::Motor;
using pros::delay;
using std::to_string;
//Initialize Controller here
pros::Controller controller(pros::E_CONTROLLER_MASTER);

//Initialize Motors/MotorGroups Here
Motor intakeHood(21, pros::v5::MotorGears::blue);
Motor intakeFront(22, pros::v5::MotorGears::blue);
//Motor basket(20, pros::v5::MotorGears::green);


//Initialize Pneumatics Here
 pros::adi::DigitalOut utilArm('A'); //doinker
 pros::adi::DigitalOut mogo1('B'); // mogo clamp 1
 pros::adi::DigitalOut mogo2('C'); // mogo clamp 2
 pros::adi::DigitalOut wallArm1('E'); //Wallstake 1
 pros::adi::DigitalOut wallArm2('F'); // Wallstake 2
 pros::adi::DigitalOut intakeLift('G'); // Wallstake 2

 //Variables for ControllerHUD(); -> no touchies
    double avgTempLeft = 0;
    double avgTempRight = 0;
    int avgTempTotal = 0;
    std::string tempReturn = " ";

    std::string autonName = "";
//Booleans for Driver Control Toggles
   bool mogoToggle = false;
   bool wallToggle = false;
   bool utilToggle = false;
   bool intakePistonToggle = false;
//PID Variables
    float error = 0.0;
    float prevError = 0.0;
    float P = 0.0;
    float I = 0.0;
    float D = 0.0;
    float F = 0.0;
    float integral = 0.0;
    float deriv = 0.0;
    float deadband = 1;


//Auton Controller HUD => Displays/Cycles Selected Auton
void controllerAutonHUD(){
    while (true) {
                // When the right button is pressed it cycles "right" through the autons on the controller
                //yes it wraps around cuz of mod (when on last one, incrementing it )
                if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
                    int newIndex = (selector.selected_routine - &selector.routines[0] + 1) % selector.routines.size();
                    selector.selected_routine = &selector.routines[newIndex];
                }
                 // When the left button is pressed it cycles "left" through the autons on the controller
                if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
                    int newIndex = (selector.selected_routine - &selector.routines[0] - 1 + selector.routines.size()) % selector.routines.size();
                    selector.selected_routine = &selector.routines[newIndex];
                }
        //gets the name (as a string) of the current auton and prints it to controller
        autonName = selector.selected_routine->name;
        controller.set_text(0,0,autonName);
        delay(110);
}
}

//Driver Control Controller HUD => Displays DT Temp
void controllerHUD(){
        while (true) {
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

      //Text Display
        if(avgTempTotal < 200){
        controller.set_text(0, 0, "DT: " + tempReturn + "F ");
        }else {
        
         controller.set_text(0, 0, "Motor(s) Unplugged");
        }
        //Because this is run as a task, you need a delay to protect the Brain's CPU
        //Normally this is like 10-25 ms, but the controller screen updates every 110 ms
        //So having the delay be smaller (more frequent updates) will brick ur controller
        delay(110);
    }
}
//TUNE THESE
    float kP = 0.0; // "Gas Pedal"
    float kI = 0.0; //Ignore this unless desperate
    float kD = 0.0; //This should be much larger than kP
    float kF = 0.0; //Feedforward (counteracts gravity)
    float range = 10000; //Limit for integral anti-windup
//arm pid loop for macros and whatnot

/*
void armPID(float target){
    //deadband
    while(deadband >fabs(error) ){
     error = target - armTrack.get_angle();
        //kills the loop if the driver tries to move it manually
        //essentially an override
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) 
         || controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
            break;
        }
        //Calculates the differences between the target and current
        error = target - armTrack.get_angle();
        //calculations
        P = error*kP;
        integral += error;
        I = integral*kI;    
        deriv = error-prevError;
        D = deriv*kD;
        F = target*kF;
        prevError = error;
    //Integral Anti-WindUp
        if(fabs(integral) > range){
            integral = 0;
            integral += error;
        }
        //Actual Movement
         basket.move(P+I+D+F);
    }

} */

//Driver Control Functions Go Here -> Call them in the opControl(); while loop
void intakeControl(){
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            intakeHood.move_voltage(12000);
            intakeFront.move_voltage(12000);
        }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
            intakeHood.move_voltage(-12000);
            intakeFront.move_voltage(12000);
        }else{
            intakeHood.move_voltage(0);
            intakeFront.move_voltage(0);
        }
    }

//Driver Control Pneumatics Code
void pneumaticsControl(){
       // utility arm 
            if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
                utilToggle = ! utilToggle;
                utilArm.set_value(utilToggle);
            }
        //mogo clamp
            if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
                mogoToggle = ! mogoToggle;
                mogo1.set_value(mogoToggle);
                mogo2.set_value(mogoToggle);
            }
        //intake piston
            if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){
                intakePistonToggle = ! intakePistonToggle;
                intakeLift.set_value(intakePistonToggle);
            }
        //wallstake pistons
            if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
                wallToggle = ! wallToggle;
                wallArm1.set_value(wallToggle);
                wallArm2.set_value(wallToggle);
            }

    }


   //Color Sorting Code
    // void segregation(){
    //     //RED
    //     if (!controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
    //         if(allianceColor == 1){
    //                 //closes colorsort mech when it returns to baseline ora red ring
    //                 if(((350 >= vision.get_hue()) && (10 <= vision.get_hue())) 
    //                  ||((90 >= vision.get_hue()) && (70 <= vision.get_hue()))){
    //                     colorSort.set_value(false);
                    
    //                     //tune this range 
    //                 }else  if((220 >= vision.get_hue()) && (200 <= vision.get_hue())){
    //                     //kicks out the wrong color ring
    //                             colorSort.set_value(true);
    //                 }
    //         }
    //                  //BLUE
    //                  if(allianceColor == 2){
    //                     if((220 >= vision.get_hue()) && (200 <= vision.get_hue()) 
    //                      || ((90 >= vision.get_hue()) && (70 <= vision.get_hue()))){
    //                         //tune this bih
    //                  }else if((350 >= vision.get_hue()) && (10 <= vision.get_hue())){
    //                     //kicks out the wrong color ring
    //                             colorSort.set_value(true);
    //                  }
                        
                    
    //     }else{
    //         colorSort.set_value(false);
    //     }
    // }
    // }
