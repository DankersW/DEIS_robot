#!/usr/bin/env python
#
#Copyright (c) 2017, Martin Cooney
#All rights reserved.
#THIS SOFTWARE IS PROVIDED "AS IS".
#IN NO EVENT WILL THE COPYRIGHT OWNER BE LIABLE FOR ANY DAMAGES
#ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE.

import rospy
from std_msgs.msg import String
from subprocess import call
import sys
from sensor_msgs.msg import Image
import cv2
import cv_bridge

def talker():

	rospy.init_node('gpsServer', anonymous=True)
	image_pub= rospy.Publisher("gps_channel", Image, queue_size=10)
	rate=rospy.Rate(10)
	myBridge= cv_bridge.CvBridge()
	#cameraGetImageCommand = "wget --user root --password r3dM1lk -q -O current.jpg http://192.168.1.26/axis-cgi/jpg/image.cgi"
	while not rospy.is_shutdown():
		rospy.loginfo("published image") #for debugging
		#call(cameraGetImageCommand, shell=True)
		img = cv2.imread("/home/pi/ros_ws/src/gps_server/scripts/current.jpg")
		#resized_img = cv2.resize(img, (640, 480)) #use this if you want to use a different sized image
		#convertedImg = myBridge.cv2_to_imgmsg(resized_img, encoding="bgr8")
		convertedImg = myBridge.cv2_to_imgmsg(img, encoding="bgr8")
		image_pub.publish(convertedImg)
		rate.sleep()

if __name__== '__main__':
	try:
		talker()
	except rospy.ROSInterruptException:
		pass

