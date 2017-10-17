"""
Serial module
"""
import threading
import time
import mutex
import sys
import select
# non-default imports
import lcm
import serial
import select
#data types

sys.path.append('../LCM')

from exlcm import sensor_vals_t
from exlcm import wheel_speeds_t

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
    def __init__(self, port, baud):
        self.data = 0
        self.command = 0
        self.port = serial.Serial(port, baudrate=baud, timeout=1)
        self.lock = mutex.mutex()
        self.lcm = lcm.LCM()
        self.lcm.subscribe("COMMAND", self.command_receiver)

        thread = threading.Thread(target=self.run, args=())
        thread.daemon = True  # Daemonize thread
        thread.start() # Start the execution

        lcm_thread = threading.Thread(target=self.poll_lcm, args=())
        lcm_thread.daemon = True  # Daemonize thread
        lcm_thread.start() # Start the execution
        


    def poll_lcm(self):
        """
        TODO: Maybe add a sleep
        """
        timeout = 0
        while True:
            rfds,_,_ = select.select([self.lcm.fileno()], [], [], timeout)
            if rfds:
                self.lcm.handle()
            time.sleep(0)

    def command_receiver(self, channel, data):
        """
        Receives messages on LCM Channel COMMAND.
        Parses and sends over uart
        """
        msg = wheel_speeds_t.decode(data)
        text = "l: " + str(msg.wheel.left) + "\tr: " +str(msg.wheel.right)
        print2(text)
        data = ""
        if msg.wheel.left == 0.0 and msg.wheel.right == 0.0:
            data = "stop\r\n"  
        else:
            data = "drive," + str(msg.wheel.left) + "," + str(msg.wheel.right) + "\r\n"

        print2("SERIAL SEND:%" + str(data) + "%")
        self.port.flushOutput()
        self.port.write(data)

        
        
            
    def run(self):
        """
        Worker method.
        Will run forever and try to read serial port.
        """
        while True:
            while self.port.inWaiting():
                #self.print("Data found")
                #Try to lock mutex
                while not self.lock.testandset:
                    time.sleep(0.01)
                #lock aquired. Read data
                line = self.port.readline()
                msg = SerialHandler.parse_line(line)
                if msg is not None:
                    #print line
                    self.lcm.publish("SENSOR", msg.encode())
                self.lock.unlock()
            time.sleep(0.1)

    #
    @staticmethod
    def parse_line(line=""):
        """
        Static method.
        takes a string of values and encodes into sensor_values_t type
        """

        msg = sensor_vals_t()
        msg.timestamp = int(time.time())
        result = [x.strip() for x in line.split(',')]

        if len(result) > 2 and result[0] == "ALL":
            msg.wheel_encoders.left = long(result[1])
            msg.wheel_encoders.right = long(result[2])
            msg.line_sensors.left = result[3]
            msg.line_sensors.middle = result[4]
            msg.line_sensors.right = result[5]
            return msg

        if len(result) > 1 and result[0] == "DEBUG":
            print result[1]
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
