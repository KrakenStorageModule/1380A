#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include <algorithm>
#include <string>
#include "robodash/api.h"
#include "autons.cpp"
#include "pros/motor_group.hpp"
#include "pros/misc.hpp"
using fmt::to_string;
using pros::delay;
using pros::Task;
using pros::battery::get_capacity;
using std::string;

// initialize function. Runs on program startup
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors
	//Sets intake to coast so motors don't die
	intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // print position to brain screen
    
    //High Stakes Motors Set to Coast
    //intakeTop.set_Brake_Mode(coast);
    //intakeBottom.set_Brake_Mode(coast);
    //High Stakes Piston State Initialization 
    //stealArm.set_value(false);
    //stealArmClamp.set_value(false);
    //mogo1.set_value(false);
    //mogo2.set_value(false);
    //passiveHang1.set_value(false);
    //passiveHang2.set_value(false);
}
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
    console.println("Disabled! Closing All Pneumatics...");
	//closes hang and all wings
	hang1.set_value(false);
	hang2.set_value(false);
	backWings.set_value(false);
	wingsL.set_value(false);
	wingsR.set_value(false);
    //High Stakes Disable Code
    //stealArm.set_value(false);
    //stealArmClamp.set_value(false);
    //mogo1.set_value(false);
    //mogo2.set_value(false);
    //passiveHang1.set_value(false);
    //passiveHang2.set_value(false);
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */


void autonomous() {
    selector.run_auton();
    
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

//Prints out temperature of drivetrain 
//And the battery level
//onto the controller screen cuz why not
std::string avgDriveTemp(){
    double avgTempLeft = 0;
    double avgTempRight = 0;
    double avgTempTotal;
    std::string tempReturn = ""; 
        avgTempLeft = (left_motor_group.get_temperature(0) + left_motor_group.get_temperature(1) + left_motor_group.get_temperature(2))/3;
        avgTempRight = (right_motor_group.get_temperature(0) + right_motor_group.get_temperature(1) + right_motor_group.get_temperature(2))/3;
        avgTempTotal = (((avgTempLeft + avgTempRight)/2)*1.8)+32;
        return tempReturn = to_string(avgTempTotal);
}
std::string batteryDisplay() { 
    double batteryLevel = (get_capacity()/12.8)*100; 
    return to_string(batteryLevel);
    }

void controllerHUD(){
    string temp = avgDriveTemp();
    std::string batteryPercent = batteryDisplay();
    while(true){
    controller.set_text(0,0,"Drivetrain Temp: " + temp);
    controller.set_text(1,0,  + "Battery: " + batteryPercent + "%");
    delay(200);
    }
}

//Driver Control
void opcontrol() {
    //Task Managing All Pneumatics
    Task pistonTask (pistonsTask);
    //Task Managing The Controller
    Task controllerTask(controllerHUD);
    //Task Managing The Intake
    Task intakeTask(intakeControl);
    // Drivetrain Control Scheme is here
    while (true) {
        // Drivetrain Control Scheme is here
        // Curvature Drive
        //  get left y and right x positions
            int leftY =
            controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
            int rightX =
            controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the robot   
            chassis.curvature(leftY, rightX);
        // delay to save resources
             delay(25);
    }
}    
    

    // High StakesIntake Callback
    //     while(true)
	// 	{	
    //      intakeControl();
	// 	    delay(20);
	// 	}   

    //High Stakes Hang Pneumatics Callback
    // while (true) {
    //     hangControl();
    //     delay(20);
    // }

    //High Stakes Mogo Clamp Pneumatics Callback
    // while (true) {
    //     mogoControl();
    //     delay(20);
    // }
    //High Stakes Mogo Clamp Pneumatics Callback
    // while (true) {
    //     intakeLiftControl();
    //     delay(20);
    // }
    //High Stakes Mogo Clamp Pneumatics Callback
    // while (true) {
    //     intakeLiftControl();
    //     delay(20);
    // }
    //High Stakes Steal Arm/Steal Clamp Pneumatics Callback
    // while (true) {
    //     stealControl();
    //     delay(20);
    // }
    //High Stakes PissTake Pneumatics Callback
    // while (true) {
    //     pissTakeControl();
    //     delay(20);
    // }



