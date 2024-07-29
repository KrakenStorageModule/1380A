#include "fmt/core.h"
#include "fmt/format.h"
#include "lemlib/chassis/chassis.hpp"
#include "liblvgl/misc/lv_async.h"
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include <algorithm>
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "robodash/api.h"
#include "robodash/views/console.hpp"
#include "robodash/views/selector.hpp"
#include "robodash/core.h"
#include "devices.cpp"
using pros::delay;
using pros::Task;
using pros::c::delay;
using rd::Console;
//creating a console
Console console;

//Lemlib
//left side of drive
pros::MotorGroup right_motor_group({4, -5, 6}, pros::MotorGears::blue);
//right side of drive
pros::MotorGroup left_motor_group({-1, 2, -3}, pros::MotorGears::blue);


// drivetrain settings
lemlib::Drivetrain drivetrain(&left_motor_group, // left motor group
                              &right_motor_group, // right motor group
                              10, // 10 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 4" omnis
                              360, // drivetrain rpm is 360
                              2 // horizontal drift is 2 (for now)
);

// imu
pros::Imu imu(10);
// // horizontal tracking wheel encoder
// pros::Rotation horizontal_encoder(20);
// // vertical tracking wheel encoder
// pros::adi::Encoder vertical_encoder('C', 'D', true);
// // horizontal tracking wheel
// lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_275, -5.75);
// // vertical tracking wheel
// lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_275, -2.5);

// odometry settings
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr,// horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);
// create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors // odometry sensors
);

//Auton Functions For High Stakes
//Intake Auton Function; autonIntake(true); => intakes
// void autonIntake(bool check){
//     if(check == true){
//         intakeBottom.move_voltage(12000);
//         intakeTop.move_voltage(12000);
//     }else {
//         intakeBottom.move_voltage(-12000);
//         intakeTop.move_voltage(-12000);   
//     }
// }

//Stops the intake in diff way
//check = 0 => full intake stop
//check = 1 => top intake stop
//check = 2 => bottom intake stop
// void intakeKill(int check){
//     if(check == 0){
//         intakeBottom.move_voltage(0);
//         intakeTop.move_voltage(0);
//     }else if(check == 1){
//         intakeTop.move_voltage(0);   
//     }else if(check == 2){
//         intakeBottom.move_voltage(0);
//     }
// }
//auton mogo clamp
// void autonMogo(){
//     clampToggle = ! clampToggle;
//     mogo1.set_value(clampToggle);
//     mogo2.set_value(clampToggle);
// }

// //auton pisstake functions
// void autonPissTake(){
//     pissTakeToggle = ! pissTakeToggle;
//     pissTake.set_value(pissTakeToggle);
//     pissTake.set_value(pissTakeToggle);
// }

//Values used by the following fuctions to track and subsequently display odom values
float xValue = 0;
float yValue = 0;
float thetaValue = 0;
//odometry tracking displayed on screen
void trackOdom(){
    while (true) {
        xValue = chassis.getPose().x;
        yValue = chassis.getPose().y;
        thetaValue = chassis.getPose().theta;     
        console.clear();
        console.println("X: " + fmt::to_string(xValue));
        console.println("Y: " + fmt::to_string(yValue));
        console.println("Theta: " + fmt::to_string(thetaValue));
        delay(25);
    }

}

//AUTONS

//forward/back tuning pid auton
void lateralPIDTune() {
    console.println("Running Auton:Lateral PID!");
    //displays location while letting the rest of the auton run
    Task odomTrack(trackOdom);
    // set position to x:0, y:0, heading:0
    chassis.setPose(0, 0, 0);
    // turn to face heading 90 with a very long timeout
    chassis.turnToHeading(90, 100000);
}

//turning tuning pid auton
void turningPIDTune(){
    console.println("Running Auton:Turning PID!");
    //displays location
    Task odomTrack(trackOdom);
    // set position to x:0, y:0, heading:0
    chassis.setPose(0, 0, 0);
    // move 48" forwards
    chassis.moveToPoint(0, 48, 10000);
} 

