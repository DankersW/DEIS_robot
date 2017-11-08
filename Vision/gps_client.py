import rospy
from sensor_msgs.msg import Image
import cv2
import new_cv_bridge as cv_bridge
from new_cv_bridge import CvBridgeError
from gps_class import GpsData
import threading

class GpsClient(object):
    def image_callback(self, data):
        try:
            cv_image = cv_bridge.CvBridge().imgmsg_to_cv2(data)
            self.gps_parser.image = cv_image
            (pos_x, pos_y, theta) = self.gps_parser.get_x_y()
            self.callback(pos_x, pos_y, theta)
            cv2.waitKey(10)
        except CvBridgeError as error:
            print error

    def __init__(self, position_callback):
        self.callback = position_callback
        self.gps_parser = GpsData()
        rospy.Subscriber("gps_channel", Image, self.image_callback)
