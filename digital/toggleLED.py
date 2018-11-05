#!/usr/bin/python
#
# This python program turns off or on a LED attached to a GPIO
# pin.
#
# Turn on the LED by typing at a command line prompt
#   ./toggleLED.py 1
# Turn off the LED by typing at the command line prompt
#   ./toggleLED.py 0
# 
# Circuit:
#   Pin 25 connected to a 220 Ohm resister.  Other end of
#   resistor connected to anode of LED.  Cathode of LED
#   connected to GND.

# Define GPIO pin used by this example program. This definition
# is for the GPIO of a Raspberry Pi Model B.  If you are using
# a Raspberry Pi 3 Model B, then you may need to change the pin
# number to something else. 
led_pin = 25

# import the GPIO library
import RPi.GPIO as GPIO
# import the sys library
import sys

# validate command line arguments
if len(sys.argv) == 2:
  ledState = sys.argv[1]
  if ledState != '0' and ledState != '1':
    print "Usage: toggleLED.py 0 | 1"
    ledState = '0'
    exit(1)
else:
  print "Usage: toggleLED.py 0 | 1"
  ledState = '0'
  exit(1)

# initialize the GPIO interface
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

# define the GPIO pin as an output
GPIO.setup(led_pin, GPIO.OUT)

# turn on or off the LED
if ledState == '1':
  GPIO.output(led_pin,GPIO.HIGH)
  print("LED state: ON")
else:
  GPIO.output(led_pin,GPIO.LOW)
  GPIO.cleanup()
  print("LED state: OFF")

exit(0)

# end program
