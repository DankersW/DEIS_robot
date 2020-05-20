import lcm
from exlcm import position_t
import math
import threading
from time import sleep
class GpsClient(object):
    def callback(self, channel, data):
        self.i = self.i+1
        msg = position_t.decode(data)
        dy = msg.y - self.y
        dx = msg.x - self.x
        
        if(math.sqrt(dx**2+ dy**2) > 2):
            if(abs(dx) < 10):
                dx = 0
            if(abs(dy) < 10):
                dy = 0
            self.theta = math.atan2(dy, dx)
            self.x = msg.x
            self.y = msg.y
            self.position_callback(self.x, self.y, self.theta)
        #print "x: ", self.x, " y: ", self.y, " theta: ", self.theta, " i: ", self.i

    def __init__(self, position_callback):
        self.lc = lcm.LCM("udpm://239.255.76.67:7667?ttl=1")
        self.subscription = self.lc.subscribe("GPS", self.callback)
        self.position_callback = position_callback
        self.go = True
        self.i = 0
        self.x = 0
        self.y = 0
        self.theta = 0
        thread = threading.Thread(target=self.run, args=())
        thread.daemon = True # Daemonize thread
        thread.start()
        
    def run(self):
        while self.go:
            self.lc.handle()
        
    def stop(self):
        self.go = False
    
if __name__ == '__main__':
    t = GpsClient(0)
    while True:
        sleep(0.1)
"""
class GpsClient(object):
	
    def image_callback(self, data):
		
        try:
            cv_image = cv_bridge.CvBridge().imgmsg_to_cv2(data)
            self.gps_parser.image = cv_image
            (pos_x, pos_y, theta) = self.gps_parser.get_x_y(color = self.color)
            self.callback(pos_x, pos_y, theta)
            cv2.waitKey(10)
        except CvBridgeError as error:
            print error

    def __init__(self, position_callback, color):
        self.callback = position_callback
        self.gps_parser = GpsData()
        self.color = color
        rospy.Subscriber("gps_channel", Image, self.image_callback)
"""
