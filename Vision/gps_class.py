import cv2
import numpy as np
import math



class GpsData:
    def __init__(self):
        self.image = cv2.imread('current.PNG',1)
        self.x = 0
        self.y = 0
        self.theta = 0
        self.x_offset = 0
        self.y_offset = 0
        #self.i = 0
        
    '''def __del__(self):
        self.image.release()'''
    
    
        
    def get_x_y(self):
    
   
        '''
        #assign strings for ease of coding
        hh='Hue High'
        hl='Hue Low'
        sh='Saturation High'
        sl='Saturation Low'
        vh='Value High'
        vl='Value Low'
        wnd = 'Colorbars'
        cv2.namedWindow('Colorbars') #Create a window named 'Colorbars'
        #Begin Creating trackbars for each
        cv2.createTrackbar(hl, wnd,0,179,lambda x:x)
        cv2.createTrackbar(hh, wnd,0,179,lambda x:x)
        cv2.createTrackbar(sl, wnd,0,255,lambda x:x)
        cv2.createTrackbar(sh, wnd,0,255,lambda x:x)
        cv2.createTrackbar(vl, wnd,0,255,lambda x:x)
        cv2.createTrackbar(vh, wnd,0,255,lambda x:x)
    
        #self.i = self.i +1
        #print "images ",self.i,
        
        '''
        
        self.image = cv2.GaussianBlur(self.image,(5,5),0)
        
        #cv2.imshow('my_image_feed', self.image)
        
        img_hsv = cv2.cvtColor(self.image, cv2.COLOR_BGR2HSV)
        window = np.ones((3, 3), np.uint8)
        
        '''
        #read trackbar positions for each trackbar
        hul=cv2.getTrackbarPos(hl, wnd)
        huh=cv2.getTrackbarPos(hh, wnd)
        sal=cv2.getTrackbarPos(sl, wnd)
        sah=cv2.getTrackbarPos(sh, wnd)
        val=cv2.getTrackbarPos(vl, wnd)
        vah=cv2.getTrackbarPos(vh, wnd)
        
        #make array for final values
        HSVLOW=np.array([hul,sal,val])
        HSVHIGH=np.array([huh,sah,vah])
        mask = cv2.inRange(img_hsv,HSVLOW, HSVHIGH)
        #mask = cv2.dilate(mask,window,iterations=1)
        
        rows, cols = np.where(mask == 255)
        
        '''
        
        #img_hsv = cv2.cvtColor(self.img, cv2.COLOR_BGR2HSV)
        #window = np.ones((3, 3), np.uint8)
        lower_pink = np.array([157, 26, 26])
        upper_pink = np.array([170, 255, 255])
        mask = cv2.inRange(img_hsv, lower_pink, upper_pink)
        #mask = cv2.erode(mask, window, iterations=0)
        #cv2.imshow('mask', mask)
        rows, cols = np.where(mask == 255)
        image_height, image_width, channel = img_hsv.shape
        
        
        
        #cv2.imshow(wnd, mask) 
           
        x_median = np.median(cols)
        y_median = np.median(rows)
        
        print "x_median", x_median, "y_median", y_median
        
        if(!math.isnan(x_median) and !math.isnan(y_median)):
            x_coordinate = int((2250*np.median(cols) / image_width)/10)*10 + self.x_offset # check_col needs to be multiplied by real width of the table seen the camera
            y_coordinate = int((1650*np.median(rows) / image_height)/10)*10 + self.y_offset # check_row needs to be multiplied by real height of the table seen   by the camera

            dy = y_coordinate - self.y
            dx = x_coordinate - self.x
            
            if(dx != 0 and dy != 0):
                self.theta = math.atan2(dy, dx)
                
            self.x = x_coordinate
            self.y = y_coordinate
        
        return self.x, self.y , self.theta
        

#first_coordinate = gps_data()
#print first_coordinate.get_x_y()
