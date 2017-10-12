from Controller_lib import Pose
from Controller_lib import Update_Pose
import math
import time
import lcm
import threading
from exlcm import sensor_vals_t


pos = Pose(0,0,0)
wheel_base = 18
wheel_dia = 6.5
EResol = 192


#Controller parameters
k1 = 0;
k2 = 2;
Vel_max = 10;
Stop_dist = 2;
vel_l = 0;
vel_r = 0;

control_param = [k1,k2,Vel_max,Stop_dist]

Enc_left_old = 0;
Enc_right_old = 0;
Enc_left = 0;
Enc_right = 0;
lc = lcm.LCM()
run = True
def handle_Caller():
    while run:
        print 'start'
        lc.handle()

def my_handler(channel, data):
    global Enc_right_old
    global Enc_left_old
    msg = sensor_vals_t.decode(data)

    Enc_left = msg.wheel_encoders.left
    Enc_right = msg.wheel_encoders.right

    dEr = Enc_right - Enc_right_old
    dEl = Enc_left - Enc_left_old

    Update_Pose(pos,dEr,dEl,wheel_dia,EResol,wheel_base)

    Enc_left_old = Enc_left
    Enc_right_old = Enc_right
    
    print("Received message on channel \"%s\"" % channel)
    print("   timestamp   = %s" % str(msg.timestamp))
    print("   Left    = %s" % str(msg.wheel_encoders.left))
    print("   Right   = %s" % str(msg.wheel_encoders.right))
    print("")


subscription = lc.subscribe("SENSOR", my_handler)

HandleThread = threading.Thread(target=handle_Caller,args=())
HandleThread.demon = True
HandleThread.start()

try:

    while True:
        print pos.x
        time.sleep(1)
       
except KeyboardInterrupt:
    pass    

run = False
lc.unsubscribe(subscription)


