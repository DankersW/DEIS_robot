"""
text
"""
import serial


if  __name__ == '__main__':
    read = False
    buf = ""
    port = serial.Serial('/dev/serial0', baudrate=115200, timeout=1)
    while True:
        if True:
            if port.inWaiting():
                #Try to lock mutex
                #lock aquired. Read data
                line = port.readline()
                print line
                """
                byte = port.read(1)
                #print byte
                char = str(byte)
                print char
                if char == '\n':
                    print buf
                    buf = ""
                    read = False
                    break
                buf += char
                #self.decode_line()
                #print line
                """
        else:
            print "Printing line"
            port.write("GPS,150,150\n")
            read = True
