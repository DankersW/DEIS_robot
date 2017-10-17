#include "controller.h"
#include "math.h"

#define abs(x) (((x)<0) ? -(x) : (x))

/*
Controller::Controller()
    : Controller::Controller({0}){

}
*/

static int Sign(double Val) { return (Val > 0) ? 1 : ((Val < 0) ? -1 : 0); }

static double AngDiff(double A1, double A2){ return (A1-A2) + ((A1-A2) < -M_PI)*(2*M_PI) + ((A1-A2) > M_PI)*(-2*M_PI);} //roll-over


Controller::Controller(pos_t start, encoder_t encoders, encoder_t velocity)
    : position(start)
    , encoders(encoders)
    , velocity(velocity){
    
}

void Controller::updatePosition(encoder_t encoder_deltas){
    double d_r            = M_PI * Controller::WHEEL_DIAMETER * 
                                     (encoder_deltas.right/ Controller::ERESOL);
    double d_l            = M_PI * Controller::WHEEL_DIAMETER * 
                                      (encoder_deltas.left/ Controller::ERESOL);
    double d_c            = (d_r + d_l)/2;
    double theta_change  = (d_r - d_l)/Controller::WHEEL_BASE;

    position.x     += d_c * cos(position.theta + (theta_change/2));
    position.y     += d_c * sin(position.theta + (theta_change /2));
    position.theta += theta_change;

}

/**
 * Checks for overflow 
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

void Controller::update(encoder_t encoder_new){
    //dX= position.x-100


    ego_cosys_ang = atan2(d_y,d_x);
    ego_delta = AngDiff(position.theta,ego_cosys_ang);
    ego_theta = AngDiff(position.theta,ego_cosys_ang);


    kappa = -1/r*(Controller::K2*(ego_delta-atan(-Controller::K1*ego_theta))+
    (1+Controller::K1/(1+(Controller::K1*pow(ego_theta,2))))*sin(ego_delta));
    vel_lin = vel_max/(1+10*abs(pow(kappa,2)));
    omega = vel_lin*kappa;

    velocity.left = vel_lin-omega*Controller::WHEEL_BASE/2;
    velocity.right = 2*vel_lin-velocity.left;

    //this->updateEncoders(encoder_new)
    this->updatePosition(encoder_new);

    //reading.right = pWhite_Board_RX->Position_M1;
    //reading.left = pWhite_Board_RX->Position_M0;
    encoders.right = encoder_new.right;
    encoders.left = encoder_new.left;
}

