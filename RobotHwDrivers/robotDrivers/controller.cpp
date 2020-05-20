#include "controller.h"
#include "math.h"
#include "robot.h"
//#define M_PI 3.14159265358979323846

static inline int Sign(double Val) { return (Val > 0) ? 1 : ((Val < 0) ? -1 : 0); }
static double AngDiff(double A1, double A2){ return (A1-A2) + ((A1-A2) < -M_PI)*(2*M_PI) + ((A1-A2) > M_PI)*(-2*M_PI);} //roll-over

Controller::Controller(pos_t start, encoder_t encoders)
    : position(start)
    , encoders(encoders)
	, state(IDLE)
	, target_speed(0)
	, left_encoder(Encoder(-32768, 32767))
	, right_encoder(Encoder(-32768, 32767))
	, right_lane_change(false)
	, lane_change_start(0)
  , e(0)
  , last_e(0)
  , ed(0)
  , last_ed(0){

    velocity = {0};
    waypoint = {0};
}

void Controller::setWaypoint(pos_t i){;
  waypoint.x = i.x;
  waypoint.y = i.y;
}

void Controller::setStateToIdle(){
  state = IDLE;
}

int Controller::getState(){
  return (int)state;
}

void Controller::updateGPS(int16_t x, int16_t y, int16_t theta){
	position.x = x;
	position.y = y;
	position.theta = (double)theta / 1000;
}

void Controller::updatePosition(encoder_t encoder_deltas){
    double d_r            = M_PI * Controller::WHEEL_DIAMETER * (double(encoder_deltas.right)/ Controller::ERESOL);
    double d_l            = M_PI * Controller::WHEEL_DIAMETER * (double(encoder_deltas.left)/ Controller::ERESOL);
    double d_c            = (d_r + d_l)/2;
    double theta_change   = (d_r - d_l)/Controller::WHEEL_BASE;
    
    //Serial.print("\tpxd: " + String(cos(position.theta + (theta_change/2))) + "\tdr: " + String(d_r));
    
    position.x     += d_c * cos(position.theta + (theta_change/2));
    position.y     += d_c * sin(position.theta + (theta_change/2));
    position.theta += theta_change;

    //Serial.println("\tposX: " + String(position.x) + "\tposY: " + String(position.y) + "\tpos theta: " + String(position.theta)); 
}
pos_t Controller::getPosition(){
  return position;
}

/**
 *  Checks for overflow
 */
encoder_t Controller::updateEncoders(encoder_t encoder_deltas){
  static int32_t pos_r = 0;
  static int32_t pos_l = 0;

  //Serial.println("Encoder deltas R: " + String(encoder_deltas.right) + "\t L: " + String(encoder_deltas.left));

  pos_r += encoder_deltas.right;
  pos_l += encoder_deltas.left;

  //delta = new_count + range - last;

  if(abs(encoder_deltas.right)>Controller::ENCODER_MAX/2){
    pos_r += Sign(encoders.right)*2*(Controller::ENCODER_MAX+1);
  }

  if(abs(encoder_deltas.left)>Controller::ENCODER_MAX/2){
    pos_l += Sign(encoders.left)*2*(Controller::ENCODER_MAX+1);
  }

  encoder_t deltas = {pos_r-encoders.right, pos_l- encoders.left};

  encoders.right = pos_r;
  encoders.left = pos_l;  
  return deltas;
}

#if 0

encoder_t Controller::updateEncoders(encoder_t encoder_new){
  left_encoder.update(encoder_new.left);
  right_encoder.update(encoder_new.right);
  return {right_encoder.getDelta(), left_encoder.getDelta()};
}

#endif

