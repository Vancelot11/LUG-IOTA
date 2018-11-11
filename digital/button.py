import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(13, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)

while True:
    input_state = GPIO.input(13)
    time.sleep(0.01)
    GPIO.wait_for_edge(13, GPIO.RISING)
    if input_state == False:
        print("It works!")
        time.sleep(0.2)

GPIO.cleanup()
