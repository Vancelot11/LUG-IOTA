#!/bin/bash
# This bash script reads a GPIO digital input connected to a
# push button switch and prints out 0 or 1 depending on whether
# button is pushed.
#
# Circuit:
#   SPST momentary closed switch: pin 1 of switch connected
#   to VCC. Pin 2 connected to digital GPIO pin and to a 10K
#   Ohm resister.  Other end of the resister connected to GND.
#   Pushing the button sends a HIGH logic level to the
#   raspberry pi, otherwise the level is LOW.
#
# Note:
#   This script must be run as superuser, i.e.,
#      sudo ./monitorPushButton.sh

# Define GPIO pin used by this example program. This definition
# is for the GPIO of a Raspberry Pi Model B.  If you are using
# a Raspberry Pi 3 Model B, then you may need to change the pin
# number to something else. 
push_button_pin=24

# setup the GPIO pin for input
cd /sys/class/gpio
if [ ! -d "gpio$push_button_pin" ]; then
  echo 24 > export
fi
cd gpio$push_button_pin
echo in > direction

previous_button_state=1
# continually check the pin for HIGH or LOW logic level
while true;
do
  # exit the script if any key is pressed
  read -rsn1 -t 0.001 key_pressed
  if [ ! "$key_pressed" == "" ]; then 
    cd ..
    echo " "
    echo $push_button_pin > unexport
    exit 0
  fi
  # get the logic level applied to the pin
  button_state=`cat value`
  # print out the level if it has changed
  if [ ! "$previous_button_state" == "$button_state" ]; then
    echo -e -n "\rbutton state: $button_state  "
    previous_button_state=$button_state
  fi
  sleep .1
done

# end script

