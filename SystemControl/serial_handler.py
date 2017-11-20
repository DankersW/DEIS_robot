"""
Serial module
"""
import threading
import time
import mutex
import sys
import select
# non-default imports
#import lcm
import serial

from serial_parser import CommandFactory
#data types


lock = mutex.mutex()

def print2(msg):
    global lock
    while not lock.testandset:
        time.sleep(0)
    print msg
    lock.unlock()

class SerialHandler(object):
    """
    Receives data on uart. runs itself on a background thread
    Uses a mutex lock. Might not be necessary,
    all other threads will only read.

    TODO: Receive commands, send on uart.
    """
    #serial.Serial('/dev/ttyUSB1', baudrate=4800, timeout=1)
    def __init__(self, port, baud, target):
        self.data = 0
        self.command = 0
        self.port = serial.Serial(port, baudrate=baud, timeout=1)
        self.lock = mutex.mutex()
        self.target = target

    def send_message(self, msg):
        self.port.write(msg)

    @staticmethod
    def parse_line(line=""):
        """
        Static method.
        Takes a string input and parses into a command
        """
        cmd = CommandFactory.create(line)
        cmd.execute()
        #msg = sensor_vals_t()
        #msg.timestamp = int(time.time())
        #result = [x.strip() for x in line.split(',')]

        #if len(result) > 2 and result[0] == "ALL":
            #msg.wheel_encoders.left = long(result[1])
            #msg.wheel_encoders.right = long(result[2])
            #msg.line_sensors.left = result[3]
            #msg.line_sensors.middle = result[4]
            #msg.line_sensors.right = result[5]
            #return msg

        #if len(result) > 1 and result[0] == "DEBUG":
        #print result[1]
        return None




def main():
    """
    Module test code
    """

    # Starting background thread
    _ = SerialHandler('/dev/ttyUSB1', 19200)
    print2("starting loop")
    while True:
        #print "knock knock"
        time.sleep(1)


if  __name__ == '__main__':
    main()
