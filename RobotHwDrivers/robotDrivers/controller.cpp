#include "controller.h"
#include "math.h"

static inline int Sign(double Val) { return (Val > 0) ? 1 : ((Val < 0) ? -1 : 0); }
static double AngDiff(double A1, double A2){ return (A1-A2) + ((A1-A2) < -M_PI)*(2*M_PI) + ((A1-A2) > M_PI)*(-2*M_PI);} //roll-over

Controller::Controller(pos_t start, encoder_t encoders)
    : position(start)
    , encoders(encoders)
	, state(IDLE)
	, target_speed(0){

    velocity = {0};
    waypoint = {0};
}

void Controller::setWaypoint(pos_t i){
  //Serial.println("Distance \t: " + String(i.distance));
  waypoint.x += i.x;
  waypoint.y += i.y;
    //waypoint.x += i.distance;// 10*cos(position.theta) ;
    //waypoint.y += 10*sin(position.theta) ;
}

void Controller::updateGPS(pos_t gps_pos){
	position = gps_pos;
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
}

/**
 *  Checks for overflow
 */
void Controller::updateEncoders(encoder_t encoder_deltas){
  static double pos_r = 0;
  static double pos_l = 0;

  pos_r += encoder_deltas.right;
  pos_l += encoder_deltas.left;
            
  if(abs(encoder_deltas.right)>Controller::ENCODER_MAX/2){
    pos_r += Sign(encoders.right)*2*Controller::ENCODER_MAX;
  }          
  if(abs(encoder_deltas.left)>Controller::ENCODER_MAX/2){
    pos_l += Sign(encoders.left)*2*Controller::ENCODER_MAX;
  }

  encoders.right = pos_r;
  encoders.left = pos_l;  
  return;
}

encoder_t Controller::update(encoder_t encoder_new, line_sensors_t line_sensors, int distance){
	encoder_t deltas = encoder_new - encoders;
    updatePosition(deltas);

  if(distance < 20){ //object detected less then 20 cm in front 
    return{0};
  }
	
	// TODO: Implementing a proper state machine using inheritance would be more neat
	switch(state){
	case IDLE:
		break;
	case WAYPOINT_FOLLOW:
		return waypointFollow(encoder_new, line_sensors);
	case LINE_FOLLOW:
		return lineFollow(line_sensors);
	}
	return {0};
}

encoder_t Controller::waypointFollow(encoder_t encoder_new, line_sensors_t line_sensors){
    d_x = waypoint.x - position.x;
    d_y = waypoint.y - position.y;
          
    ego_cosys_ang = atan2(d_y,d_x);
    ego_delta = AngDiff(position.theta,ego_cosys_ang);
    ego_theta = AngDiff(waypoint.theta,ego_cosys_ang);

    r = sqrt(pow(d_x, 2) + pow(d_y, 2));

    if (r < 15){
    	return {0};
    }

    //Serial.print("R: " + String(r) + "\tDx: " + String(d_x)+ "\tDy: " + String(d_y));
      
    kappa = -1/r*(Controller::K2*(ego_delta-atan(-Controller::K1*ego_theta))+ (1+Controller::K1/(1+(Controller::K1*pow(ego_theta,2))))*sin(ego_delta));
    vel_lin = vel_max/(1+10*abs(pow(kappa,2)));
    omega = vel_lin*kappa;
	
    velocity.left = vel_lin-omega*Controller::WHEEL_BASE/2;
    velocity.right = 2*vel_lin-velocity.left;

    encoders.right = encoder_new.right;
    encoders.left = encoder_new.left;
    encoder_t ret_vel;

    ret_vel.left  = max(velocity.left/vel_max*255/2,0);
    ret_vel.right = max(velocity.right/vel_max*255/2,0);
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
encoder_t Controller::lineFollow(line_sensors_t sensor){
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
		
	encoder_t ret_vel = {v_left, v_right};
	return ret_vel;
}

/************************************************************************/
/* Only drives on fused position                                        */
/************************************************************************/
void Controller::driveSetDistance(uint32_t mm){
	
	waypoint.y = position.y + mm*sin(position.theta);
	waypoint.x = position.x + mm*cos(position.theta);
	state = WAYPOINT_FOLLOW;
}

Controller controller = Controller();
