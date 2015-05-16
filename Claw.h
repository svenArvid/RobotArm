/*
  Joakim Carlsson
  
 */

enum claw_state {OPEN, CLOSED};	

class Claw
{
  public:
	// constructor
	Claw(int en_pin, int A1_pin, int A2_pin, claw_state init_pos);
	
	claw_state Get_ref(void);
	void Set_ref(claw_state r);
	claw_state Get_pos(void);
		
	void Stop_motor(void);
	
	/* Calculate control signal. If at wrong position move motor until a timeout. Then stop motor and update position */
	void Calculate_control(void);
  
  private:
	int ENABLE_PIN;
	int FORWARD_PIN;
	int BACKWARD_PIN;
	
	claw_state pos, ref; 
	int motion_counter;
};
