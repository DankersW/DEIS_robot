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
from TurnCheck import TurnCheck
import rospy
import cv2
import operator


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
        if pos_x is None or pos_y is None or theta is None:
            return
            
        self.my_x = pos_x
        self.my_y = pos_y
        self.my_theta = theta
        #self.update_speed()
        msg = "GPS,"+str(int(pos_x)) + ","+str(int(pos_y)) + "," + str(theta)
        self.heartbeat.update_gps(pos_x, pos_y, 0)
        #print msg
        
    def position_callback_other(self, pos_x, pos_y, theta):
        """
        Callback for gps position other updates.
        """
        if pos_x is None or pos_y is None or theta is None:
            return
            
        self.other_x = pos_x
        self.other_y = pos_y
        self.other_theta = theta
        #self.update_speed()
        msg = "GPS_other,"+str(int(pos_x)) + ","+str(int(pos_y)) + "," + str(theta)
        #self.heartbeat.update_gps(pos_y, pos_x)
        #self.serial.send_message(msg)
        #print msg

    def go_to_lane(self, lane_array, motion):
        platoon_list = []
        following_list = {1: 0, 2: 0, 3: 0, 4: 0, 5: 0}
        movetolane = {}
        stringlist = []
        for i in range(len(lane_array)):  # Obtain a dictionary with robot_id : following_robot_id
            following_list[i + 1] = lane_array[i]
        for i in range(len(lane_array)):  # Check who is the leader
            if lane_array[i] == -1:
                platoon_list.append(i + 1)
        for i in range(5):
            for j in range(5):
                if following_list[j + 1] == platoon_list[i]:
                    platoon_list.append(j + 1)
        if motion == False:
            for i in range(len(platoon_list)):
                movetolane[i + 1] = platoon_list[i]
            sorted_movetolane = sorted(movetolane.items(), key=operator.itemgetter(1))
            for i in range(len(sorted_movetolane)):
                stringlist.append(sorted_movetolane[i][0])
        else:
            for i in range(len(platoon_list)):
                movetolane[i + 1] = platoon_list[i]
                sorted_movetolane = sorted(movetolane.items(), key=operator.itemgetter(1))
            for i in range(len(sorted_movetolane)):
                stringlist.append(sorted_movetolane[i][0])

        for i in range(len(stringlist)):
            if stringlist[i] == 5:
                stringlist[i] = 1

        a = ",".join(str(x) for x in stringlist)
        return str(a)

    def representsInt(self,s):
        try:
            int(s)
            return True
        except ValueError:
            return False

    def heartbeat_callback(self, data):
        #print 'HeartBeat: %s' % data.data
        h = data.data.strip().split(',')
        if (len(h) == 3 and self.representsInt(h[0]) and self.representsInt(h[1]) and self.representsInt(h[2])):
            self.posData[int(h[0]) - 1] = [int(h[1]), int(h[2])]
            #self.update_speed()

    def platoon_pos_callback(self, data):
        print 'platoon_pos: %s' % data.data
        h = data.data.strip().split(',')
        if (len(h) == 2 and self.representsInt(h[0]) and self.representsInt(h[1])):
            self.followingData[int(h[0]) - 1] = int(h[1])

    def fan_out_callback(self, data):
        # if message = "Fanout" --> save in fanoutFlag
        h = data.data.strip()
        if (h == "fanOut"):
            self.fanOutFlag = True
            print "Fan out message received!!!"
            if(self.followingData[2] == -1):
                self.turnC.compute_direction(self.my_x, self.my_y, self.my_theta)
                lanes = self.go_to_lane(self.followingData, self.turnC.cw)
                print "lanes position: " + str(lanes)
                self.heartbeat.update_lane_change_pos(lanes)
            else:
                print "Not the leader!"

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
            self.laneToGoTo =  int(h[2])
            print "Lane to go to: " + str(self.laneToGoTo)
            for i in range(len(h)):
                self.fanOutData[i] = int(h[i])


    def update_speed(self):
        speed = self.robot_follower.computeVelocity_P([self.my_x,self.my_y], [self.other_x, self.other_y],)
        #print ("pos Group 4 : \t" + str(self.posData[3]))
        if not (self.posData[4][0] == -1 or self.posData[4][1] == -1):
            speed = self.robot_follower.computeVelocity_P([self.my_x,self.my_y], self.posData[4],)
            #print "speed: ", speed

            if not speed == self.speed:
                # if(speed <= 5):
                #     self.i2c.set_idleState()
                #     self.speed = speed
                # else:
                #print "STATE: " + str(self.i2c.get_state())
                if (self.i2c.get_state() == 1):
                    self.speed = speed
                    # only if you linefollow == False if(i2c.getState == Lane
                    self.i2c.startLineFollow(int(speed))
                    #print "Set speed to: ", speed

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
            print ("Object in front is: " + str(res))

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
                c.turnC.compute_direction(self.my_x, self.my_y, self.my_theta)
                #print "\n\nX: " + str(self.my_x) + "\t Y: " + str(self.my_y) + "\t LaneDirection: " + str(c.turnC.get_lane(self.my_x, self.my_y)) + "CW: "+str(c.turnC.cw)+ "\t lookup: " + str(c.turnC.DirectionLookUpTable[self.my_x, self.my_y]) + "\n\n"
                self.i2c.set_laneChange(c.turnC.get_lane(self.my_x, self.my_y)[1])
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
                    #print ("drive again")
                    self.i2c.startLineFollow(60)
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
        rospy.init_node('g3')
        self.i2c = I2CHandler()
        self.vs = VideoStream().start()

        self.posData = [[-1,-1],[-1,-1],[-1,-1],[-1,-1],[-1,-1]] #-1 represents no data --> from the start no data has received
        self.followingData = [-1,-1,-1,-1,-1] #-1 represents leader --> from start everyone is the leader
        self.fanOutData = [-1,-1,-1,-1,-1] #-1 represents no data has received
        self.laneToGoTo = -1
        self.fanOutFlag = False
        self.foundLaneTime = -1


        self.robot_follower = RobotFollower(100,k_p = 0.5)
        #self.serial = SerialHandler('/dev/ttyUSB0', 9600)
        self.turnC = TurnCheck([640, 480], "segImage.png")


        #self.gps_other = GpsClient(self.position_callback_other, color='green')
        self.heartbeat = Heartbeat(self.heartbeat_callback, self.platoon_pos_callback, self.fan_out_callback, self.lane_change_callback)
        self.gps = GpsClient(self.position_callback)
        self.image_recognizer = ImageRecognizer(SVM_PATH)
        self.get_ultrasound()

        time.sleep(2)


