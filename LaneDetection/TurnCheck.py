import cv2
import numpy as np
import math
import random


class TurnCheck(object):


    def __init__(self,imsize,LookUpTableImage):
        self.LookUpTable = cv2.imread(LookUpTableImage,cv2.IMREAD_GRAYSCALE)
        self.DirectionLookUpTable = np.zeros((imsize[1],imsize[0]),dtype=np.uint8)

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

        #cv2.imshow('Direction',self.DirectionLookUpTable*50)
        #cv2.waitKey(0)



    def GetLane(self,coord_x,coord_y,orientation):
        orient = (round(2*orientation/math.pi) * math.pi/2)
        print orient
        cw = False
        if(orientation == math.pi or orientation == -math.pi):
            if(self.DirectionLookUpTable[coord_x,coord_y] == 2 or self.DirectionLookUpTable[coord_x,coord_y] == 3):
                cw = True
        elif(orientation == math.pi/2):
            if (self.DirectionLookUpTable[coord_x, coord_y] == 1 or self.DirectionLookUpTable[coord_x, coord_y] == 2):
                cw = True
        elif(orientation == -math.pi/2):
            if (self.DirectionLookUpTable[coord_x, coord_y] == 4 or self.DirectionLookUpTable[coord_x, coord_y] == 3):
                cw = True
        else:
            if (self.DirectionLookUpTable[coord_x, coord_y] == 1 or self.DirectionLookUpTable[coord_x, coord_y] == 4):
                cw = True

        Lane = self.LookUpTable[coord_x][coord_y]
        if cw:
            print 'CW'
            if Lane == 1 :
                return [Lane, 1] # right
            elif Lane == 4:
                return [Lane, 0] # left
            else:
                return [Lane, random.randint(0,1)] #random

        else:
            print 'CCW'
            if Lane == 1:
                return [Lane, 0]
            elif Lane == 4:
                return [Lane, 1]
            else:
                return [Lane, random.randint(0,1)]













