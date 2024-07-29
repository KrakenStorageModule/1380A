#include "main.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/motor_group.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
using pros::Controller;
Controller controller(pros::E_CONTROLLER_MASTER);
using pros::delay;
using pros::E_CONTROLLER_DIGITAL_DOWN;
using pros::E_CONTROLLER_DIGITAL_L2;

//Initialize Intake
pros::MotorGroup intake({7, -8});


//Initialize Pneumatics
pros::adi::DigitalOut backWings ('A');
pros::adi::DigitalOut wingsR ('B');
pros::adi::DigitalOut wingsL ('C');
pros::adi::DigitalOut hang1 ('D');
pros::adi::DigitalOut hang2 ('E');

//toggle for wings
bool backWingsToggle = false;
bool wingsLToggle = false;
bool wingsRToggle = false;


//intake driver control function
 void intakeControl(){
    while (true) {
		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
				intake.move_voltage(1200);
				} else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
				intake.move_voltage(-1200);
				} else {
				intake.move_voltage(0);
				}
		delay(10);
	}
 }
//Hang Driver Control Function
void hangControl(){
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
			hang1.set_value(true);
		}
                if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)) {
                        hang1.set_value(false);
			hang2.set_value(false);
                }
}

//BackWings Driver Control Function
void backWingsControl(){
     if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){
					backWingsToggle = !backWingsToggle;
					backWings.set_value(backWingsToggle);
				}
}

//Left Wing Driver Control Function
void wingsLControl(){
if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
					wingsLToggle = !wingsLToggle;
					wingsL.set_value(wingsLToggle);
				}
}

//Right Wing Driver Control Function
void wingsRControl(){
                                if (controller.get_digital_new_press(
                                        E_CONTROLLER_DIGITAL_L2)) {
                                        wingsRToggle = !wingsRToggle;
					wingsR.set_value(wingsRToggle);
                                }
}


//Big piston loop
void pistonsTask(){
	while(true){
		hangControl();
		backWingsControl();
		wingsLControl();
		wingsRControl();
    	delay(10);
    }
}


//High Stakes Pneumatics
//pros::adi::DigitalOut mogo1 ('A');
//pros::adi::DigitalOut mogo2 ('B');
//  pros::adi::DigitalOut lift1 ('C');
//  pros::adi::DigitalOut lift2 ('D');
// pros::adi::DigitalOut passiveHang1 ('E');
// pros::adi::DigitalOut passiveHang2 ('F');
// pros::adi::DigitalOut stealArm ('G');
// pros::adi::DigitalOut stealArmClamp ('H');
 //pros::adi::DigitalOut pissTake ('I');
//High Stakes Toggles
// bool liftToggle = false;
//bool clampToggle = false;
//bool stealArmToggle = false;
//bool stealArmClampToggle = false;
//bool pissTakeToggle = false;
//  High Stakes Intake
//pros::Motor intakeTop(18, pros::v5::MotorGears::blue);
//pros::Motor intakeBottom(17, pros::v5::MotorGears::blue);

//High Stakes Intake Driver Control Function
	// void intakeControl(){
	// 	if(liftToggle == true && controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
	// 		intakeTop.move_voltage(12000);
	// 	}else if(liftToggle == false && controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
	// 		intakeTop.move_voltage(12000);
	// 		intakeBottom.move_voltage(12000);
	// 	}else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
	// 		intakeTop.move_voltage(-12000);
	// 		intakeBottom.move_voltage(-12000);
	// 	}else{
	// 		intakeTop.move_voltage(0);
	// 		intakeBottom.move_voltage(0);
	// 	}
	// }


//High Stakes Mogo Driver Control Function
// void mogoControl(){
//      if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)) 
// 	 {
//         clampToggle = !clampToggle;
// 		mogo1.set_value(clampToggle);
// 		mogo2.set_value(clampToggle);
//      }
// }

//High Stakes PissTake Driver Control Function
// void pissTakeControl(){
//      if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)) 
// 	 {
//         pissTakeToggle = !pissTakeToggle;
// 		pissTake.set_value(pissTakeToggle);
// 		pissTake.set_value(pissTakeToggle);
//      }
// }

//High Stakes Intake Lift Driver Control Function
// void intakeLiftControl(){
//      if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)) 
// 	 {
//      liftToggle = !liftToggle;
// 		lift1.set_value(liftToggle);
// 		lift2.set_value(liftToggle);
//      }
// }

//High Stakes Hang Driver Control Function
// void hangControl(){
//      if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)) 
// 	    {
//          liftToggle = !liftToggle;
// 			passiveHang.set_value(liftToggle);
//		    passiveHang.set_value(liftToggle);
//      }
// }

//High Stakes Steal Arm Driver Control Function
// void stealControl(){
//      if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)) 
// 	    {
//          stealArmToggle = !stealArmToggle;
// 			stealArm.set_value(stealArmToggle);
//      }
//      if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)) 
// 	    {
//          stealArmClampToggle = !stealArmClampToggle;
// 			stealArmClamp.set_value(stealArmClampToggle);
//      }
// }

