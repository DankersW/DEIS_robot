import cv2
import numpy as np


class TurnCheck(object):


    def __init__(self,imsize,LookUpTableImage):
        self.LookUpTable = cv2.imread(LookUpTableImage,cv2.IMREAD_GRAYSCALE)
    def recognizeImage(self,coord_x,coord_y):
        return self.LookUpTable[coord_y][coord_x]


if __name__ == '__main__':
    ImReg = TurnCheck([480,640],'segImage.png')
    TestImg = cv2.imread('segImage.png');
    lane = ImReg.recognizeImage(76,95)
    print lane
    res = np.multiply(TestImg,TestImg)
    markers8int = np.uint8(TestImg * 50)
    cols = cv2.applyColorMap(markers8int, cv2.COLORMAP_JET)
    cv2.circle(cols,(76,95),3,(255,0,0),-1)
    cv2.imshow('image',cols)
    cv2.waitKey(0)







