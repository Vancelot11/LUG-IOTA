#ifndef __HEADER__
#define __HEADER__

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wiringPi.h>
#include<sys/time.h>
#include<string.h>

#define SERIALPIN 7
#define GPIOEXPORTPATH "/sys/class/gpio/export"
#define GPIOUNEXPORTPATH "/sys/class/gpio/unexport"
#define GPIOPATH "/sys/class/gpio/gpio"

#endif
