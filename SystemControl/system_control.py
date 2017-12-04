"""
System control module
"""
#import sys
import time
from i2c_handler import I2CHandler
from gps_client import GpsClient
from heartbeat import Heartbeat
#from image_recognizer import ImageRecognizer
from robot_follower import RobotFollower

import rospy

SVM_PATH = "~/DEIS/svm"

class SystemControl(object):
    """
    System Control class.
    """
    def position_callback(self, pos_x, pos_y, theta):
        """
        Callback for gps position updates.
        """
        
        #print "pos x: ", pos_x, " pos y: ", pos_y, " theta: ", theta
        
        if pos_x is None or pos_y is None or theta is None:
            return
            
        self.my_x = pos_x
        self.my_y = pos_y
        self.my_theta = theta
        self.update_speed()
        msg = "GPS,"+str(int(pos_x)) + ","+str(int(pos_y)) + "," + str(theta)
        #self.heartbeat.update_gps(pos_y, pos_x)
        #self.serial.send_message(msg)
        #print msg
        
    def position_callback_other(self, pos_x, pos_y, theta):
        """
        Callback for gps position other updates.
        """
        
        print "pos x_other: ", pos_x, " pos y_other: ", pos_y, " theta_other: ", theta
        
        if pos_x is None or pos_y is None or theta is None:
            return
            
        self.other_x = pos_x
        self.other_y = pos_y
        self.other_theta = theta
        self.update_speed()
        msg = "GPS_other,"+str(int(pos_x)) + ","+str(int(pos_y)) + "," + str(theta)
        #self.heartbeat.update_gps(pos_y, pos_x)
        #self.serial.send_message(msg)
        #print msg

    def heartbeat_callback(self, data):
        #result = [x.strip() for x in data.split(',')]
        print 'HB: %s' % data.data


    def update_speed(self):
        speed = self.robot_follower.computeVelocity_P([self.my_x,self.my_y], [self.other_x, self.other_y],)
        print "speed: ", speed
        if not speed == self.speed:
            print "Set speed to: ", speed
            self.speed = speed
            self.i2c.startLineFollow(int(speed))

    def get_ultrasound(self):
        #threat loop
        # if time past
            #  self.i2c.get_ultrasound()
        #else
            #sleep
        
    def __init__(self):
        """
        Constructor
        """
        global SVM_PATH
        self.my_x = 0
        self.my_y = 0
        self.my_theta = 0
        self.other_x = 0
        self.other_y = 0
        self.other_theta = 0
        self.speed = 0
        rospy.init_node('g3', anonymous=True)
        self.i2c = I2CHandler()

        #self.serial = SerialHandler('/dev/ttyUSB0', 9600)
        #self.heartbeat = Heartbeat(self.heartbeat_callback)
        #self.gps = GpsClient(self.position_callback, color='blue')
        #self.gps_other = GpsClient(self.position_callback_other, color='green')
        #self.image_recognizer = ImageRecognizer(SVM_PATH)
        #self.robot_follower = RobotFollower(300,k_p = 0.5)
        self.get_ultrasound()

if __name__ == '__main__':
    c = SystemControl()
    pingTimeUltra = int(round(time.time() * 1000)) #current time in milliseconds
    while True:
        c.get_ultrasound()
        time.sleep(0.01)
    rospy.spin()
