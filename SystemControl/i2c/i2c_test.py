import smbus
import time
bus = smbus.SMBus(1)

#This is the address we setup in the Arduino Program
address = 0x04
var = 0


def readNumberData(reg):
    x = (bus.read_byte_data(address,reg))
    time.sleep(0.1)
    print x    
def writeNumber(value):
    try:
        bus.write_byte(address, value)
        return True
    except:
        print "Failed to write on bus. Retry"
        return False
    
    # bus.write_byte_data(address, 0, value)
    return -1

def readNumber():
    number = bus.read_byte(address)
    # number = bus.read_byte_data(address, 1)
    print "Arduino: Hey RPI, I received a digit ", number
    return number


def readReg(reg):
    number = bus.read_byte_data(address,reg)
    print "Arduino: Hey RPI, I received a digit ", number
    return number
while True:
    var = var + 1
    #var = #input("Enter 1 - 9: ")
    if not var:
        continue
    writeNumber(2)
    #while not writeNumber(var):
    #    pass
    #writeBlock(var)
    
    #print "RPI: Hi Arduino, I sent you ", var
    # sleep one second
    #time.sleep(0.01)
    time.sleep(1)
    readReg(1)
    time.sleep(1)
    #number =
    #readNumber()
    #readNumberData(201)
    #readNumberData(202)
    #readNumberData(203)
    #number = readBlock()
    print
