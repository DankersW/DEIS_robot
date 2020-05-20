from Controller_lib import Pose
from Controller_lib import Update_Pose
from Controller_lib import Controller_Output

import math
import time
import lcm
import threading
import sys


sys.path.append('../LCM')
from exlcm import wheel_speeds_t
from exlcm import sensor_vals_t

pos = 0
wheel_base = 18
wheel_dia = 6.5
EResol = 192

#Controller parameters

k1 = 0
k2 = 2
Vel_max = 1
Stop_dist = 20
subscription = 0
#ListenThread = 0
#SendThread = 0

#waypoint  = [0,0,0]

class MotionController(object):
    """
        pos = 0
        wheel_base = 18
        wheel_dia = 6.5
        EResol = 192

        #Controller parameters

        k1 = 0
        k2 = 2
        Vel_max = 10
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
    """
    def handle_Receive_Caller(self):
        while True:
            self.lc.handle()

    def receive_msg(self, channel, data):
        global wheel_dia
        global EResol
        global wheel_base
        msg = sensor_vals_t.decode(data)

        self.Enc_left = msg.wheel_encoders.left
        self.Enc_right = msg.wheel_encoders.right

        self.dEr = self.Enc_right - self.Enc_right_old
        self.dEl = self.Enc_left - self.Enc_left_old
        print "enc_l, enc_r",self.Enc_left,"",self.Enc_right
        Update_Pose(self.pos, self.dEr, self.dEl, wheel_dia, EResol, wheel_base)

        self.Enc_left_old = self.Enc_left
        self.Enc_right_old = self.Enc_right



    def create_send_msg(self):
        """
        """
        global wheel_base
        vel_l,vel_r = Controller_Output(self.pos,self.waypoint,self.control_param,wheel_base)
        msg = wheel_speeds_t()
        print "vel_l: ", vel_l, "vel_r: ", vel_r 
        msg.wheel.left = vel_l
        msg.wheel.right = vel_r

        return msg


    def run(self):

        while True:
            msg = self.create_send_msg()
            self.ls.publish("COMMAND",msg.encode())


    def setWaypoint(self,x,y,theta):
        self.waypoint = [x,y,theta]


    def __init__(self, init_x,init_y,init_theta):

        self.control_param = 0
        self.Enc_left_old = 0
        self.Enc_right_old = 0
        self.Enc_left = 0
        self.Enc_right = 0
        self.lc = lcm.LCM()
        self.ls = lcm.LCM()
        
        self.pos = Pose(init_x,init_y,init_theta)
        self.subscription = self.lc.subscribe("SENSOR", self.receive_msg)
        self.control_param = [k1,k2,Vel_max,Stop_dist]
        self.waypoint = [0,0,0]
        self.ListenThread = threading.Thread(target=self.handle_Receive_Caller, args=())
        self.SendThread = threading.Thread(target=self.run, args=())

        self.ListenThread.demon = True
        self.ListenThread.start()
        self.SendThread.demon = True
        self.SendThread.start()

def main():
    motion_controller = MotionController(0,0,0)
    motion_controller.setWaypoint(30,0,0)
    print "motion controller instansiated"


if  __name__ == '__main__':
    main()
