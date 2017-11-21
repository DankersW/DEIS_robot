# import the necessary packages
from __future__ import print_function
from imutils.video.pivideostream import PiVideoStream
from imutils.video import FPS
from picamera.array import PiRGBArray
from picamera import PiCamera
import argparse
import imutils
import time
import cv2
 
 
# initialize the camera and stream
camera = PiCamera()
camera.resolution = (320, 240)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(320, 240))
stream = camera.capture_continuous(rawCapture, format="bgr",use_video_port=True)
print("[INFO] sampling THREADED frames from `picamera` module...")

vs = PiVideoStream().start()
time.sleep(2.0)

while True:
	frame = vs.read()
	frame = imutils.resize(frame, width=400)
	cv2.imshow("Frame",frame)
	key = cv2.waitKey(1) & 0xFF
	
cv2.destroyAllWindows()
vs.stop()
