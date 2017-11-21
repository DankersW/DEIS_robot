import wiringpi
import sys

fd = wiringpi.wiringPiI2CSetup(0x04)
if fd == -1:
    print "errno: ", errno
    quit()
    
speed = int(sys.argv[1])
print "Trying to linefollow at speed: ", speed

result = wiringpi.wiringPiI2CWriteReg8(fd,0x20,speed)
if result == -1:
    print "Error writing. errno: ", result
else:
    print "Start linefollow at speed ", speed
    
