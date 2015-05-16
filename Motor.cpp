/** 
Class Motor, for position control of motor
**/
#include "General.h"
#include "Motor.h"
Motor::Motor(int en_pin, int A1_pin, int A2_pin, int sens_pin, float K_prop) :	// class constructor
    ENABLE_PIN(en_pin),	FORWARD_PIN(A1_pin), BACKWARD_PIN(A2_pin), SENSOR_PIN(sens_pin) // use of member initialization list, C++ direkt, p. 232			
{
	// initialize the digital pins.
	pinMode(ENABLE_PIN, OUTPUT);
	pinMode(FORWARD_PIN, OUTPUT);
	pinMode(BACKWARD_PIN,OUTPUT);  
	digitalWrite(ENABLE_PIN, LOW);  
	digitalWrite(FORWARD_PIN, LOW);
	digitalWrite(BACKWARD_PIN,LOW);
	
	pinMode(SENSOR_PIN, INPUT);
		
	pos = 0;			// MAYBE Set to an in parameter?
	K_kick = 200;		// // MAYBE Set to an in parameter?
	Kp = K_prop;
	
	ref = pos;
	u = 0;	
	dir = 0;
	dead_band = 1;
		
	sensor = digitalRead(SENSOR_PIN);
	prev_sensor = sensor;
	watchdog_timer = 0;
}

void Motor::Update_position() 
{
	if(dir != 0){
		sensor = digitalRead(SENSOR_PIN);
			  
		watchdog_timer++;
		if(sensor == HIGH && prev_sensor == LOW) // positive edge detected
		{
			pos += dir;
			watchdog_timer = 0;
		}
		else if(watchdog_timer > TIMEOUT){
			watchdog_timer = 0;
			dir = 0;
			Stop_motor();
			Serial.print("pos: "); Serial.println(pos);
		}
	}
	
	prev_sensor = sensor;
}  // end of UpdatePosition

void Motor::Stop_motor(){
	u = 0;
    digitalWrite(FORWARD_PIN,LOW);
    digitalWrite(BACKWARD_PIN,LOW);	
	digitalWrite(ENABLE_PIN, LOW);  
}

void Motor::Calculate_control(){
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
	
	else{ // Calculate control signal
		u = (int) Kp*(ref - pos);  
		int kick = Sign(u)*K_kick * (dir == 0);	// Add kick if dir is 0
		u = Sign(u)*min(abs(u + kick), 255);    // Saturate control signal
    
		if( u >= 0 ){                			// Run forward
			digitalWrite(FORWARD_PIN,HIGH);
			digitalWrite(BACKWARD_PIN,LOW);	
			dir = 1;	
		}
		else{                            		// Run backward
			digitalWrite(FORWARD_PIN,LOW);
			digitalWrite(BACKWARD_PIN,HIGH);
			dir = -1;
		}
		analogWrite(ENABLE_PIN, abs(u));  	
	}
	Serial.print("u: "); Serial.println(u);
}