encoder_t Controller::update(encoder_t encoder_new, line_sensors_t line_sensors, int distance, int v[2]){
	encoder_t deltas = encoder_new - encoders;
	deltas = updateEncoders(deltas);

	//Serial.println("delt-l: " + String(deltas.left) + " delt-r: " + String(deltas.right));
	updatePosition(deltas);

	avgWheelSpeed[0] = v[0];
	avgWheelSpeed[1] = v[1];

	if(distance < 15){ //object detected less then 20 cm in front
    //robot.angleScoop(0);
	  return{0};
	}
	
	// TODO: Implementing a proper state machine using inheritance would be more neat
	switch(state){
	case IDLE:
		break;
	case WAYPOINT_FOLLOW:
		return waypointFollow(line_sensors);
	case LINE_FOLLOW:
		return lineFollow(line_sensors,distance);
	case LANE_CHANGE:
		return laneChange(line_sensors);
	}
	return {0};
}


bool Controller::startLaneChange(bool right, uint8_t rad_cm){
  lane_change_speeds = {avgWheelSpeed[0], avgWheelSpeed[1]};
	//lane_change_speeds = {75,75} ;//robot.getWheelSpeeds(); 
	right_lane_change = right;
	lane_change_start = millis();
	state = LANE_CHANGE;
	return true;
}

encoder_t Controller::laneChange(line_sensors_t line_sensors){
  static bool lost_line = false;
  float x = 0.7;

//  if(millis() < lane_change_start + 750){
//    lost_line = false;
//    if(right_lane_change) // turn to right
//      return {lane_change_speeds.right + 30, lane_change_speeds.left};
//    else // turn to left
//      return { lane_change_speeds.right, lane_change_speeds.left  + 30};
//  }

  if(millis() < lane_change_start + 750){
    lost_line = false;
    if(right_lane_change) // turn to right
      return {lane_change_speeds.right, int(lane_change_speeds.left  * x)};
    else // turn to left
      return { int(lane_change_speeds.right * x), lane_change_speeds.left};
  }

  if(!lost_line){
    if(line_sensors.left < Controller::LINETHRESHOLD
      && line_sensors.middle < Controller::LINETHRESHOLD
      && line_sensors.right < Controller::LINETHRESHOLD){
      lost_line = true;
    }
  }

  if(lost_line){
    //if(line_sensors.left > Controller::LINETHRESHOLD
        //|| line_sensors.middle > Controller::LINETHRESHOLD
        //|| line_sensors.right > Controller::LINETHRESHOLD)
     if(line_sensors.middle > Controller::LINETHRESHOLD)
        state = LINE_FOLLOW;
  }
  return { target_speed , target_speed };

}

encoder_t Controller::waypointFollow(line_sensors_t line_sensors){
    d_x = waypoint.x - position.x;
    d_y = waypoint.y - position.y;
          
    ego_cosys_ang = atan2(d_y,d_x);
    ego_delta = AngDiff(position.theta,ego_cosys_ang);
    ego_theta = AngDiff(waypoint.theta,ego_cosys_ang);

    r = sqrt(pow(d_x, 2) + pow(d_y, 2));

    if (r < 10){
    	return {0};
    }

    //Serial.print("R: " + String(r) + "\tDx: " + String(d_x)+ "\tDy: " + String(d_y));
      
    kappa = -1/r*(Controller::K2*(ego_delta-atan(-Controller::K1*ego_theta))+ (1+Controller::K1/(1+(Controller::K1*pow(ego_theta,2))))*sin(ego_delta));
    vel_lin = vel_max/(1+10*abs(pow(kappa,2)));
    omega = vel_lin*kappa;
	
    velocity.left = vel_lin-omega*Controller::WHEEL_BASE/2;
    velocity.right = 2*vel_lin-velocity.left;

    //encoders.right = encoder_new.right;
    //encoders.left = encoder_new.left;
    encoder_t ret_vel;

    ret_vel.left  = max(velocity.left/vel_max*255/3.5,0);
    ret_vel.right = max(velocity.right/vel_max*255/3.5,0);
    return ret_vel;
}

/************************************************************************/
/* Starts line follow mode. Will try to drive at a set speed            */
/************************************************************************/
void Controller::startLineFollow(int speed){
	target_speed = speed;
	state = LINE_FOLLOW;
}

/************************************************************************/
/* Linefollow. Will be called repeatedly when in linefollow state       */
/************************************************************************/

