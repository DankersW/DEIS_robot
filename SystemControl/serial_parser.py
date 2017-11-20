"""
Parse serial commands module.
Contains all classes related to serial parsing.
"""
class CommandFactory(object): # pylint: disable=too-few-public-methods
    """
    CommandFactory
    """
    @staticmethod
    def create(line):
        """
        Creates a command from input. Will have a NoneType command if invalid
        Splits line and strips of special chars.
        Returns the command encapsulated as a class.
        """
        result = [x.strip() for x in line.split(',')]

        choices = { \
            'ODM': Odometry(result), \
            'US': Ultrasonic(result),\
        }
        result = choices.get(result[0], None)
        return result

class Odometry(object):
    """
    Encapsulation of Odometry command
    """
    def __init__(self, results):
        if len(results) < 4:
            raise OverflowError('Not enough arguments in line for odometry', \
                ' command')
        self.pos_x = results[1]
        self.pos_y = results[2]
        self.theta = results[3]

    def execute(self, target):
        """
        Executes command on target using inversion of control.
        target.odometry(x, y, theta)
        """
        target.odometry(self.pos_x, self.pos_y, self.theta)
        #target.pos_x = self.pos_x
        #target.pos_y = self.pos_y
        #target.theta = self.theta

    def get(self):
        """
        Alternative option to get values
        """
        return (self.pos_x, self.pos_y, self.theta)

    def __str__(self):
        """
        String method. Used for debugging.
        """
        print "x: ", self.pos_x, " y: ", self.pos_y, " theta: ", self.theta

class Ultrasonic(object):
    """
    ultrasonic command
    """
    def __init__(self, result):
        """
        constructor
        """
        self.distance = result[1]

    def execute(self, target):
        """
        IOC. target needs .ultrasonic(distance) method
        """
        target.ultrasonic(self.distance)

    def get(self):
        """
        alternative way of accessing value
        """
        return self.distance

    def __str__(self):
        return "Ultrasonic distance: ", str(self.distance)

class Debug(object):
    """
    Debug command. Will print on terminal
    """
    def __init__(self, result):
        """
        Constructor
        """
        self.arg = result[1]

    def execute(self, target=None):
        """
        IOC. target needs .ultrasonic(distance) method
        """
        if target is None:
            print self
        else:
            target.debug(self.arg)

    def get(self):
        """
        alternative way of accessing value
        """
        return self.arg

    def __str__(self):
        return "Debug: ", str(self.arg)
