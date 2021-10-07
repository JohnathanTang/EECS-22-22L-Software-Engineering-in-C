//Advaned.h: Header file for Advanced.c, declare function prototype for Brightness, HMirror, and HueRotate
#ifndef ADVANCED_H
#define ADVANCED_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Constants.h"
#include "Image.h"

IMAGE *Brightness(IMAGE *image, int brightness); 

IMAGE *HMirror(IMAGE *image);

IMAGE *HueRotate(IMAGE *image, float angle); 
#endif 

/* EOF */ 
