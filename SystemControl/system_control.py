"""
System control module
"""
#import sys
import time
from i2c_handler import I2CHandler
from gps_client import GpsClient
from heartbeat import Heartbeat
from ImageRecognizer import ImageRecognizer
from robot_follower import RobotFollower
from VideoStream import VideoStream
import rospy
import cv2

SVM_PATH = "/home/pi/DEIS/SVM/bof.pkl"

detectionFlag = False
detectionFlagPrev = False
detectedObject = -1


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
        self.heartbeat.update_gps(pos_y, pos_x, 0)
        #self.serial.send_message(msg)
        print msg
        
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

    def representsInt(self,s):
        try:
            int(s)
            return True
        except ValueError:
            return False

    def heartbeat_callback(self, data):
        print 'HeartBeat: %s' % data.data
        h = data.data.strip().split(',')
        if (len(h) == 3 and self.representsInt(h[0]) and self.representsInt(h[1]) and self.representsInt(h[2])):
            self.posData[int(h[0]) - 1] = [h[1], h[2]]

    def platoon_pos_callback(self, data):
        print 'platoon_pos: %s' % data.data
        h = data.data.strip().split(',')
        if (len(h) == 2 and self.representsInt(h[0]) and self.representsInt(h[1])):
            self.followingData[int(h[0]) - 1] = h[1]

    def fan_out_callback(self, data):
        # if message = "Fanout" --> save in fanoutFlag
        h = data.data.strip()
        if (h == "fanOut"):
            self.fanOutFlag = True
            print "Fan out message received!!!"

    def lane_change_callback(self, data):
        # to go to lane filter out own pos --> save in LaneToGoToData
        corruptData = True
        h = data.data.strip().split(',')
        if (len(h) == 5):
            corruptData = False
            for i in range(len(h)):
                if not (self.representsInt(h[i])):
                    corruptData = True

        if not corruptData:
            self.laneToGoTo =  h[2]
            print "lane to go to: " + str(self.laneToGoTo)


    def update_speed(self):
        #speed = self.robot_follower.computeVelocity_P([self.my_x,self.my_y], [self.other_x, self.other_y],)
        speed = self.robot_follower.computeVelocity_P([self.my_x,self.my_y], [0, 0],)

        print "speed: ", speed
        if not speed == self.speed:
            print "Set speed to: ", speed
            self.speed = speed
            self.i2c.startLineFollow(int(speed))

    def get_ultrasound(self):
        global detectionFlag
        global detectionFlagPrev
        global detectedObject

        if(self.i2c.get_ultrasound()):
            counter = 0
            res = 0
            while counter < 5:
                image = self.vs.read()
                if image is not None:
                    res = self.image_recognizer.recognizeImage(image)

                counter += 1
            print ("detection result: " + str(res))

            if res == 1: #pedestrian detected
                detectionFlag = True
                self.i2c.set_idleState()
                detectedObject = res
            elif res == 2: #sign detected
                detectionFlag = True
                self.i2c.set_scoop(1)
                detectedObject = res
            elif res == 3: # tree detected
                #detectionFlag = True
                self.i2c.set_laneChange(1)
            elif res == 4: # wall detected
                #detectionFlag = False
                pass
            else: #anything else detected
                #detectionFlag = False
                pass
        else:
            detectionFlag = False
            if (detectionFlag == False and detectionFlagPrev == True):
                if (detectedObject == 1): #pedestrain has left the floor
                    print ("drive again")
                    self.i2c.startLineFollow(90)
                elif (detectedObject == 2): #sign has been plowed away
                    self.i2c.set_scoop(2)
                detectionFlag = False

        detectionFlagPrev = detectionFlag


        
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
        self.vs = VideoStream().start()

        self.posData = [[-1,-1],[-1,-1],[-1,-1],[-1,-1],[-1,-1]] #-1 represents no data --> from the start no data has received
        self.followingData = [-1,-1,-1,-1,-1] #-1 represents leader --> from start everyone is the leader
        self.laneToGoTo = -1
        self.fanOutFlag = False


        self.robot_follower = RobotFollower(300,k_p = 0.5)
        #self.serial = SerialHandler('/dev/ttyUSB0', 9600)
        self.heartbeat = Heartbeat(self.heartbeat_callback, self.platoon_pos_callback, self.fan_out_callback, self.lane_change_callback)
        self.gps = GpsClient(self.position_callback)
        #self.gps_other = GpsClient(self.position_callback_other, color='green')
        self.image_recognizer = ImageRecognizer(SVM_PATH)
        self.get_ultrasound()

if __name__ == '__main__':
    c = SystemControl()
    try:
        while True:
            c.get_ultrasound()
            time.sleep(0.1)
        rospy.spin()
    except KeyboardInterrupt:
        print ("Stopping system control!")
        c.vs.stop()
        c.gps.stop()
        
