#!/usr/bin/env python
# license removed for brevity
import rospy
from std_msgs.msg import String
from geometry_msgs.msg import Twist
from numpy import*
import serial


class driver:
    def __init__(self):
        # init ros
        rospy.init_node('car_driver', anonymous=True)
        rospy.Subscriber('/cmd_vel', Twist, self.get_cmd_vel)
        # self.pub_cmd_vel = rospy.Publisher('cmd_vel',Twist,queue_size=20)
        self.ser = serial.Serial('/dev/ttyUSB0', 115200)
        self.get_arduino_message()

    def get_cmd_vel(self, data):
        x = data.linear.x
        angular = data.angular.z
        self.send_cmd_to_arduino(x, angular)

    def send_cmd_to_arduino(self, x, angular):
        right = int((x + angular) * 50)
        left = int((x - angular) * 50)
        message = "{},{},{},{},{},{}*".format(right, left, right, left, right, left)
        print message
        self.send_arduino_message(message)

    def send_arduino_message(self, message):
        self.ser.write(message)

    def get_arduino_message(self):
        pub = rospy.Publisher('arduino', String, queue_size=10)
        r = rospy.Rate(10)
        while not rospy.is_shutdown():
            message = self.ser.readline()
            pub.publish(message)
            r.sleep()

if __name__ == '__main__':
    try:
        d = driver()
    except rospy.ROSInterruptException: 
        pass
