#!/usr/bin/python -u
#
# This python program reads a GPIO digital input connected to a
# push button switch and prints out the state of the button:
#    ON if pushed
#    OFF if not pushed
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
# Import libraries to monitor for keyboard events
import threading
import click
import Queue

keyPressed = 0

# Setup a thread to monitor for keyboard events.  Whenever a
# key gets pressed, update the global variable 'key' with the
# value of the character pressed.

# Define the target function for the thread.
def monitorKeyboard():
  global keyPressed
  while(True):
    # Gracefully exit the thread when requested by the parent.
    try:
      if objQueue.get(False) == 'stop':
        break
    except Queue.Empty:
      pass
    # Wait for a key to be pressed.  This a blocking function.
    keyPressed = click.getchar()
    time.sleep(.01) # give the CPU some time to catch up
#end def

objQueue = Queue.Queue()

objThread = threading.Thread(target=monitorKeyboard)
objThread.start()

# Initialize the GPIO interface
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

# Setup as input the pin used for the push button
GPIO.setup(PUSH_BUTTON_PIN, GPIO.IN)

# The following variable is used to determine when the push
# button state has changed.
inputValue_prev = 1

# Tell the user the program has started and how to exit it.
print "Monitoring button state - press any key to exit..."

while True:
  # Read the logic level on the push button input pin
  inputValue = GPIO.input(PUSH_BUTTON_PIN)

  # Only print the state of the button if it has changed
  if inputValue != inputValue_prev:
    # Print the state of the button - either ON or OFF
    if(inputValue == True):
      print("\rButton state: ON  "),
    else:
      print("\rButton state: OFF "),
    inputValue_prev = inputValue
    # Delay small amount to avoid switch branch
    time.sleep(.1)

  # Sleep a small amount of time to allow the cpu
  # to do other processing
  time.sleep(.01)

  # If any key has been pressed, end the program
  if keyPressed != 0:
    break
#end while

# User has pressed a key, so gracefully exit the program.
objQueue.put('stop')
objThread.join()
exit()

# end program
