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
        self.port = serial.Serial(port, baudrate=baud, timeout=5)
        self.lock = mutex.mutex()
        self.target = target
        
        thread = threading.Thread(target=self.run, args=())
        thread.daemon = True  # Daemonize thread
        thread.start() # Start the execution

    def send_message(self, msg):
        self.port.write(msg)
    
    
    
    def run(self):
        """
        Worker method.
        Will run forever and try to read serial port
        """
        while True:
            if self.port.inWaiting():
                #print "Data found"
                #Try to lock mutex
                #lock aquired. Read data
                line = self.port.readline()
                #self.decode_line()
                self.parse_line(line)
                #print line
            time.sleep(0.1)

    def parse_line(self, line=""):
        """
        Static method.
        Takes a string input and parses into a command
        """
        print line
        cmd = CommandFactory.create(line)
        if cmd is None:
            return
        cmd.execute(self.target)




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
