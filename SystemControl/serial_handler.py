"""
Serial module
"""
import threading
import time
import mutex
import sys
# non-default imports
import lcm
import serial
#data types

sys.path.append('../LCM')

from exlcm import sensor_vals_t
from exlcm import wheel_speeds_t


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
        TODO: Maybe add a timeout and a sleep
        """
        while True:
            self.lcm.handle()

    def command_receiver(self, channel, data):
        """
        Receives messages on LCM Channel COMMAND.
        Parses and sends over uart
        """
        print "Command received on channel ", channel
        msg = wheel_speeds_t.decode(data)
        print "l: ", msg.wheel.left, "\nr: ", msg.wheel.right
        data = "drive," + str(msg.wheel.left) + "," + str(msg.wheel.right) + "\r\n"
        self.port.write(data)

    def run(self):
        """
        Worker method.
        Will run forever and try to read serial port.
        """
        while True:
            if self.port.inWaiting():
                print "Data found"
                #Try to lock mutex
                while not self.lock.testandset:
                    time.sleep(1)
                #lock aquired. Read data
                line = self.port.readline()
                #self.decode_line()
                msg = SerialHandler.encode_line(line)
                print line
                self.lcm.publish("SENSOR", msg.encode())
                self.lock.unlock()
            time.sleep(0.1)

    #
    @staticmethod
    def encode_line(line=""):
        """
        Static method.
        takes a string of values and encodes into sensor_values_t type
        """

        msg = sensor_vals_t()
        msg.timestamp = int(time.time())
        result = [x.strip() for x in line.split(',')]
        if len(result) > 2:
            msg.wheel_encoders.left = long(result[1])
            msg.wheel_encoders.right = long(result[2])
        return msg


def main():
    """
    Module test code
    """

    # Starting background thread
    _ = SerialHandler('/dev/ttyUSB0', 4800)
    print "starting loop"
    while True:
        pass



if  __name__ == '__main__':
    main()
