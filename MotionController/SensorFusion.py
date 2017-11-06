
class SensorFusion:
    def __init__(self,gps_data,odometry_data):
        self.gps_data=gps_data
        self.odometry_data=odometry_data


    def fusion(self):
        if self.gps_data:
            #print self.gps_data

            estimated_x = 0.9 * self.gps_data[0] + 0.1 * self.odometry_data[0]
            estimated_y = 0.9 * self.gps_data[1] + 0.1 * self.odometry_data[1]
            estimated_theta = 0.9 * self.gps_theta[2] + 0.1 * self.odometry_data[2]

            estimated_pos=[estimated_x,estimated_y,estimated_theta]
            print estimated_pos
            return estimated_pos
        else:
            estimated_pos = self.odometry_data

            print estimated_pos
            return estimated_pos

