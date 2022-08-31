import time 
import grovepi
from grovepi import *
import math
from grove_rgb_lcd import *
button = 4
sensor = 0
pinMode(button,"INPUT")

while True:
    button_status= digitalRead(button)    #Read the Button status
    if button_status:   #If the Button is in HIGH position, run the program
        setRGB(200,205,50)
        temp = grovepi.temp(sensor,'1.1')
        setText("Temp = " + str(temp) + " C")
        time.sleep(0.5)
        #try:
        #    temp = grovepi.temp(sensor,'1.1')
        #    setText("Temp = " + temp + " C")
        #    setRGB(124,252,0)      

        #except KeyboardInterrupt:
        #    break
        #except IOError:
        #    print ("Error")
        
        # print "\tBuzzing"               
    else:         #If Button is in Off position, print "Off" on the screen
        setRGB(100,128,254)   
        setText("Presiona el boton\n")
        time.sleep(0.5)
        
        # print "Off"               
