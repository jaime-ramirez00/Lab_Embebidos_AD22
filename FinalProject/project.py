import time 
import grovepi
from grovepi import *
import math
from grove_rgb_lcd import *

servo = 5
button = 4
touch_sensor = 8
sensor = 0

pinMode(button,"INPUT")
grovepi.pinMode(touch_sensor,"INPUT")
grovepi.pinMode(servo,"OUTPUT")
time.sleep(1)

grovepi.analogWrite(servo,90)

while True:
    button_status= digitalRead(button)  #Read the Button status
    if button_status:
        setRGB(0,0,0)
        setText(" ")
        time.sleep(1)#If the Button is in HIGH position, run the program
        aux = True
        while aux:
            button_status= digitalRead(button)
            time.sleep(0.5)
            if button_status:
                aux = False
    else: #If Button is in Off position, print "Off" on the screen
        setRGB(100,128,254)
        time.sleep(0.5)
        temp = grovepi.temp(sensor,'1.1')

        if grovepi.digitalRead(touch_sensor) == 1:
            time.sleep(0.5)
            setText("Temp = " + str((temp * 9 / 5) + 32) + " F")
        else:
            time.sleep(0.5)
            setText("Temp = " + str(temp) + " C")
        
        if temp > 22:
            grovepi.analogWrite(servo, 10)
        else:
            grovepi.analogWrite(servo, 245)
        
        time.sleep(0.5)
        
