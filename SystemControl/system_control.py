from serial_handler import SerialHandler
from Controller import MotionController
def main():
    _ = SerialHandler('/dev/ttyUSB0', 4800)
    motion_controller = MotionController(0,0,0)
    gps_class         = gps_data()

if __name__ == '__main__':
    main()