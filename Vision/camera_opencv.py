import cv2 as cv
import time
import lcm
import math
from picamera.array import PiRGBArray
from picamera import PiCamera
import numpy as np
from exlcm import example_t
camera = PiCamera()
height = 240
width = 320
camera.resolution = (width,height)
camera.exposure_mode = 'off'
#camera.exposure_compensation = 10



cv.namedWindow("Image", 1)

lower_red = np.array([0,25,25])
upper_red = np.array([2,255,255])
element = np.ones((5,5),np.uint8)

#http://www.ginifab.com/feeds/pms/pms_color_in_image.php  192.168.80.249

lc = lcm.LCM()

msg = example_t()
msg.timestamp = int(time.time() * 1000000)
msg.position = (1, 2, 3)
msg.orientation = (1, 0, 0, 0)
msg.ranges = range(15)
msg.num_ranges = len(msg.ranges)
msg.name = "warning"
msg.enabled = True

lc.publish("CAM", msg.encode())


while True:
    rawCapture = PiRGBArray(camera)
    camera.capture(rawCapture, format ="bgr")
    image = rawCapture.array
    hsv_im = cv.cvtColor(image,cv.COLOR_BGR2HSV)
    
    mask = cv.inRange(hsv_im,lower_red,upper_red)
    mask = cv.erode(mask,element,iterations = 2)
    
    rows, cols = np.where(mask == 255)

    check = np.median(cols)

    
    if(check > 3/4*width):
        print('Object is on the right')
        msg.name = "warning: Object is on the right"
        lc.publish("EXAMPLE", msg.encode())
    elif(1/4*width<check<3/4*width):
        print('Object is in the center')
        msg.name = "warning: Object is in the center"
        lc.publish("EXAMPLE", msg.encode())
    elif(check <1/4*width):
        print('Object is on the left')
        msg.name = "warning: Object is on the left"
        lc.publish("EXAMPLE", msg.encode())
    elif(math.isnan(check)):
        print('No object')
    




    
    #image = cv.equalizeHist(image)
    
    cv.imshow("Image",mask)
    if cv.waitKey(1) ==1:
        break

                  
