import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(13, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.setup(24, GPIO.OUT)


try:
    while True:
        button_state = GPIO.input(13)
        if button_state == 1:
            GPIO.output(24,1)
            print('Button Pressed...')
            time.sleep(0.2)
        else:
            GPIO.output(24,0)

except:
    GPIO.cleanup()
