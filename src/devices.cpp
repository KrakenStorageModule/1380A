#include "main.h"
#include "devices.h"

//Initialize Controller here
pros::Controller controller(pros::E_CONTROLLER_MASTER);

//Initialize Motors/MotorGroups Here
pros::MotorGroup intake({8,-9}, pros::v5::MotorGears::blue);

//Initialize Pneumatics Here
pros::adi::DigitalOut backWings('A');
pros::adi::DigitalOut wingsL('F');
pros::adi::DigitalOut wingsR('C');
pros::adi::DigitalOut hang1('E');
pros::adi::DigitalOut hang2('H');

 //Variables for ControllerHUD(); -> Move these out of devices at some point lol
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
    bool backToggle = false;
    bool rightToggle = false;
    bool leftToggle = false;

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

void hang(){
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
        hang1.set_value(true);
        hang2.set_value(true);
    }else if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)){
        hang1.set_value(false);
        hang2.set_value(false);
    }
    }

void pneumaticsControl(){
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
        backToggle = !backToggle;
        backWings.set_value(backToggle);
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
        rightToggle = !rightToggle;
        wingsR.set_value(rightToggle);
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
        leftToggle = !leftToggle;
        wingsL.set_value(leftToggle);
    }
    }