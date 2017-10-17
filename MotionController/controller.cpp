#include "controller.h"
/*
Controller::Controller()
    : Controller::Controller({0}){

}
*/

Controller::Controller(pos_t start, encoder_t encoders)
    : position(start)
    , encoders(encoders) {
    
}

void Controller::updatePosition(encoder_t encoder_deltas){
    double d_r            = M_PI * dia * (encoder_deltas.right/ Controller::EResol);
    double d_l            = M_PI * dia * (encoder_deltas.left/ EResol);
    double d_c            = (Dr + Dl)/2;
    double theta_change  = (Dr - Dl)/wheel_base;

    position.x     += Dc * cos(Pos->theta + (theta_change/2));
    position.y     += Dc * sin(Pos->theta + (theta_change /2));
    position.theta += theta_change;

}

int main(int argv, const char *args[]){

}