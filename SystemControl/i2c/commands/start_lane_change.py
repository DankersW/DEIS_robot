import wiringpi
import sys

fd = wiringpi.wiringPiI2CSetup(0x04)
if fd == -1:
    print "errno: ", errno
    quit()
    
var = int(sys.argv[1])
print "Try lanechange dir: ", var

result = wiringpi.wiringPiI2CWriteReg8(fd,0x22,var)
if result == -1:
    print "Error writing. errno: ", result
else:
    print "Start linefollow at speed ", var
    
