import wiringpi
import time

fd = wiringpi.wiringPiI2CSetup(0x04)
if fd == -1:
    print "errno: ", errno
    quit()

var = 0
start = time.time()
while True:
    var = var + 1
    #var = #input("Enter 1 - 9: ")
    if not var:
        continue
    result = wiringpi.wiringPiI2CWriteReg8(fd,1,var)
    if result == -1:
        print "Error writing. errno: ", result
    if time.time() - start >= 10:
        print "RPI: ", var
        print
        start = time.time()
    
    #result = wiringpi.wiringPiI2CWrReg8
    #time.sleep(1)
    #print
