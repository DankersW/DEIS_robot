#!/usr/bin/env python
#
#Copyright (c) 2017, Martin Cooney
#All rights reserved.
#THIS SOFTWARE IS PROVIDED "AS IS".
#IN NO EVENT WILL THE COPYRIGHT OWNER BE LIABLE FOR ANY DAMAGES
#ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE.

import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Image
import cv2
import cv_bridge
from cv_bridge import CvBridgeError
from gpsClass import gps_data
from serial_handler import SerialHandler
import math
PARSER = gps_data()
SERIAL_HANDLER = SerialHandler('/dev/ttyUSB0', 9600)
count = 0

def image_callback(data):
    global PARSER
    global SERIAL_HANDLER
    global count
    rospy.loginfo("Received image") #use this for debugging
    try:
        cv_image= cv_bridge.CvBridge().imgmsg_to_cv2(data, "bgr8")
        #cv2.imshow('my_image_feed', cv_image)
        PARSER.image = cv_image
        (x,y) = PARSER.get_x_y()
        if math.isnan(x):
            return
        count = count + 1
        msg = "gps,"+str(int(x)) + ","+str(int(y))
        print "msg: " + msg
        SERIAL_HANDLER.send_message(msg)
        if count == 1:
	    print "sending start gps"
            msg = "drivegps,1000,"+ str(int(x)) + "," + str(int(y))
            SERIAL_HANDLER.send_message(msg)
        print "x: ", x, "y : ", y
        cv2.waitKey(10)
    except CvBridgeError as e:
        print(e)
  
def listener():
    print "listener"
    rospy.init_node('exampleGPSClient', anonymous=True)
    rospy.Subscriber("gps_channel", Image, image_callback)

    rospy.spin()

if __name__ == '__main__':
    #global parser
    print "start"
    listener()
    

	
