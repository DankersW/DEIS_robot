import numpy as np

class KalmanFilter(object):

    def __init__(self, process_variance, estimated_measurement_variance):
        self.process_variance = process_variance # Q
        self.estimated_measurement_variance = estimated_measurement_variance # R
        self.posteri_estimate = 0.0 # Xhat
        self.posteri_error_estimate = 1.0 # P

    def input_latest_sensor_measurement(self, measurement):
        self.measurement_z=np.array(measurement) # z
        priori_estimate = self.posteri_estimate # xhatminus[k] = xhat[k-1]
        priori_error_estimate = self.posteri_error_estimate + self.process_variance #    Pminus[k] = P[k-1]+Q

        blending_factor = priori_error_estimate / (priori_error_estimate + self.estimated_measurement_variance) # K[k] = Pminus[k]/( Pminus[k]+R )
        self.posteri_estimate = priori_estimate + blending_factor * (self.measurement_z - priori_estimate) # xhat[k] = xhatminus[k]+K[k]*(z[k]-xhatminus[k])
        self.posteri_error_estimate = (1 - blending_factor) * priori_error_estimate # P[k] = (1-K[k])*Pminus[k]

    def get_latest_estimated_measurement(self):
        return self.posteri_estimate # Xhat