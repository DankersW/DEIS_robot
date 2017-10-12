import math


def Angdiff(A1,A2):
    return (A1 - A2) + ((A1 - A2) < - math.pi) * (2 * math.pi) + ((A1 - A2) > math.pi) * (-2 * math.pi)


def Update_Pose(pose,d_EncoderR,d_EncoderL,wheel_dia,EResol,wheel_base):

    Dr = math.pi * wheel_dia * (d_EncoderR / EResol)
    Dl = math.pi * wheel_dia * (d_EncoderL / EResol)
    Dc = (Dr + Dl) / 2
    theta_change = (Dr - Dl) / wheel_base

    pose.x = pose.x + Dc * math.cos(pose.theta + (theta_change / 2))
    pose.y = pose.y + Dc * math.sin(pose.theta + (theta_change / 2))
    pose.theta = pose.theta + theta_change


def Controller_Output(pose,waypoint,control_param,wheel_base):


    k1 =control_param[0]
    k2 =control_param[1]
    vel_Max = control_param[2]
    stop_Dist = control_param[3]

    dX = waypoint.x - pose.x
    dY = waypoint.y - pose.y
    r = math.sqrt(pow(dX, 2) + pow(dY, 2))

    if (r < stop_Dist):
        return 0, 0

    ego_cosys_ang = math.atan2(dY, dX)
    ego_delta = Angdiff(pose.theta, ego_cosys_ang)
    ego_theta = Angdiff(waypoint.theta, ego_cosys_ang)

    kappa = -1 / r * (
        k2 * (ego_delta - math.atan(-k1 * ego_theta)) + (1 + k1 /
                                                         (1 + (k1 * pow(ego_theta, 2)))) * math.sin(ego_delta))
    vel_lin = vel_Max / (1 + 10 * abs(pow(kappa, 2)))
    omega = vel_lin * kappa;

    vel_l = vel_lin - omega * wheel_base / 2
    vel_r = 2 * vel_lin - vel_l

    return vel_l, vel_r





class Pose:

    x = 0
    y = 0
    theta = 0

    def __init__(self,x_init,y_init,theta_init):
        self.x = x_init
        self.y = y_init
        self.theta = theta_init


    def update_Pose(self,x_new,y_new,theta_new):
        self.x = x_new
        self.y = y_new
        self.theta = theta_new





