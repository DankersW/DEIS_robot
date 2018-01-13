import cv2
import numpy as np
import math
import random


class TurnCheck(object):


    def __init__(self,imsize,LookUpTableImage):

        self.LookUpTable = cv2.imread(LookUpTableImage,cv2.IMREAD_GRAYSCALE)
        self.DirectionLookUpTable = np.zeros((imsize[1],imsize[0]),dtype=np.uint8)
        self.cw = False
        self.lane = 1
        for y in range(imsize[0]):
            for x in range (imsize[1]):
                if y >= imsize[0]/2:
                    if x >= imsize[1]/2:
                        self.DirectionLookUpTable[x,y] = 4
                    else:
                        self.DirectionLookUpTable[x,y] = 1
                else:
                    if x >= imsize[1]/2:
                        self.DirectionLookUpTable[x,y] = 3
                    else:
                        self.DirectionLookUpTable[x,y] = 2


    def get_direction(self):
        return self.cw

    def compute_direction(self,coord_x,coord_y,orientation):
        orient = (round(2*orientation/math.pi) * math.pi/2)
        #print orient
        self.cw = False

        if(orient == math.pi or orient == -math.pi):
            if(self.DirectionLookUpTable[coord_x,coord_y] == 2 or self.DirectionLookUpTable[coord_x,coord_y] == 3):
                self.cw = True
        elif(orient == math.pi/2):
            if (self.DirectionLookUpTable[coord_x, coord_y] == 1 or self.DirectionLookUpTable[coord_x, coord_y] == 2):
               self.cw = True
        elif(orient == -math.pi/2):
            if (self.DirectionLookUpTable[coord_x, coord_y] == 4 or self.DirectionLookUpTable[coord_x, coord_y] == 3):
                self.cw = True
        else:
            if (self.DirectionLookUpTable[coord_x, coord_y] == 1 or self.DirectionLookUpTable[coord_x, coord_y] == 4):
                self.cw = True
        if self.cw:
            print  "cw ",
        else:
            print "ccw"

    def get_lane(self,coord_x,coord_y):

        self.lane = self.LookUpTable[coord_x,coord_y]

        if self.cw:
            if self.lane == 1:
                return [self.lane, 0] # right
            elif self.lane == 4:
                return [self.lane, 1] # left
            else:
                return [self.lane, random.randint(0,1)] #random

        else:
            if self.lane == 1:
                return [self.lane, 1]
            elif self.lane == 4:
                return [self.lane, 0]
            else:
                return [self.lane, random.randint(0,1)]

    def get_lane_switch_commands(self,Lane_to_change_to):

        lanechange = self.lane - Lane_to_change_to
        print "self lane: ", self.lane , " lane to: ", Lane_to_change_to ,
        if(lanechange<0):
            if self.cw:
                print "turn right cw",
                return [0,abs(lanechange)] # right
            else:
                print "turn left cw",
                return [1,abs(lanechange)] # left
        elif(lanechange>0):
            if self.cw:
                print "turn left ccw",
                return [1,abs(lanechange)]
            else:
                print "turn right ccw",
                return [0,abs(lanechange)]
        else:
            return [-1,-1] # no change we are already here















