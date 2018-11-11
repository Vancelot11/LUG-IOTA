import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(19, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)

while True:
    input_state = GPIO.input(19)
    GPIO.wait_for_edge(19, GPIO.RISING)
    if input_state == False:                #This depends on pulldown/pullup resistor.
        print("It works!")
        time.sleep(0.2)
    