void skills(){

}
//high stakes soloAWP
void BlueSoloAWPRight(){
    // //sets initial location
    // chassis.setPose(-59.547,-7.823,0);
    // //move to right in front of the alliance stake while intaking
    // chassis.moveToPoint(-59.12, 0.282, 500, {.forwards = true}, true);
    // //chassis.moveToPose(-59.12, 0.282, 90, 500);
    // intakeBottom.move_velocity(12000);
    // //turn to face away from wall stake
    // chassis.turnToHeading(90, 500);
    // //slows down bottom intake so that it doesn't burn out
    // intakeBottom.move_velocity(600);
    // chassis.moveToPoint(-65, 0.282, 500, {.forwards = false}, true);    
    // //run top intake
    // intakeTop.move_voltage(12000);
    // delay(150);
    // //Kills the top intake,and moves backwards to the goal before clamping on
    // intakeKill(0);
    // chassis.moveToPose(-29.374, -20.913, 300, 500, {false, 2, .3, 127, 127, 0}, false);
    // autonMogo();
    // //moves to ring while running intake
    // chassis.moveToPoint(-45.222, -44.452, 500, {true}, true);
    // autonIntake(true);
    // chassis.turnToHeading(95, 500);
    // autonPissTake();
    // //moves to ring stack and intakes the top
    // chassis.moveToPoint(-29.055, -46.776, 500, {true}, false);
    // autonPissTake();
    // delay(50);
    // intakeKill(0);
    // //move to slightly before the ladder and stops everything
    // chassis.moveToPoint(-15.805, -18.838, 500);
    // autonMogo();
    // //moves to touch the ladder
    // chassis.moveToPoint(-13.25, -12.771, 500);
}

void BlueSoloAWPLeft(){
    // chassis.setPose(-59.068,7.823,180);
    // //intake ring and move in front of stake
    // intakeBottom.move_voltage(600);
    // chassis.moveToPoint(-59.228, 0, 500);
    // //turn to face away from stake
    // chassis.turnToHeading(90, 500);
    // chassis.moveToPoint(-59.228, 0, 500, {false}, true);   
    // autonIntake(true);
    // delay(500);
    // intakeKill(0);
    // autonPissTake();
    // //move to the stack, kick out top ring and intake bottom
    // chassis.moveToPoint(-52.842, 0, 500, {false}, true);
    // intakeBottom.move_voltage(-12000);
    // autonIntake(true);
    // autonPissTake();
    // //finally move to mogo lol
    // chassis.moveToPose(-29.853, 20.275, 230, 500, {false}, false);
    // intakeKill(0);
    // autonMogo();
    // //intake top ring of the next stack
    // //idk if bot will be able to intake the top of stacks, but hopefully so this wont be super hard
    // chassis.moveToPoint(-24.106, 43.902, 500, {true}, true);
    // autonPissTake();
    // autonIntake(true);
    // //might need to tune the following delay/ kick-out the red ring
    // delay(100);
    // //intakes the top 2 stacks
    // chassis.moveToPose(-8.621, 51.086, 90, 500);
    // // autonPissTake();
    // autonIntake(true);
    // chassis.moveToPose(-9.419, 44.062, 90, 500);


} 

void RedSoloAWPLeft(){
    chassis.setPose(-59.547,-7.823,0);
} 
void RedSoloAWPRight(){
    chassis.setPose(-65.294,-7.823,0);
} 

//ACTUAL SELECTOR
rd::Selector selector
({{"lateralPIDTune", &lateralPIDTune},
{"turningPIDTune", &turningPIDTune},
{"Skills", &skills},
{"BlueSoloAWPRight", BlueSoloAWPRight},
{"BlueSoloAWPLeft", BlueSoloAWPLeft},
{"RedSoloAWPRight", RedSoloAWPRight},
{"RedSoloAWPLeft", RedSoloAWPLeft},

});