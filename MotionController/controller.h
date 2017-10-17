#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

typedef struct pos_struct{
    double x;
    double y;
    double theta;
} pos_t;

typedef struct encoder {
    double right;
    double left;
} encoder_t;
 
class Controller {

public:
    static const double wheel_base = 18; //cm
    static const double dia        = 3 ; //cm
    static const double EResol     = 8084.21;
    static const double k1 = 0;
    static const double k2 = 2;
    double Vel_lin = 0;
    double Vel_max = 10;
    static const int CHANNEL = 1;
    //Controller();
    Controller(pos_t start = {0}, encoder_t encoders);
    void updatePosition(encoder_t encoder_deltas);

private:
    pos_t position;
    encoder_t encoders;
};

#endif