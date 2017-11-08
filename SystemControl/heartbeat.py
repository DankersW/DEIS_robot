
import threading
import rospy
from std_msgs.msg import String

class Heartbeat:

    def __init__(self, heartbeat_callback):
        self.x = 0
        self.y = 0
        self.go = True
        self.heartbeat_callback = heartbeat_callback
        thread = threading.Thread(target=self.run, args=())
        thread.daemon = True # Daemonize thread
        thread.start()

        rospy.Subscriber("heartbeat_channel", String, self.heartbeat_callback)
        #thread_listen = threading.Thread(target=self.listen, args=())
        #thread_listen.daemon = True # Daemonize thread
        #thread_listen.start()

    def stop(self):
        """
        Running this method will stop the thread
        """
        self.go = False

    def update_gps(self, pos_x, pos_y):
        self.x = pos_x
        self.y = pos_y

    def run(self):
        try:
            pub = rospy.Publisher('heartbeat_channel', String, queue_size=1)
            #rospy.init_node('g3_send', anonymous=True)
            rate = rospy.Rate(10) # 10hz
            while not rospy.is_shutdown() and self.go:
                hello_str = "3," + str(self.x) + "," + str(self.y)
                #rospy.loginfo(hello_str)
                pub.publish(hello_str)
                rate.sleep()

        except rospy.ROSInterruptException:
            pass

def callback(data):
    rospy.loginfo(rospy.get_caller_id() + ' I heard %s', data.data)

if __name__ == '__main__':
    _ = Heartbeat(callback)
