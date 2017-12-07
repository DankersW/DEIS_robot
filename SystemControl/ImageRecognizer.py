import cv2
import time
import numpy as np
from sklearn.externals import joblib
from sklearn.preprocessing import normalize





class ImageRecognizer:

    def __init__(self,PATH_TO_SVM_FILE):
        # Create feature extraction and keypoint detector objects
        self.surf = cv2.xfeatures2d.SURF_create(extended = True)
        
        # Load the classifier, class names, scaler, number of clusters and vocabulary
        self.clf, self.classes_names, self.k, self.Kcl = joblib.load(PATH_TO_SVM_FILE)
        self.avg_pred = np.zeros(3)
        self.count = 0
        self.flag = False

    def recognizeImage(self,img):
        #start_time = time.time()
       
        kpts, des = self.surf.detectAndCompute(img,None) 

        #prepare test_feature array
        test_features = np.zeros((1, self.k), "float32")

        # Computing the histogram of features
        if des is not None:
			words = self.Kcl.predict(des)
			for w in words:
				test_features[0][w] += 1  # Calculating the histogram of features

			# Scale the features
			test_features = normalize(test_features, norm='l2', copy=False)
			# Perform the prediction
			# prediction = [self.classes_names[i] for i in self.clf.predict(test_features)]
			
			prediction = self.clf.predict(test_features)
			#print("--- Calculation time: %s seconds ---" % (time.time() - start_time))
			
			indx = np.where(prediction == 1)
			
			if len(indx[0]):
				if (len(indx[1]) == 1):
					self.avg_pred[self.count] = indx[1]+1
					self.count += 1
					if(self.count == 3):
						self.count = 0
						self.flag = True
				
			else:
				self.flag = False
				#self.avg_pred.fill(0)
				self.count = 0
				
						
				
			if self.flag:
				return int(round(np.mean(self.avg_pred)))	
			return 0
			
        return "no descriptors"
			
        

