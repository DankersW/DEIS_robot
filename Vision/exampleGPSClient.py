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

def image_callback(data):
    rospy.loginfo("Received image") #use this for debugging
    try:
	print "Sending"
	cv_image= cv_bridge.CvBridge().imgmsg_to_cv2(data, "bgr8")
	cv2.imshow('my_image_feed', cv_image)
	cv2.waitKey(10)
    except CvBridgeError as e:
	print("error")
  
def listener():
    print "listener"
    rospy.init_node('exampleGPSClient', anonymous=True)
    rospy.Subscriber("gps_channel", Image, image_callback)
    rospy.spin()

if __name__ == '__main__':
    print "start"
    listener()
