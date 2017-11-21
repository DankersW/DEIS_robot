"""
System control module
"""
#import sys
from serial_handler import SerialHandler
from gps_client import GpsClient
from heartbeat import Heartbeat
from sensor_fusion import SensorFusion
from image_recognizer import ImageRecognizer
from robot_follower import RobotFollower

import rospy

SVM_PATH = "~/DEIS/svm"
pos_x = 0
pos_y = 0
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

        msg = "gps,"+str(int(pos_x)) + ","+str(int(pos_y)) + "," + str(theta) + "\n"
        #self.serial.send_message(msg)

        args = [pos_x,pos_y,theta]
        odo = [float(self.heartbeat.x),float(self.heartbeat.y),float(self.heartbeat.theta)]
        res = SensorFusion(odo,args).fusion()
        self.heartbeat.update_gps(res[0], res[1], res[2])
        print msg

    def heartbeat_callback(self, data):
        pass
        """
        global pos_x
        global pos_y
        result = [x.strip() for x in str(data).split(',')]
        if len(result) >= 3:
            velocity_value = self.robot_follower.computeVelocity_P([float(pos_x), float(pos_y)], [float(result[1]), float(result[2])])
            v = min(int(velocity_value),100)
            msg = "linefollow,"+str(v)
            #print msg
            #self.serial.send_message(msg)
        #print 'HB: %s' % data.data
        """
    def odometry(self, x, y, theta):
        global pos_x
        global pos_y
        pos_x = x
        pos_y = y
        print "odometry update x: " , x, " y:", y, " theta:", theta
        args = [x,y,theta]
        res = SensorFusion(args).fusion()
        self.heartbeat.update_gps(''.join(e for e in res[0] if e.isalnum()), ''.join(e for e in res[1] if e.isalnum()), ''.join(e for e in res[2] if e.isalnum()))
    
    def ultrasonic(self,distance):
        pass
        
    def debug(self,msg):
        pass

    def __init__(self):
        """
        Constructor
        """
        global SVM_PATH
        rospy.init_node('g3', anonymous=True)
        self.heartbeat = Heartbeat(self.heartbeat_callback)
        self.serial = SerialHandler('/dev/ttyUSB1', 115200, self)
        self.gps = GpsClient(self.position_callback)
        #self.image_recognizer = ImageRecognizer(SVM_PATH)
        self.robot_follower = RobotFollower(50,5)

if __name__ == '__main__':
    _ = SystemControl()
    rospy.spin()
