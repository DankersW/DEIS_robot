import serial

current_command = None
data = None
current_line = ""
port = serial.Serial('/dev/ttyUSB0', baudrate=9600, timeout=1)
while True:
    if(port.inWaiting()):
        s = port.read()
        if s != "\0": #end of msg
            print s
            current_line += s
        else:
            print "else"
            if not current_command:
                print "Read command", current_line
                current_command = current_line
            else:
                data = current_line
                "data read: ", current_line
