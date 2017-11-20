"""
System control module
"""
#import sys
from serial_handler import SerialHandler
from gps_client import GpsClient
from heartbeat import Heartbeat
import SensorFusion
from image_recognizer import ImageRecognizer
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
        if pos_x is None:
            print "No position gps"
            return

        msg = "GPS,"+str(int(pos_x)) + ","+str(int(pos_y)) + "," + str(theta)
        self.serial.send_message(msg)

        args = [pos_x,pos_y,theta]
        odo = [self.heartbeat.x,self.heartbeat.y,self.heartbeat.theta]
        res = SensorFusion(odo,args).fusion()
        self.heartbeat.update_gps(res[1], res[0])
        print msg

    def heartbeat_callback(self, data):
        pass
        #result = [x.strip() for x in data.split(',')]
        #print 'HB: %s' % data.data

    def odometry(self, x, y, theta):
        args = [x,y,theta]
        res = SensorFusion(args).fusion()
        self.heartbeat.update_gps(res[1], res[0])

    def __init__(self):
        """
        Constructor
        """
        global SVM_PATH
        rospy.init_node('g3', anonymous=True)
        self.serial = SerialHandler('/dev/ttyUSB0', 9600, self)
        self.heartbeat = Heartbeat(self.heartbeat_callback)
        self.gps = GpsClient(self.position_callback)
        #self.image_recognizer = ImageRecognizer(SVM_PATH)
        #self.robot_follower = RobotFollower(50)

if __name__ == '__main__':
    _ = SystemControl()
    rospy.spin()
