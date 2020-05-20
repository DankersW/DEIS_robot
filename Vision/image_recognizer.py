import cv2
import time
import numpy as np
from sklearn.externals import joblib
from scipy.cluster.vq import *


class ImageRecognizer:

    def __init__(self,PATH_TO_SVM_FILE):
        # Create feature extraction and keypoint detector objects
        self.sift = cv2.xfeatures2d.SIFT_create()
        # Load the classifier, class names, scaler, number of clusters and vocabulary
        self.clf, self.classes_names, self.stdSlr, self.k, self.voc = joblib.load(PATH_TO_SVM_FILE)

    def recognizeImage(self,img):
        start_time = time.time()
        kpts, des = self.sift.detectAndCompute(img, None)

        #prepare test_feature array
        test_features = np.zeros((1, self.k), "float32")

        # Computing the histogram of features
        words, distance = vq(des, self.voc)
        for w in words:
            test_features[0][w] += 1  # Calculating the histogram of features

        # Scale the features
        test_features = self.stdSlr.transform(test_features)

        # Perform the prediction
        prediction = [self.classes_names[i] for i in self.clf.predict(test_features)]
        print("--- Calculation time: %s seconds ---" % (time.time() - start_time))
        return prediction

if __name__ == '__main__':
    ImReg = ImageRecognizer("C:\Users\Alexander\PycharmProjects\untitled1\\bof.pkl")
    print ImReg.recognizeImage(cv2.imread("D:\UNI_MAT\DEIS\TestImages\Sign\Sign2.png"))