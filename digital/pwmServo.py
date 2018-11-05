#!/usr/bin/python

# This interactive python program moves a servo using the GPIO
# pulse width modulation functions. The user may enter the
# following:
#    n an integer angle between 0 and 180 degrees
#   'c' for continuous oscillation mode
#   's' to exit the program

# Import GPIO library
import RPi.GPIO as GPIO
import time
# Import libraries to monitor for keyboard events
import threading
import click

# Define constants

SERVO_CONTROL_PIN = 23 # GPIO pin number
PWM_FREQUENCY = 100 # Hertz

def setDutyCycle(angle):
    # Set the duty cycle of the PWM signal to the servo
    dutyCycle = float(angle) / 10.0 + 3.0
    pwm.ChangeDutyCycle(dutyCycle)
#end def

def setServo(sCmd):
  # Move the servo to the supplied angle.
  if sCmd.isdigit() and int(sCmd) <= 180:
    angle = int(sCmd)
    setDutyCycle(angle)
  else:
    print "Must enter an integer between 0 and 180."
# end def

def continuousMotion():
  # Launch continuous movement mode.  The servo will move
  # back and forth from the 0 degree position to the 180
  # position.
  global keyPressed

  # Setup a thread to monitor for key pressed events.  When any
  # key gets pressed, update the global variable 'keyPressed'
  # with the value of the character pressed.
  keyPressed = 0
  threading.Thread(target=monitorKeyboard).start()

  while True:
    # Oscillate the servo back and forth.  Exit if the
    # user presses a key.
    for i in range(0, 180, 5):
      setDutyCycle(i)
      if keyPressed != 0: return
      time.sleep(0.1)
    for i in range(180, 0, -5):
      setDutyCycle(i)
      if keyPressed != 0: return
      time.sleep(0.1)
# end def

def monitorKeyboard():
  # Get key pressed by the user.  This function gets launched
  # as a thread to monitor for a key pressed event.
  global keyPressed
  keyPressed = click.getchar()
#end def

# Setup the GPIO interface
GPIO.setmode(GPIO.BCM)
GPIO.setup(SERVO_CONTROL_PIN, GPIO.OUT)
pwm = GPIO.PWM(SERVO_CONTROL_PIN, PWM_FREQUENCY)

# Start the PWM signal to the servo
pwm.start(3.0)


# Enter a command loop waiting for input from the user
print 'Enter a servo angle (0 to 180), c for continuous motion,' \
      '\nor s to exit.'
while True:
  sCmd = raw_input('>')
  if sCmd == 'c':
    print "Continuous motion mode. Press any key to stop."
    continuousMotion()
  elif sCmd == 's':
    break
  else:
    setServo(sCmd)

GPIO.cleanup()
exit(0)

