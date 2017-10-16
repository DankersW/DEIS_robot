from Controller_lib import Pose
from Controller_lib import Update_Pose
import math
import time
import lcm
import threading
from exlcm import wheel_speeds_t
from exlcm import sensor_vals_t



class MotionController:
    pos = 0
    wheel_base = 18
    wheel_dia = 6.5
    EResol = 192

    #Controller parameters

    k1 = 0
    k2 = 2
    Vel_max = 255
    Stop_dist = 2
    control_param = 0
    Enc_left_old = 0
    Enc_right_old = 0
    Enc_left = 0
    Enc_right = 0
    lc = lcm.LCM()
    ls = lcm.LCM()
    subscription = 0
    ListenThread = 0
    SendThread = 0

    waypoint  = [0,0,0]

    def handle_Receive_Caller(self):
        while True:
            lc.handle()

    def receive_msg(channel, data):

        msg = sensor_vals_t.decode(data)

        self.Enc_left = msg.wheel_encoders.left
        self.Enc_right = msg.wheel_encoders.right

        dEr = Enc_right - Enc_right_old
        dEl = Enc_left - Enc_left_old

        Update_Pose(pos,dEr,dEl,wheel_dia,EResol,wheel_base)

        self.Enc_left_old = Enc_left
        self.Enc_right_old = Enc_right



    def create_send_msg(self):
        vel_l,vel_r = Controller_Output(self.pos,self.waypoint,self.control_param,self.wheel_base)
        msg = wheel_speeds_t()
        msg.wheel.left = vel_l
        msg.wheel.right = vel_r

        return msg


    def run(self):

        while True:
            msg = create_send_msg()
            self.ls.publish("COMMAND",msg.encode())


    def setWaypoint(self,x,y,theta):
        self.waypoint = [x,y,theta]


    def __init__(self, init_x,init_y,init_theta):
        self.pos = Pose(init_x,init_y,init_theta)
        self.subscription = lc.subscribe("SENSOR", receive_msg)
        self.control_param = [k1,k2,Vel_max,Stop_dist]
        self.waypoint = [0,0,0]
        self.ListenThread = threading.Thread(target=handle_Receive_Caller, args=())
        self.SendThread = threading.Thread(target=run, args=())

        self.ListenThread.demon = True
        self.ListenThread.start()
        self.SendThread.demon = True
        self.SendThread.start()






