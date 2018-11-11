import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(13, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)

try:
    while True:
        button_state = GPIO.input(13)
        if button_state == 1:
            print('Button Pressed...')
            time.sleep(0.2)
except:
    GPIO.cleanup()
