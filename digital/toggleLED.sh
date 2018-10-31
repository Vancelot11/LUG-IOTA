#!/bin/bash
#
# This bash script turns off or on a LED attached to a GPIO
# pin.
#
# Turn on the LED by typing at a command line prompt
#   ./toggleLED.sh 1
# Turn off the LED by typing at the command line prompt
#   ./toggleLED.sh 0
# 
# Circuit:
#   Pin 25 connected to a 220 Ohm resister.  Other end of
#   resistor connected to anode of LED.  Cathode of LED
#   connected to GND.
#
# Note:
#   You will need to run this script as superuser, i.e.,
#     sudo ./toggleLED.sh 1

# Define GPIO pin used by this example program. This definition
# is for the GPIO of a Raspberry Pi Model B.  If you are using
# a Raspberry Pi 3 Model B, then you may need to change the pin
# number to something else. 
led_pin=25

# validate command line arguments
if [ "$#" -eq "0" ] || ([ "$1" -ne "0" ] && [ "$1" -ne "1" ]); then
  echo "Usage: toggleLED.sh 0 | 1"
  exit 1
fi

# initialize the GPIO interface
if [ ! -d "/sys/class/gpio/gpio$led_pin" ]; then
  echo 25 > /sys/class/gpio/export
fi
echo out > /sys/class/gpio/gpio$led_pin/direction

# turn off or on the LED
echo $1 > /sys/class/gpio/gpio$led_pin/value

# end script

