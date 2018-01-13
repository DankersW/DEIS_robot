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
        result = wiringpi.wiringPiI2CWriteReg16(self.file_descriptor,0x10,x) #og: fd???
        if result == -1:
            print "Error writing. errno: ", result
        else:
            print "Set x to: ", x
        
        result = wiringpi.wiringPiI2CWriteReg16(self.file_descriptor,0x12,y)
        if result == -1:
            print "Error writing. errno: ", result
        else:
            print "Set y to: ", y
        
        result = wiringpi.wiringPiI2CWriteReg16(self.file_descriptor,0x14,int(theta*1000))
        if result == -1:
            print "Error writing. errno: ", result
        else:
            print "Set y to: ", y

    def get_ultrasound(self):
        result = wiringpi.wiringPiI2CReadReg8(self.file_descriptor, 0x30)
        if result == -1:
            return result
        else:
            return result

    def set_idleState(self):
        result = wiringpi.wiringPiI2CWriteReg8(self.file_descriptor, 0x80, 1)
        if result == -1:
            print "Error writing. errno: ", result
        else:
            print "State set to IDLE"

    def set_laneChange(self, direction):
        result = wiringpi.wiringPiI2CWriteReg8(self.file_descriptor, 0x22, direction)
        if result == -1:
            print "Error writing. errno: ", result
        else:
            print "Changing lane to: ", direction

    def set_tone(self, tone):
        result = wiringpi.wiringPiI2CWriteReg8(self.file_descriptor, 0x40, tone)
        if result == -1:
            print "Error writing. errno: ", result
        else:
            print "Setting tone to: ", tone

    def set_scoop(self, scoop):
        result = wiringpi.wiringPiI2CWriteReg8(self.file_descriptor, 1, scoop)
        if result == -1:
            print "Error writing. errno: ", result
        else:
            print "Setting scoop to: ", scoop

    def get_state(self):
        result = wiringpi.wiringPiI2CReadReg8(self.file_descriptor, 0x90)
        if result == -1:
            return result
        else:
            return result


        
    
