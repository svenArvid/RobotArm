/*
  Joakim Carlsson
  
 */

class RobotArm
{
  public:
	// constructor
	RobotArm(Motor motor_arr[], Claw claw);
	
	int Get_ref(void);
	void Set_ref(int r);
	int Get_pos(void);
	int Get_dir(void);
	
	/* Update pos when positive edge detected. If no edges are detected for some time: reset in_motion */
	void Update_position(void);
	
	void Stop_motor(void);
	
	/* Calculate control signal. If in_motion in the wrong direction: stop motor. When no longer in_motion: Restart */
	void Calculate_control(void);
		
  private:
	int ENABLE_PIN;
	int FORWARD_PIN;
	int BACKWARD_PIN;
	int SENSOR_PIN;
	
	int ref;
	float Kp;
	int K_kick;
	int u;	
	volatile int sensor, prev_sensor, watchdog_timer, pos, dir;
};
