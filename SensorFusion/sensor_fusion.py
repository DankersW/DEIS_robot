
class SensorFusion:
    def __init__(self,odometry_data,gps_data=None,K=0.9):
        self.gps_data=gps_data
        self.odometry_data=odometry_data
        self.K=K


    def fusion(self):
        if self.gps_data:
            #print "gps_data: ", self.gps_data
            #print "od_data: ", self.odometry_data
            
            estimated_x = self.K * self.gps_data[0] + (1-self.K) * self.odometry_data[0]
            estimated_y = self.K * self.gps_data[1] + (1-self.K) * self.odometry_data[1]
            estimated_theta = self.K * self.gps_data[2] + (1-self.K) * self.odometry_data[2]

            estimated_pos=[estimated_x,estimated_y,estimated_theta]
            #print estimated_pos
            return estimated_pos
        else:
            estimated_pos = self.odometry_data

            #print estimated_pos
            return estimated_pos