if __name__ == '__main__':
    c = SystemControl()
    c.turnC.compute_direction(c.my_x,c.my_y,c.my_theta)
    speed_update = -1
    #print "Direction: " + str(c.turnC.get_direction())
    #print "Get Lane: " + str(c.turnC.get_lane(c.my_x,c.my_y))
    #print "TO do lanes: " + str(c.turnC.get_lane_switch_commands(1))


    try:
        while True:
            c.get_ultrasound()
            if time.time() > speed_update+1:
                c.update_speed()
                speed_update = time.time()
            #

            # if not (c.laneToGoTo == -1):
            #     c.turnC.compute_direction(c.my_x, c.my_y, c.my_theta)
            #     print "lane to go to: " + str(c.laneToGoTo) + "\t current lane: " + str(c.turnC.get_lane(c.my_x,c.my_y)[0])
            #     if not (c.turnC.get_lane(c.my_x,c.my_y)[0] == c.laneToGoTo ):
            #         print "Changeing lanes \t Current state: " + str(c.i2c.get_state())
            #         if(c.i2c.get_state() == 1): #line following atm
            #             print "LANEs TO CHANGE: " + str(c.turnC.get_lane_switch_commands(c.laneToGoTo)) + str("\n")
            #             c.i2c.set_laneChange(c.turnC.get_lane_switch_commands(c.laneToGoTo)[1])
            #     else:
            #         c.laneToGoTo = -1

            c.turnC.compute_direction(c.my_x, c.my_y, c.my_theta)
            #print "lane to go to: " + str(c.laneToGoTo) + "\t current lane: " + str(c.turnC.get_lane(c.my_x, c.my_y)[0]),
            if(c.fanOutFlag):
                print " fanout true",
                if(c.i2c.get_state() == 1):
                    #print " line follow state found lane time: ", c.foundLaneTime, " current time: ", time.time(),
                    if(c.foundLaneTime + 1 < time.time()): # 1 sec delay
                        if(c.laneToGoTo == c.turnC.get_lane(c.my_x,c.my_y)[0]):
                            print " reset fanout ",
                            c.fanOutFlag = False
                            continue
                        lane_cmd = c.turnC.get_lane_switch_commands(c.laneToGoTo)[0]
                        print " lane cmd: ", lane_cmd
                        c.i2c.set_laneChange(lane_cmd)

                if(c.i2c.get_state() == 3):
                    print " lane change state"
                    c.foundLaneTime = time.time()
            else:
                print "fanout false"


            time.sleep(0.1)
        rospy.spin()
    except KeyboardInterrupt:
        print ("Stopping system control!")
        c.vs.stop()
        c.gps.stop()
        
