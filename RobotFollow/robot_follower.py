import math
import numpy as np


class RobotFollower:

    def __init__(self, distance=50, k_p = 10,k_i = 10, memory_i = 10):
        #K_p:   amplification factor for P-controller & PI-controller
        #K_p:   amplification factor for PI-controller
        #Memory_i:  number of past values considered in PI-controller
        #Dist   distance to other robot
        self.K_p = k_p
        self.K_i = k_i
        self.Dist = distance
        self.I_array = np.zeros((1, memory_i), dtype=float)

        self.Memory_i = memory_i
        self.C = 0



    def computeVelocity_P(self, coord_Self, coord_Other):

        #Simple P-controller

        dx = coord_Self[0]-coord_Other[0]
        dy = coord_Self[1]-coord_Other[0]
        dd = math.sqrt(dx**2+dy**2)-self.Dist

        if dd < 0:
            return 0
        else:
            return self.K_p*dd


    def computeVelocity_PI(self, coord_Self, coord_Other):

        # Simple PI-controller
        dx = coord_Self[0] - coord_Other[0]
        dy = coord_Self[1] - coord_Other[0]
        dd = math.sqrt(dx**2+dy**2)-self.Dist

        if dd < 0:
            return 0
        else:
            self.C += 1
            self.C %= self.Memory_i
            self.I_array[self.C] = dd
            return self.K_p * dd + self.K_i * self.I_array.mean()