encoder_t Controller::lineFollow(line_sensors_t sensor,int distance){
	static int16_t v_left = 0;
	static int16_t v_right = 0;
	int i;
	int lost_line = true;
	int16_t s[3];
	s[0] = sensor.left;
	s[1] = sensor.middle;
	s[2] = sensor.right;
	for (i = 0; i < 3; i++) {
	if (s[i] > LINETHRESHOLD)
	  lost_line = false;
	}

	if (lost_line) {
		if (e < 0)
		  e = -100;
		else
		  e = 100;
	}
	else {
		if((s[0] + s[1] + s[2]) != 0)
			e = (- 100L * s[0] + 100L * s[2]) / (s[0] + s[1] + s[2]);
	}
  ed = e-last_e;
	v_left  = target_speed + (int16_t)((kp * e)  + kd*(ed) + kdd*(ed-last_ed));
	v_right = target_speed - (int16_t)((kp * e)  + kd*(ed) + kdd*(ed-last_ed));

  

	//int speedDifference = e / 4;// + 6 * (e - last_e);
	//Serial.println("e: " + String(e) + " last e: " + String(last_e));
	  //v_left = target_speed + speedDifference;
	  //v_right = target_speed - speedDifference;

	  if (v_left < 0)
		  v_left = 0;
	  if (v_right < 0)
		  v_right = 0;
	  if (v_left > MAX_SPEED)
		  v_left = MAX_SPEED;
	  if (v_right > MAX_SPEED)
		  v_right = MAX_SPEED;
      
  v_left = int(v_left - ((v_left - last_vel_left) * ke));
  v_right = int(v_right - ((v_right - last_vel_right) * ke));
	last_e = e;
  last_ed = ed;
  last_vel_right = v_right;
  last_vel_left = v_left;
	//Serial.println("left: " + String(s[0]) + " middle: " + String(s[1]) + " right: " + String(s[2]) + " e: " + String(e) + "v left:" + String(v_left) + "v right: " + String(v_right));


	encoder_t ret_vel = {v_right, v_left};
	return ret_vel;
// now set your motor speeds to v_left and v_right
}
#if 0
encoder_t Controller::lineFollow(line_sensors_t sensor,int distance){
	static int16_t v_left = 0;
	static int16_t v_right = 0;
	
	// calculate speed from sensor readings
	if((sensor.left < LINETHRESHOLD) && (sensor.middle > LINETHRESHOLD) && (sensor.right < LINETHRESHOLD)){ //middle line
		v_right = target_speed;
		v_left = target_speed;
	  }
	  else if((sensor.left < LINETHRESHOLD) && (sensor.middle > LINETHRESHOLD) && (sensor.right > LINETHRESHOLD)){ //mid right
		v_right = target_speed;
		v_left = target_speed - 20;
	  }
	  else if((sensor.left < LINETHRESHOLD) && (sensor.middle < LINETHRESHOLD) && (sensor.right > LINETHRESHOLD)){ //right
		v_right = target_speed;
		v_left = target_speed - 40;
	  }
	  else if((sensor.left > LINETHRESHOLD) && (sensor.middle > LINETHRESHOLD) && (sensor.right < LINETHRESHOLD)){ //mid left
		v_right = target_speed - 20;
		v_left = target_speed;
	  }
	  else if((sensor.left > LINETHRESHOLD) && (sensor.middle < LINETHRESHOLD) && (sensor.right < LINETHRESHOLD)){ //left
		v_right = target_speed - 40;
		v_left = target_speed;
	  }
	  else{ //none of them --> keep old speed
	  }
		if (distance < 40){
      //controller.startLaneChange(true, 35);
    //state=LANE_CHANGE;
    }
	encoder_t ret_vel = {v_left, v_right};
	return ret_vel;
}
#endif
/************************************************************************/
/* Only drives on fused position                                        */
/************************************************************************/
void Controller::driveSetDistance(uint32_t mm){
	
	waypoint.y = position.y + mm*sin(position.theta);
	waypoint.x = position.x + mm*cos(position.theta);
	state = WAYPOINT_FOLLOW;
}

Controller controller = Controller();

  
