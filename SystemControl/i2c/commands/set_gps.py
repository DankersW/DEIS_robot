import wiringpi
import sys
import time

fd = wiringpi.wiringPiI2CSetup(0x04)
if fd == -1:
    print "errno: ", errno
    quit()
    
x = int(sys.argv[1])
y = int(sys.argv[2])
theta = int(sys.argv[3])

print "Trying to set X to: ", x

result = wiringpi.wiringPiI2CWriteReg16(fd,0x10,x)
if result == -1:
    print "Error writing. errno: ", result
else:
    print "X set to: ", x
time.sleep(0.5)
result = wiringpi.wiringPiI2CReadReg16(fd, 0x10)
if result == -1:
    print "Error writing. errno: ", result
else:
    print "read x to be: ", result
    
print "Trying to set Y to: ", y
time.sleep(0.5)
result = wiringpi.wiringPiI2CWriteReg16(fd,0x12,y)
if result == -1:
    print "Error writing. errno: ", result
else:
    print "Y set to: ", y
time.sleep(0.5)

result = wiringpi.wiringPiI2CReadReg16(fd, 0x12)
if result == -1:
    print "Error writing. errno: ", result
else:
    print "read y to be: ", result


print "Trying to set Theta to: ", theta
time.sleep(0.5)
result = wiringpi.wiringPiI2CWriteReg16(fd,0x14,theta)
if result == -1:
    print "Error writing. errno: ", result
else:
    print "Theta set to: ", theta
time.sleep(0.5)

result = wiringpi.wiringPiI2CReadReg16(fd, 0x14)
if result == -1:
    print "Error writing. errno: ", result
else:
    print "read theta to be: ", result

