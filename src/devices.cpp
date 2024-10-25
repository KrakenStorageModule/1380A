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

//Initialize Controller here
pros::Controller controller(pros::E_CONTROLLER_MASTER);

//Initialize Motors/MotorGroups Here
Motor intakeHood(10, pros::v5::MotorGears::blue);
Motor intakeFront(11, pros::v5::MotorGears::blue);


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
std::vector<int> ports = {};

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

void controllerHUD() {
  //local cuz it resets per loop
  std::string portsString = ""; //essentially collects all of the bad port numbers
  bool allMotorsConnected = true; // Flag to check motor connection status

  // Check intake motors
  if (intakeFront.get_port() != 11) {
    portsString += "11 ";
    allMotorsConnected = false;
  }
  if (intakeHood.get_port() != 10) {
    portsString += "10 ";
    allMotorsConnected = false;
  }

  // Check left motor group
  if (left_motor_group.get_port(0) != 1) {
    portsString += "1 ";
    allMotorsConnected = false;
  }
  if (left_motor_group.get_port(1) != 3) {
    portsString += "3 ";
    allMotorsConnected = false;
  }
  if (left_motor_group.get_port(2) != 4) {
    portsString += "4 ";
    allMotorsConnected = false;
  }

  // Check right motor group
  if (right_motor_group.get_port(0) != 5) {
    portsString += "5 ";
    allMotorsConnected = false;
  }
  if (right_motor_group.get_port(1) != 6) {
    portsString += "6 ";
    allMotorsConnected = false;
  }
  if (right_motor_group.get_port(2) != 7) {
    portsString += "7 ";
    allMotorsConnected = false;
  }

  // Only rumble and show unplugged message if any motors are unplugged
  if (!allMotorsConnected) {
    controller.rumble("--------");
    controller.set_text(0, 0, "Unplugged motors: " + portsString);
    return; // Exit early if motors are unplugged
  }

  // If all motors are connected, run auton selector
  if (!autonRan) {
    // When the right button is pressed, cycle "right" through the autons on the controller
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
      int newIndex = (selector.selected_routine - & selector.routines[0] + 1) % selector.routines.size();
      selector.selected_routine = & selector.routines[newIndex];
    }
    // When the left button is pressed, cycle "left" through the autons on the controller
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
      int newIndex = (selector.selected_routine - & selector.routines[0] - 1 + selector.routines.size()) % selector.routines.size();
      selector.selected_routine = & selector.routines[newIndex];
    }
    // Gets the name (as a string) of the current auton and prints it to the controller
    autonName = selector.selected_routine -> name;
    controller.set_text(0, 0, autonName);
  } else { //Runs the temp display now
    // This block averages the temp of the left side of the drive
    avgTempLeft = (left_motor_group.get_temperature(0) +
      left_motor_group.get_temperature(1) +
      left_motor_group.get_temperature(2)) / 3;

    // This block averages the temp of the right side of the drive
    avgTempRight = (right_motor_group.get_temperature(0) +
      right_motor_group.get_temperature(1) +
      right_motor_group.get_temperature(2)) / 3;

    //Prints avg of both sides
    avgTempTotal = int((((avgTempLeft + avgTempRight) * 1.8) / 2) + 32);
    tempReturn = std::to_string(avgTempTotal);
    controller.set_text(0, 0, "DT: " + tempReturn + "F ");
  }
}

// //Driver Control Controller HUD => Displays DT Temp
// void controllerHUD() {

//     if (intakeFront.get_port() != 11) {
//     ports.emplace_back(11);
//   }
//     if (intakeHood.get_port() != 10) {
//       ports.emplace_back(10);
//     }

// if(left_motor_group.get_port(0) != 1){
//       ports.emplace_back(1);

// }
// if(left_motor_group.get_port(1) != 3){
//       ports.emplace_back(3);

// }
// if(left_motor_group.get_port(2) != 4){
//       ports.emplace_back(4);
// }


// if(right_motor_group.get_port(0) != 5){
//       ports.emplace_back(5);

// }
// if(right_motor_group.get_port(1) != 6){
//       ports.emplace_back(6);

// }
// if(right_motor_group.get_port(2) != 7){
//       ports.emplace_back(7);
// }
// controller.rumble("--------");

//   //checks if the tag (manually set in each auton) has been triggered yet
//   if (autonRan == false) {
//     // When the right button is pressed it cycles "right"         through the autons on the controller
//     //yes it wraps around cuz of mod (when on last one, incrementing it )
//     if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
//       int newIndex = (selector.selected_routine - & selector.routines[0] + 1) % selector.routines.size();
//       selector.selected_routine = & selector.routines[newIndex];
//     }
//     // When the left button is pressed it cycles "left" through the autons on the controller
//     if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
//       int newIndex = (selector.selected_routine - & selector.routines[0] - 1 + selector.routines.size()) % selector.routines.size();
//       selector.selected_routine = & selector.routines[newIndex];
//     }
//     //gets the name (as a string) of the current auton and prints it to controller
//     autonName = selector.selected_routine -> name;
//     controller.set_text(0, 0, autonName);
//   } else {
//     //This block averages the temp of the left side of the drive
//     avgTempLeft = (left_motor_group.get_temperature(0) +
//       left_motor_group.get_temperature(1) +
//       left_motor_group.get_temperature(2)) / 3;

//     //This block averages the temp of the right side of the drive
//     avgTempRight = (right_motor_group.get_temperature(0) +
//       right_motor_group.get_temperature(1) +
//       right_motor_group.get_temperature(2)) / 3;

//     //Generates the avg of the drivetrain in Fahrenheit
//     avgTempTotal = int((((avgTempLeft + avgTempRight) * 1.8) / 2) + 32);

//     //Casts avgTempTotal to a String
//     //Because the controller set_text function only accepts strings
//     tempReturn = to_string(avgTempTotal);

//     //Text Display
//       controller.set_text(0, 0, "DT: " + tempReturn + "F ");

    
//   }
// }



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
void intakeControl() {
  if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
    intakeHood.move_voltage(12000);
    intakeFront.move_voltage(12000);
  } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
    intakeHood.move_voltage(-12000);
    intakeFront.move_voltage(12000);
  } else {
    intakeHood.move_voltage(0);
    intakeFront.move_voltage(0);
  }
}

//Driver Control Pneumatics Code
void pneumaticsControl() {
  // utility arm 
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
    utilToggle = !utilToggle;
    utilArm.set_value(utilToggle);
  }
  //mogo clamp
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
    mogoToggle = !mogoToggle;
    mogo1.set_value(mogoToggle);
    mogo2.set_value(mogoToggle);
  }
  //intake piston
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
    intakePistonToggle = !intakePistonToggle;
    intakeLift.set_value(intakePistonToggle);
  }
  //wallstake pistons
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
    wallToggle = !wallToggle;
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