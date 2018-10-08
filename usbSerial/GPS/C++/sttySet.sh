#!/bin/bash

chmod 744 /dev/ttyUSB0;
stty -F /dev/ttyUSB0 ignbrk icrnl ixon 4800;
