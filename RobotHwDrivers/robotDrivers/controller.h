#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#include <Arduino.h>
//#define abs(x) (((x)<0) ? -(x) : (x))

typedef struct pos_struct{
    double x;
    double y;
    double theta;
} pos_t;

typedef struct encoder {
    double right;
    double left;

    struct encoder operator+(struct encoder other){
        other.right += right;
        other.left  += left;
        return other;
    }

    struct encoder operator-(struct encoder other){
        other.right -= right;
        other.left  -= left;
        return other;
    }
} encoder_t;
 
class Controller {

public:
    /**
     * Static constants
     */
    static constexpr double WHEEL_BASE      = 18; //cm
    static constexpr double WHEEL_DIAMETER  = 6.5 ; //cm
    static constexpr double ERESOL          = 192;
    static constexpr double K1              = 0;
    static constexpr double K2              = 2;
    static const int        ENCODER_MAX     = 0X7ff;
    
    double vel_lin = 0;
    double vel_max = 10;
    static const int CHANNEL = 1;
    pos_t waypoint;
    
    
    /**
     *  
     */
    Controller(pos_t start = {0}, encoder_t encoders = {0});
    void updatePosition(encoder_t encoder_deltas);
    void updateEncoders(encoder_t encoder_deltas);
    encoder_t update(encoder_t encoder_new);

private:
    pos_t position;
    encoder_t encoders;
    encoder_t reading;
    encoder_t velocity;

    double ego_cosys_ang = 0;
    double ego_delta = 0;
    double ego_theta = 0;
    double kappa = 0;
    double omega = 0;
    double d_x = 0;
    double d_y = 0;
    double r = 0;
};
#endif

