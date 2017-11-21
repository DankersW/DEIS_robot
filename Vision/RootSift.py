import numpy as np
import cv2



class RootSIFT:

    def __init__(self):
        self.sift = cv2.xfeatures2d.SIFT_create()


    def detectAndCompute(self, img, eps=1e-7):
        (kps,descs) = self.sift.detectAndCompute(img,None)

        if len(kps) == 0:
            return ([],None)

        descs /= (descs.sum(axis=1, keepdims=True) + eps)
        descs = np.sqrt(descs)

        return (kps,descs)