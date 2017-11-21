import wiringpi
#import sys

class I2CHandler:

    def __init__(self):
        self.file_descriptor = wiringpi.wiringPiI2CSetup(0x04)
        if self.file_descriptor == -1:
            print "errno: ", self.file_descriptor
            quit()


    def startLineFollow(self, speed):
        result = wiringpi.wiringPiI2CWriteReg8(self.file_descriptor,0x20,speed)
        if result == -1:
            print "Error writing. errno: ", result
        else:
            print "Start linefollow at speed ", speed

        
    def setGPS(self, x,y,theta):
        result = wiringpi.wiringPiI2CWriteReg16(fd,0x10,x)
        if result == -1:
            print "Error writing. errno: ", result
        else:
            print "Set x to: ", x
        
        result = wiringpi.wiringPiI2CWriteReg16(fd,0x12,y)
        if result == -1:
            print "Error writing. errno: ", result
        else:
            print "Set y to: ", y
        
        result = wiringpi.wiringPiI2CWriteReg16(fd,0x14,int(theta*1000))
        if result == -1:
            print "Error writing. errno: ", result
        else:
            print "Set y to: ", y
        
    
