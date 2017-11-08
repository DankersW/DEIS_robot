#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
/**
 *	Controller class.
 *  Contains instantiation of class inside source. Instance is declared extern in this header.
 */

#include <Arduino.h>
#include "types.h"
#include "encoder.h"
typedef struct pos_struct{
    double x;
    double y;
    double theta;
} pos_t;



typedef struct cmd_input{
  int speedOg;
  int distance;
} cmd_input_t;


 
class Controller {

public:
    static constexpr double WHEEL_BASE      = 18; //cm
    static constexpr double WHEEL_DIAMETER  = 6.5 ; //cm
    static constexpr double ERESOL          = 192;
    static constexpr double K1              = 0;
    static constexpr double K2              = 1;
    static const     int	LINETHRESHOLD	= 800; // Threshold value for IR sensors
    static const     int    ENCODER_MAX     = 0x7fff;
    
    double vel_lin = 0;
    double vel_max = 2;
    static const int CHANNEL = 1;
    pos_t waypoint;
    
    Controller(pos_t start = {0}, encoder_t encoders = {0});
    void updatePosition(encoder_t encoder_deltas);
    encoder_t updateEncoders(encoder_t encoder_deltas);


	/**
	 * General update function. Will behave different depending on controller state. 
	 */
    encoder_t update(encoder_t encoder_new, line_sensors_t line_sensors, int distance);


    void setWaypoint(pos_t i);
	bool startLaneChange(bool right, uint8_t rad_cm);
	void startLineFollow(int speed);
	void updateGPS(pos_t gps_pos);

	void driveSetDistance(uint32_t mm);
	void startLaneChange();

protected:
	enum State{
		IDLE = 0,
		LINE_FOLLOW = 1,
		WAYPOINT_FOLLOW = 2,
		LANE_CHANGE = 3//,
		//DRIVE_SET_DISTANCE_GPS = 3,
		//DRIVE_SET_DISTANCE_ODO = 4
	};

    pos_t position;
    encoder_t encoders;
    encoder_t reading;
    encoder_t velocity;
    State  state;
    line_sensors_t line_sensors = {0};
    Encoder left_encoder;
    Encoder right_encoder;
    float Ielines[3] = {0}; 
    double ego_cosys_ang = 0;
    double ego_delta = 0;
    double ego_theta = 0;
    double kappa = 0;
    double omega = 0;
    double d_x = 0;
    double d_y = 0;
    double r = 0;
	
	int target_speed;
	bool right_lane_change;
	uint64_t lane_change_start;
	encoder_t lane_change_speeds;
	/* STATE DEPENDENT UPDATE FUNCTIONS */
	encoder_t lineFollow(line_sensors_t sensor);
	encoder_t waypointFollow(line_sensors_t line_sensors);
	encoder_t laneChange(line_sensors_t line_sensors);
};


extern Controller controller;

#endif

