/*
  Joakim Carlsson, Contains base class Motor and derived classes HorizontalMotor and VerticalMotor
  
 */
//#include<string>

class Motor
{
  public:
  
    int writePos = 0;	// For debugging
    
	// constructor
	Motor(String n, int en_pin, int A1_pin, int A2_pin, int sens_pin, float K_prop=5.5, int K_kick_param=200);	// Use of default parameters
	virtual ~Motor() {};	// virtual destructor should always be present in a base class, see C++ direkt, p. 335
	
	inline int Get_ref() const  { return ref; }	 // Use inline function for short member functions, see C++ direkt, p. 223. 
	inline int Get_pos() const  { return pos; }  // Note: I gor error when I put the inline functions in the .cpp-file
	inline int Get_dir() const  { return dir; }  // Declared as const, i.e. member function does NOT alter any members (read only) 
	
	inline void Set_ref(int r) { ref = r; }	
	
	/* Manage robot position by reading motion sensor and call Update_position() when positive (or negative edge detected). 
	   If no edges are detected for some time, i.e. no movement: reset dir and call Stop_motor()  */
	void Manage_position();
	
	virtual void Update_position() = 0;		// Pure virtual function, i.e. class becomes abstract, see C++ direkt p. 336
	
	void Stop_motor();
	virtual void Print_info();		// Print info for debugging, declare as virtual to get dynamic binding, see C++ direkt, p. 322
	
	/* Manage motor control signal. If in_motion in the wrong direction: stop motor. When no longer in_motion: Restart */
	void Manage_control();
		
	virtual void Calculate_control() = 0; 	// Pure virtual function, i.e. class becomes abstract, see C++ direkt p. 336
		
  protected:
	String name;
	
	const uint8_t 
	    ENABLE_PIN,
	    FORWARD_PIN,
	    BACKWARD_PIN,
	    SENSOR_PIN;
	
	int ref;
	float Kp;
	int K_kick;
	int u, sensor, prev_sensor, dead_band;				// Signals used only in functions called in ISR.
	unsigned int watchdog_timer;	
	volatile int pos, dir;					// Signals used both in ISR and in main loop, hence volatile.
	
	static const int TIMEOUT = 60;			// class variable, wait time is TIMEOUT*5ms (sample time). 
											// 50-60 if triggering on both edges and 80 if triggering only on positive edge.
};


class VerticalMotor : public Motor 
{
  public:
	// constructor only calls base constructor
    VerticalMotor(String n, int en_pin, int A1_pin, int A2_pin, int sens_pin, float K_prop=5.5, int K_kick_param=200, float grav_param=1.5) : 
	Motor(n, en_pin, A1_pin, A2_pin, sens_pin, K_prop, K_kick_param), gravity_gain(grav_param) {} 			
	
	void Update_position();
	
	/* Calculate motor control signal. Note that since u is a class member nothing is returned from this function */
	void Calculate_control();	// Includes gravity compensation
		
  protected:
	float gravity_gain;
};


class HorizontalMotor : public Motor 
{
  public:
	// constructor calls base constructor first
    HorizontalMotor(String n, int en_pin, int A1_pin, int A2_pin, int sens_pin, float K_prop=5.5, int K_kick_param=200) : 
	Motor(n, en_pin, A1_pin, A2_pin, sens_pin, K_prop, K_kick_param) {backlash_pos = BACKLASH_MAX;} 			
	
	void Update_position();	// Updates position state and also backlash_pos
	
	void Calculate_control();
	void Print_info();		// Print info for debugging, declared as virtual in base class to get dynamic binding

  protected:
	int backlash_pos;
	
	static const int BACKLASH_MAX = 2;
	static const int BACKLASH_MIN = -3;
};
