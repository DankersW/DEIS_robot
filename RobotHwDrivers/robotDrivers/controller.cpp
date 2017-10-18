#include "controller.h"
#include "math.h"

static int Sign(double Val) { return (Val > 0) ? 1 : ((Val < 0) ? -1 : 0); }
static double AngDiff(double A1, double A2){ return (A1-A2) + ((A1-A2) < -M_PI)*(2*M_PI) + ((A1-A2) > M_PI)*(-2*M_PI);} //roll-over

Controller::Controller(pos_t start, encoder_t encoders)
    : position(start)
    , encoders(encoders) {
      velocity = {0};
    
    waypoint = {0};
    waypoint.x += 200;// 10*cos(position.theta) ;
    //waypoint.y += 10*sin(position.theta) ;
}

void Controller::updatePosition(encoder_t encoder_deltas){
    double d_r            = M_PI * Controller::WHEEL_DIAMETER * (double(encoder_deltas.right)/ Controller::ERESOL);
    double d_l            = M_PI * Controller::WHEEL_DIAMETER * (double(encoder_deltas.left)/ Controller::ERESOL);
    double d_c            = (d_r + d_l)/2;
    double theta_change   = (d_r - d_l)/Controller::WHEEL_BASE;
    Serial.print("\tpxd: " + String(cos(position.theta + (theta_change/2))) + "\tdr: " + String(d_r));
    
    position.x     += d_c * cos(position.theta + (theta_change/2));
    position.y     += d_c * sin(position.theta + (theta_change/2));
    position.theta += theta_change;
}

//check for an overflow
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

void Controller::updateLineSensors(line_sensors_t line_sensors_new){
  line_sensors.left   = line_sensors_new.left;
  line_sensors.middle = line_sensors_new.middle;
  line_sensors.right  = line_sensors_new.right;   
}

encoder_t Controller::update(encoder_t encoder_new){
  int inmiddle = 1;
  if(line_sensors.middle > 800) 
    inmiddle = 0;
    float dlines = float(line_sensors.left - line_sensors.right);
    float dthetaline = inmiddle*dlines/1000 * M_PI/2;   
    Serial.print("DThetaLine: " + String(dthetaline));
    
    waypoint.x = position.x + 10*cos(position.theta+dthetaline) ;
    waypoint.y = position.y + 10*sin(position.theta+dthetaline) ;
    
    d_x = waypoint.x - position.x;
    d_y = waypoint.y - position.y;
          
    ego_cosys_ang = atan2(d_y,d_x);
    ego_delta = AngDiff(position.theta,ego_cosys_ang);
    ego_theta = AngDiff(waypoint.theta,ego_cosys_ang);

    r = sqrt(pow(d_x, 2) + pow(d_y, 2));

    if (r < 6){
      //waypoint.x = 100;
      //waypoint.y = 0;
      if (r<3){
        return {0};
      }
    }

    Serial.print("R: " + String(r) + "\tDx: " + String(d_x)+ "\tDy: " + String(d_y));
      
    kappa = -1/r*(Controller::K2*(ego_delta-atan(-Controller::K1*ego_theta))+ (1+Controller::K1/(1+(Controller::K1*pow(ego_theta,2))))*sin(ego_delta));
    vel_lin = vel_max/(1+10*abs(pow(kappa,2)));
    omega = vel_lin*kappa;
    encoder_t deltas = encoder_new - encoders;
    velocity.left = vel_lin-omega*Controller::WHEEL_BASE/2;
    velocity.right = 2*vel_lin-velocity.left;

    this->updatePosition(deltas);

    encoders.right = encoder_new.right;
    encoders.left = encoder_new.left;
    encoder_t ret_vel;
    ret_vel.left  = max(velocity.left/vel_max*255/4,0);
    ret_vel.right = max(velocity.right/vel_max*255/4,0);
    return ret_vel;
}
