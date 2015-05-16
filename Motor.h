/*
  Joakim Carlsson
  
 */

const int TIMEOUT = 50;		// want this to be a class variable
	
class Motor
{
  public:
	// constructor
	Motor(int en_pin, int A1_pin, int A2_pin, int sens_pin, float K_prop=10.5);			// Use of default parameter
	
	inline int Get_ref()       { return ref; }	// Use inline function for short member functions, see C++ direkt, p. 223. 
	inline void Set_ref(int r) { ref = r; }		// Note: I gor error when I put the inline functions in the .cpp-file
	inline int Get_pos()       { return pos; }
	inline int Get_dir()       { return dir; }
	
	/* Update pos when positive edge detected. If no edges are detected for some time: reset in_motion */
	void Update_position();
	
	void Stop_motor();
	
	/* Calculate control signal. If in_motion in the wrong direction: stop motor. When no longer in_motion: Restart */
	void Calculate_control();
		
  private:
	const uint8_t 
	    ENABLE_PIN,
	    FORWARD_PIN,
	    BACKWARD_PIN,
	    SENSOR_PIN;
	
	int ref;
	float Kp;
	int K_kick;
	int u, sensor, prev_sensor;				// Signals used only in functions called in ISR.
	unsigned int dead_band, watchdog_timer;	
	volatile int pos, dir;					// Signals used both in ISR and in main loop, hence volatile.
};
