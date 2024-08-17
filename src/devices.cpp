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
using pros::Motor;
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
// void hang(){
//         if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1) && controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
//             pto.retract();
//          if((hook.get_value() && hook2.get_value()) != true){
            

//         }


//         }


//     }
    
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