/** 
Class Claw, Close and open claw. Remember claw state
**/
#include "General.h"
#include "Claw.h"
Claw::Claw(int en_pin, int A1_pin, int A2_pin, claw_state init_pos)	// class constructor
{
	ENABLE_PIN = en_pin;
	FORWARD_PIN = A1_pin;
	BACKWARD_PIN = A2_pin;
	pos = init_pos; 
	ref = pos;
	motion_counter = 0;
	
	// initialize the digital pins.
	pinMode(ENABLE_PIN, OUTPUT);
	pinMode(FORWARD_PIN, OUTPUT);
	pinMode(BACKWARD_PIN,OUTPUT);  
	digitalWrite(ENABLE_PIN, LOW);  
	digitalWrite(FORWARD_PIN, LOW);
	digitalWrite(BACKWARD_PIN,LOW);
}

claw_state Claw::Get_ref(void){
	return ref;	
}
void Claw::Set_ref(claw_state r){
	ref = r;
}

claw_state Claw::Get_pos(void){
	return pos;
}

void Claw::Stop_motor(void){
    digitalWrite(FORWARD_PIN,LOW);
    digitalWrite(BACKWARD_PIN,LOW);	
	digitalWrite(ENABLE_PIN, LOW);	
		
}

void Claw::Calculate_control(){
	if( pos != ref ){      			// Move claw
		switch(ref){
			case CLOSED:			// then open...
				digitalWrite(FORWARD_PIN,HIGH);
				digitalWrite(BACKWARD_PIN,LOW);	
				break;
			case OPEN:			// then close...
				digitalWrite(FORWARD_PIN,LOW);
				digitalWrite(BACKWARD_PIN,HIGH);	
				break;
		}
		motion_counter++;
		//analogWrite(ENABLE_PIN, 128);
		digitalWrite(ENABLE_PIN, HIGH);		
	}
	
	if(motion_counter >= 7 ){		// stop motor when arrived at correct position
		Serial.println("claw");
		Stop_motor();
		motion_counter = 0;
		pos = ref;
	}
	
}