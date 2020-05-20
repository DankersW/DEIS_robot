import cv2 as cv
import time
import math
from picamera.array import PiRGBArray
from picamera import PiCamera
import numpy as np
from ImageRecognizer import ImageRecognizer

camera = PiCamera()
height = 320
width = 320
camera.resolution = (width,height)
camera.exposure_mode = 'auto'
camera.framerate = 32
camera.vflip = 180
camera.brightness = 60
camera.hflip = 180
#camera.exposure_compensation = 10


ImReg = ImageRecognizer('bof.pkl')


cv.namedWindow("Image", 1)


rawCapture = PiRGBArray(camera)
stream = camera.capture_continuous(rawCapture, format ="bgr", use_video_port = True)
    
    
for (i,f) in enumerate(stream):
	#time.sleep(1.0)
	image = f.array
	#image = cv.resize(image,(128,128)) 
	image = image[56:184,96:224]
	cv.imshow("Image",image)

	if(image is not None):
		print ImReg.recognizeImage(image)
	
	#cv.imwrite('wall' + str(i) + '.png',image)
	#if(i == 200):
	#	break
	if cv.waitKey(1) ==0x1b:
		#cv.imwrite("pic.jpg",image)
		break
		
	rawCapture.truncate(0)
                  
