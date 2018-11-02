#!/usr/bin/python
#
# This python program reads a GPIO digital input connected to a
# push button switch and prints out the number of times the
# button has been pushed.
#
# Circuit:
#   SPST momentary closed switch: pin 1 of switch connected
#   to VCC. Pin 2 connected to digital GPIO pin and to a 10K
#   Ohm resister.  Other end of the resister connected to GND.
#   Pushing the button sends a HIGH logic level to the
#   raspberry pi, otherwise the level is LOW.

# Define GPIO pin used by this example program. This definition
# is for the GPIO of a Raspberry Pi Model B.  If you are using
# a Raspberry Pi 3 Model B, then you may need to change the pin
# number to something else. 
PUSH_BUTTON_PIN = 24

# Import the GPIO library
import RPi.GPIO as GPIO
# Import the time library
import time

# Initialize the GPIO interface
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

# Setup as input the pin used for the push button
GPIO.setup(PUSH_BUTTON_PIN, GPIO.IN)

# Initialize a variable for counting button pushes.
count = 0

while True:
  # Read the logic level on the push button input pin
  inputValue = GPIO.input(PUSH_BUTTON_PIN)
  # Increment the counter when the button gets pushed
  if(inputValue == True):
    count += 1
    # Print the number of times the button has been pushed
    print("Button pressed: %d times." % count)
    # Delay small amount to avoid switch branch
    time.sleep(.3)
# Sleep a small amount of time to allow the cpu
# to do other processing
  time.sleep(.01)

# end program

