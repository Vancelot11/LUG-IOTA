import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(19, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.setup(26, GPIO.OUT)


try:
    while True:
        button_state = GPIO.input(19)
        if button_state == 1:
            GPIO.output(26,1)
            print('Button Pressed...')
            time.sleep(0.2)
        else:
            GPIO.output(26,0)

except:
    GPIO.cleanup()
