


int Sign(double Val) { return (Val > 0) ? 1 : ((Val < 0) ? -1 : 0); }

double AngDiff(double A1, double A2){ return (A1-A2) + ((A1-A2) < -M_PI)*(2*M_PI) + ((A1-A2) > M_PI)*(-2*M_PI);} //roll-over

const double wheel_base = 18; //cm
const double dia        = 3 ; //cm
const double EResol     = 8084.21;
const double k1 = 0;
const double k2 = 2;
double Vel_lin = 0;
double Vel_max = 10;
static const int CHANNEL = 1;

struct pos_struct{
   double x;
   double y;
   double theta;
}pos_struct;

struct encoder_struct{
	double Init_R;
	double Init_L;
	double EncoderR;
	double EncoderL;
}encoder_struct;






double ReadingR_old = pWhite_Board_RX->Position_M1;
double ReadingL_old = pWhite_Board_RX->Position_M0;
double EncoderR_old = Encoder_Values.EncoderR;
double EncoderL_old = Encoder_Values.EncoderL;
double ego_cosys_ang = 0;
double ego_delta = 0;
double ego_theta = 0;
double kappa = 0;
double omega = 0;
double Vel_l = 0;
double Vel_r = 0;
double dX = 0;
double dY = 0;
double r = 0;
   

 
Init_Odometry(&Current_Pos,target);
int velL;
int velR;





void Update_Pos(struct pos_struct *Pos, double d_EncoderR, double d_EncoderL){

  
    double Dr            = M_PI * dia * (d_EncoderR/ EResol);
    double Dl            = M_PI * dia * (d_EncoderL/ EResol);
    double Dc            = (Dr + Dl)/2;
    double theta_change  = (Dr - Dl)/wheel_base;

    Pos->x     = Pos->x     + Dc * cos(Pos->theta + (theta_change/2));
    Pos->y     = Pos->y     + Dc * sin(Pos->theta + (theta_change /2));
    Pos->theta = Pos->theta + theta_change;

}


void Init_Odometry(struct pos_struct *Pos, struct node *Start_point){
	
	 Pos->x = Start_point->x;
	 Pos->y = Start_point->y;
	 Pos->theta = Start_point->theta;	
}


void update_Encoder_Values(struct encoder_struct *encoder_Values, double dReadingR, double dReadingL){
	   	
		static double Pos_R = 0;
		static double Pos_L = 0;

		Pos_R += dReadingR;
	    Pos_L += dReadingL;
			    
		if(abs(dReadingR)>_MAX/2){
				Pos_R += Sign(encoder_Values->EncoderR)*2*_MAX;
		}
				
		if(abs(dReadingL)>_MAX/2){
				Pos_L += Sign(encoder_Values->EncoderL)*2*_MAX;
		}
		
		encoder_Values->EncoderR = Pos_R;
		encoder_Values->EncoderL = Pos_L;	
		return;
}





dX= Current_pos.x-100


ego_cosys_ang = atan2(dY,dX);
ego_delta = AngDiff(Current_Pos.theta,ego_cosys_ang);
ego_theta = AngDiff(target->theta,ego_cosys_ang);


kappa = -1/r*(k2*(ego_delta-atan(-k1*ego_theta))+(1+k1/(1+(k1*pow(ego_theta,2))))*sin(ego_delta));
Vel_lin = Vel_max/(1+10*abs(pow(kappa,2)));
omega = Vel_lin*kappa;

Vel_l = Vel_lin-omega*wheel_base/2;
Vel_r = 2*Vel_lin-Vel_l; 


memcpy(buffer,pWhite_Board_TX,32);
result = wiringPiSPIDataRW(CHANNEL, buffer, 32);

update_Encoder_Values(&Encoder_Values,pWhite_Board_RX->Position_M1-ReadingR_old,pWhite_Board_RX->Position_M0-ReadingL_old);
Update_Pos(&Current_Pos,Encoder_Values.EncoderR-EncoderR_old,Encoder_Values.EncoderL-EncoderL_old);

ReadingR_old = pWhite_Board_RX->Position_M1;
ReadingL_old = pWhite_Board_RX->Position_M0;
EncoderR_old = Encoder_Values.EncoderR;
EncoderL_old = Encoder_Values.EncoderL;