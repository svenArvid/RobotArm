/** 
Contains base class Motor and derived classes HorizontalMotor and VerticalMotor, used for position control of motor.
**/
#include "General.h"
#include "Motor.h"

Motor::Motor(String n, int en_pin, int A1_pin, int A2_pin, int sens_pin, float K_prop, int K_kick_param) :	// class constructor
    name(n), ENABLE_PIN(en_pin), FORWARD_PIN(A1_pin), BACKWARD_PIN(A2_pin), SENSOR_PIN(sens_pin) // use of member initialization list, C++ direkt, p. 232			
{
	// initialize the digital pins.
	pinMode(ENABLE_PIN, OUTPUT);
	pinMode(FORWARD_PIN, OUTPUT);
	pinMode(BACKWARD_PIN,OUTPUT);  
	digitalWrite(ENABLE_PIN, LOW);  
	digitalWrite(FORWARD_PIN, LOW);
	digitalWrite(BACKWARD_PIN,LOW);
	
	pinMode(SENSOR_PIN, INPUT_PULLUP);
		
	K_kick = K_kick_param;		
	Kp = K_prop;
	
	pos = 0;					// MAYBE Set to an in parameter?
	ref = pos;
	u = 0;	
	dir = 0;
	dead_band = 1;
		
	sensor = digitalRead(SENSOR_PIN);
	prev_sensor = sensor;
	watchdog_timer = 0;
}

void Motor::Manage_position() 
{
	sensor = digitalRead(SENSOR_PIN);
	if(dir != 0){
					  
		watchdog_timer++;
		//if(sensor == HIGH && prev_sensor == LOW) // positive edge detected
		if (sensor != prev_sensor) 				   // positive OR negative edge detected, i.e. movement detected
		{
			Update_position();	
		}
		else if(watchdog_timer > TIMEOUT){			// i.e. NO movement
			watchdog_timer = 0;
			dir = 0;
			Stop_motor();
						
			if(DEBUGMODE) {	Serial.print(name); Serial.print(" pos: "); Serial.println(pos); }
		}
	}
	
	if (writePos==1){
		Print_info(); Serial.println();
	}
	prev_sensor = sensor;
}  // end of Manage_position

void Motor::Stop_motor(){
	u = 0;
    digitalWrite(FORWARD_PIN,LOW);
    digitalWrite(BACKWARD_PIN,LOW);	
	digitalWrite(ENABLE_PIN, LOW);  
}

void Motor::Print_info(){
	Serial.print(name); 	Serial.print('\t');
	Serial.print(sensor); 	Serial.print('\t');
	Serial.print(pos); 		Serial.print('\t');
	Serial.print(u); 		Serial.print('\t');
	Serial.print(watchdog_timer);
}

void Motor::Manage_control(){
	// Stop motor if moving in wrong direction
	if( dir != 0 && Sign(dir) != Sign(ref-pos) ) {
		Stop_motor();
		return;
	}
	
	// Stop motor if close to target position (dead band)
	if(abs(ref - pos) <= dead_band){
		Stop_motor();
		return;
	}
	
	else{ 
		Calculate_control(); 	// Calculate control signal u		
		
		if( u > 0 ){                			// Run forward
			digitalWrite(FORWARD_PIN,HIGH);
			digitalWrite(BACKWARD_PIN,LOW);	
			dir = 1;	
		}
		else if( u < 0 ){                       // Run backward
			digitalWrite(FORWARD_PIN,LOW);
			digitalWrite(BACKWARD_PIN,HIGH);
			dir = -1;
		}
		analogWrite(ENABLE_PIN, abs(u));  	
	}
	if(DEBUGMODE) {	Serial.print("u: "); Serial.println(u); }
}


/* ======================= class VerticalMotor =======================   */

void VerticalMotor::Update_position(){
	pos += dir;
	watchdog_timer = 0;
}

void VerticalMotor::Calculate_control(){
	int u_temp = (int) Kp*(ref - pos);  
	int kick = Sign(u_temp)*K_kick * (dir == 0);	// Add kick if dir is 0
	
	if( (ref - pos) > dead_band){		// gravity compensation, i.e. larger u when going up.
		u_temp *= gravity_gain;
		kick   *= gravity_gain;
	}
	
	u = Sign(u_temp)*min(abs(u_temp + kick), 255);    // Saturate control signal
}


/* ======================= class HorizontalMotor =======================   */

void HorizontalMotor::Update_position(){
	if ( (dir == 1 && backlash_pos == BACKLASH_MAX) || (dir == -1 && backlash_pos == BACKLASH_MIN) ){
		pos += dir;		// Make sure backlash position is correct before updating pos.
	}
	else{
		backlash_pos += dir;	
	}
			
	watchdog_timer = 0;
}

void HorizontalMotor::Calculate_control(){
	u = (int) Kp*(ref - pos);  
	int kick = Sign(u)*K_kick * (dir == 0);	// Add kick if dir is 0
	u = Sign(u)*min(abs(u + kick), 255);    // Saturate control signal
}

void HorizontalMotor::Print_info(){
	Motor::Print_info();
	Serial.print('\t'); Serial.print(backlash_pos);
}
