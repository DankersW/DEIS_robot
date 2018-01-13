
import threading
import rospy
from std_msgs.msg import String

class Heartbeat:

    def __init__(self, heartbeat_callback, platoon_pos_callback, fan_out_callback, lane_change_callback):
        self.x = 0
        self.y = 0
        self.theta = 0
        self.go = True
        self.platoonPosFlag = False
        self.laneChangePosFlag = False
        self.toFollowId = -2
        self.laneChangePosMessage = ""

        self.heartbeat_callback     = heartbeat_callback
        self.platoon_pos_callback   = platoon_pos_callback
        self.fan_out_callback       = fan_out_callback
        self.lane_change_callback   = lane_change_callback

        thread = threading.Thread(target=self.run, args=())
        thread.daemon = True # Daemonize thread
        thread.start()

        rospy.Subscriber("heartbeat_channel", String, self.heartbeat_callback)          #channel for position data
        rospy.Subscriber("platoon_pos_channel", String, self.platoon_pos_callback)      #channel to receive who is following who
        rospy.Subscriber("fan_out_channel", String, self.fan_out_callback)              #channel to post to cancel a platoon
        rospy.Subscriber("lane_change_channel", String, self.lane_change_callback)          #channel to tell who needs to go to what lane

        #thread_listen = threading.Thread(target=self.listen, args=())
        #thread_listen.daemon = True # Daemonize thread
        #thread_listen.start()

    def stop(self):
        """
        Running this method will stop the thread
        """
        self.go = False

    def update_gps(self, pos_x, pos_y, theta):
        self.x = pos_x
        self.y = pos_y
        self.theta = theta

    def update_platoon_pos(self, toFollowId):
        self.toFollowId = toFollowId
        self.platoonPosFlag = True

    def update_lane_change_pos(self, message):
        self.laneChangePosMessage = message
        self.laneChangePosFlag = True

    def run(self):
        try:
            pub_heartbeat = rospy.Publisher('heartbeat_channel', String, queue_size=1)
            pub_platoonPos = rospy.Publisher('platoon_pos_channel', String, queue_size=1)
            pub_laneChangePos = rospy.Publisher('lane_change_channel', String, queue_size=1)
            #rospy.init_node('g3_send', anonymous=True)
            rate = rospy.Rate(10) # 10hz
            while not rospy.is_shutdown() and self.go:
                #heartbeat_message = "3," + str(self.x) + "," + str(self.y)
                #pub_heartbeat.publish(heartbeat_message)

                if(self.platoonPosFlag):
                    platoonPos_message = "3," + str(self.toFollowId)
                    pub_platoonPos.publish(platoonPos_message)
                    self.platoonPosFlag = False

                if(self.laneChangePosFlag):
                    pub_laneChangePos.publish(self.laneChangePosMessage)
                    self.laneChangePosFlag = False
                #rospy.loginfo(hello_str)
                rate.sleep()

        except rospy.ROSInterruptException:
            pass

def callback(data):
    rospy.loginfo(rospy.get_caller_id() + ' I heard %s', data.data)

if __name__ == '__main__':
    _ = Heartbeat(callback)
