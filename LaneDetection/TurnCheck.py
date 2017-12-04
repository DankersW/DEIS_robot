import cv2
import numpy as np


class TurnCheck(object):


    def __init__(self,imsize,LookUpTableImage):
        self.LookUpTable = cv2.imread(LookUpTableImage,cv2.IMREAD_GRAYSCALE)
    def GetLane(self,coord_x,coord_y):
        return self.LookUpTable[coord_y][coord_x]








