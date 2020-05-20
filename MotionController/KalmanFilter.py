import numpy as np

class KalmanFilter(object):

    def __init__(self,odometry_data, process_variance=1e-5, estimated_measurement_variance=0.1**2):
        self.Q = process_variance # Q process_variance
        self.R = estimated_measurement_variance # R estimated_measurement_variance
        self.Xhat = odometry_data # Xhat estimated_measurement_variance
        self.P = 1.0 # P posteri_error_estimate

    def input_latest_sensor_measurement(self, measurement): # GPS measurment
        self.z=np.array(measurement) # z
        Xhatminus = self.Xhat # priori_estimate = self.posteri_estimate xhatminus[k] = xhat[k-1]
        Pminus = self.P + self.Q #priori_error_estimate = posteri_error_estimate + process_variance     Pminus[k] = P[k-1]+Q

        K = Pminus / (Pminus + self.R) # blending_factor = priori_error_estimate / (priori_error_estimate + estimated_measurement_variance) # K[k] = Pminus[k]/( Pminus[k]+R )
        self.Xhat = Xhatminus + K * (self.z - Xhatminus) # posteri_estimate = priori_estimate + blending_factor * (measurement_z - priori_estimate) \\ xhat[k] = xhatminus[k]+K[k]*(z[k]-xhatminus[k])
        self.P = (1 - K) * Pminus # posteri_error_estimate = (1 - blending_factor) * priori_error_estimate \\ P[k] = (1-K[k])*Pminus[k]

    def get_latest_estimated_measurement(self):
        return self.Xhat # Xhat posteri_estimate