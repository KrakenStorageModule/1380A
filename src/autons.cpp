#include "autons.h"
#include "lemlib/asset.hpp"
pros::MotorGroup left_motor_group({-1, -3, -4}, pros::MotorGears::blue);
// right motor group
pros::MotorGroup right_motor_group({5, 6, 7}, pros::MotorGears::blue);

using pros::delay;
using rd::Console;
Console console;
//Define Jerry.IO Paths here! 
//(and make a note of what auton they are used in)
ASSET(straight_txt); //for Fun auton
ASSET(curves_txt);//for Fun auton

// drivetrain settings
lemlib::Drivetrain drivetrain(&left_motor_group, // left motor group
                              &right_motor_group, // right motor group
                              12.5, // 12.5 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.35" omnis
                              450, // drivetrain rpm is 450
                              2 // horizontal drift is 2 (for now)
);

    //odom values for the tracker function
    float xValue = 0;
    float yValue = 0;
    float thetaValue = 0;

// imu
pros::Imu imu(13);
// horizontal tracking wheel encoder
// pros::Rotation horizontal_encoder(20);
// vertical tracking wheel encoder
// pros::adi::Encoder vertical_encoder('C', 'D', true);
// horizontal tracking wheel
// lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_275, -5.75);
// vertical tracking wheel
// lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_275, -2.5);

// odometry settings
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// lateral PID controller
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

// angular PID controller
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

//autons
void lateralPIDTune(){
    console.focus();
    pros::Task odomTask(trackOdom);
    // set position to x:0, y:0, heading:0
    chassis.setPose(0, 0, 0);
    // move 24" forwards
    chassis.moveToPoint(0, 24, 2000, {}, false);

    chassis.turnToHeading(180, 10000);
}

void turningPIDTune(){
    console.focus();
    pros::Task odomTask(trackOdom);
     // set position to x:0, y:0, heading:0
    chassis.setPose(0, 0, 0);
    // turn to face heading 90 with a very long timeout
    chassis.turnToHeading(90, 100000);
}

void fun(){
    console.focus();
    pros::Task odomTask(trackOdom);
//      // set position to x:0, y:0, heading:0
//     chassis.setPose(0, 0, 0);
//     chassis.moveToPoint(0, 15, 1000, { .earlyExitRange = 0}, true);
//     chassis.turnToHeading(90, 1000, { .earlyExitRange = 0}, true);
//     chassis.moveToPoint(10, 30, 1000, {.minSpeed = 127, .earlyExitRange = 2}, true);
//     chassis.turnToHeading(270, 1000, {.earlyExitRange = 0}, true);
//     chassis.moveToPoint(0, 0, 1000, {.minSpeed = 127, .earlyExitRange = 2}, true);
//     chassis.swingToHeading(0, lemlib::DriveSide::LEFT, 1000, {.earlyExitRange = 0}, false);
//    // chassis.waitUntilDone();
//     chassis.setPose(0,0,0);
//     chassis.moveToPoint(0, 36,  1000, { .earlyExitRange = 0}, true);
//     chassis.moveToPoint(0, 0,  1000, {.forwards = false, .earlyExitRange = 0}, false);
//     chassis.setPose(0,0,0);
//     chassis.moveToPoint(-25, 40, 1000, { .earlyExitRange = 0}, true);
//     chassis.turnToHeading(0, 1000, {.earlyExitRange = 0}, false);
//     //chassis.waitUntilDone();
//     chassis.setPose(0,0,0);
//     chassis.moveToPoint(0, -38, 1000, {.forwards = false, .earlyExitRange =0}, false);
//     chassis.follow(straight_txt, 10, 5000, true, true);
//     chassis.moveToPoint(0 ,0,3000, {.earlyExitRange = 0}, true);
chassis.setPose(0,0,0);
chassis.follow(curves_txt, 15, 500000, true, false);
//chassis.swingToHeading(180, lemlib::DriveSide::RIGHT, 1000, {.earlyExitRange = 0}, true);
}


//ACTUAL SELECTOR
rd::Selector selector({{"lateralPIDTune", &lateralPIDTune},
{"turningPIDTune", &turningPIDTune},
{"fun", fun}
});