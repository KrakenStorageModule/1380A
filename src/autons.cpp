//This file is for everything to do with the autonomous period
//It contains all your autons, auton functions (used for pneumatics mostly to flip the toggles)
//As well as LemLib settings and Robodash features

#include "autons.h" //This references the header file associated with this .cpp file

#include "lemlib/asset.hpp"

#include "devices.h"

#include "globals.h"

#include "robodash/views/selector.hpp"

using pros::delay;
using rd::Console;

//Define the Robodash Console Here
Console console;

//DEFINE JERRY.IO PATHS HERE
//(and make a note of what auton they are used in pls)
ASSET(straight_txt); //for Fun auton
ASSET(curves_txt); //for Fun auton

//Lemlib Time!

//DT MotorGroups Go Here
pros::MotorGroup left_motor_group({
  -1,
  -3,
  -4
}, pros::MotorGears::blue);
pros::MotorGroup right_motor_group({
  5,
  6,
  7
}, pros::MotorGears::blue);

// Drivetrain Settings -> Input info about DT to get more accurate 
lemlib::Drivetrain drivetrain( & left_motor_group, // left motor group
  &
  right_motor_group, // right motor group
  13, // 12.5 inch track width
  lemlib::Omniwheel::NEW_325, // using new 3.35" omnis
  450, // drivetrain rpm is 450
  2 // horizontal drift is 2 (for now)
);

// imu
pros::Imu imu(13);
//Defining Odom Pods
//MAY NEED TO REVERSE TRACKERS USING (port, true);
// horizontal tracking wheel encoder
pros::Rotation horizontal_encoder(20);
// vertical tracking wheel encoder
pros::Rotation vertical_encoder(19);
// horizontal tracking wheel
lemlib::TrackingWheel horizontal_tracking_wheel( & horizontal_encoder, lemlib::Omniwheel::NEW_275, -3);
// vertical tracking wheel
lemlib::TrackingWheel vertical_tracking_wheel( & vertical_encoder, lemlib::Omniwheel::NEW_275, 1);

//Setting Up Odometry
lemlib::OdomSensors sensors( & vertical_tracking_wheel, // vertical tracking wheel 1
  nullptr, //Extraneous extra vertical tracking wheel
  &
  horizontal_tracking_wheel, // horizontal tracking wheel 1
  nullptr, //Extraneous extra horizontal tracking wheel
  &
  imu // inertial sensor
);

//Lateral PID Settings (Forward/Back Motion Controller)
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
  0, // integral gain (kI)
  25, // derivative gain (kD)
  0, // anti windup
  0, // small error range, in inches
  0, // small error range timeout, in milliseconds
  0, // large error range, in inches
  0, // large error range timeout, in milliseconds
  90 // maximum acceleration (slew)
);

// Angular PID Settings (Turning Motion Controller)
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
  0, // integral gain (kI)
  15, // derivative gain (kD)
  3, // anti windup
  1, // small error range, in inches
  100, // small error range timeout, in milliseconds
  3, // large error range, in inches
  500, // large error range timeout, in milliseconds
  0 // maximum acceleration (slew)
);

// create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
  lateral_controller, // lateral PID settings
  angular_controller, // angular PID settings
  sensors // odometry sensors
);

//Auton Functions (Functions to be used IN auton routes)
//I use these because it makes sure the piston state and toggle state stay in sync
//this is just easier than flipping the toggle bool and THEN changing the piston state 

//for controller hud => checks whether to display the dt temp or auton
bool autonRan = false;
//auton control functions
void autonMogo() {
  mogoToggle = !mogoToggle;
  mogo1.set_value(mogoToggle);
  mogo2.set_value(mogoToggle);
}
void autonWall() {

  wallToggle = !wallToggle;
  wallArm1.set_value(wallToggle);
  wallArm2.set_value(wallToggle);
}
void autonLift() {
  intakePistonToggle = !intakePistonToggle;
  intakeLift.set_value(intakePistonToggle);
}
//(too lazy to move this lol)
bool intakeState = false;
void autonIntake() {
  intakeState = !intakeState;
  if (intakeState == true) {
    intakeFront.move_voltage(12000);
    intakeHood.move_voltage(12000);
  } else if (intakeState == false) {
    intakeFront.move_voltage(-12000);
    intakeHood.move_voltage(-12000);
  }
}

//This uses Robodash to display where the robot thinks it currently is
//Variables for the trackOdom(); function
float xValue = 0;
float yValue = 0;
float thetaValue = 0;
void trackOdom() {
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

//Write Your Autons Below :)
void lateralPIDTune() {
  autonRan = true;

  //Shifts to a blank screen where position is being displayed
  console.focus();
  pros::Task odomTask(trackOdom);
  // set position to x:0, y:0, heading:0
  chassis.setPose(0, 0, 0);
  // move 24" forwards
  chassis.moveToPoint(0, 24, 2000, {}, false);

  chassis.turnToHeading(180, 10000);
}

void turningPIDTune() {
  autonRan = true;

  //Shifts to a blank screen where position is being displayed
  console.focus();
  pros::Task odomTask(trackOdom);
  // set position to x:0, y:0, heading:0
  chassis.setPose(0, 0, 0);
  // turn to face heading 90 with a very long timeout
  chassis.turnToHeading(90, 100000);
}
void soloAWP() {
  autonRan = true;

}
//Auton Selector -> You store your autons here as a vector so the auton selector can access them
//It follows this format ; 
//rd::Selector name ({"auton name", auton name}, {"auton name", auton name}});
rd::Selector selector({
  {
    "lateralPIDTune",
    & lateralPIDTune
  },
  {
    "turningPIDTune",
    & turningPIDTune
  },
  {
    "soloAWP",
    & soloAWP
  }
});