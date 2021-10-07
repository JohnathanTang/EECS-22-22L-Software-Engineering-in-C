/* DIPs.h: header file for functions used for Assignment 2 (Edge, Black and White, Shuffle, and AutoTest) */

#ifndef DIPS_H
#define DIPS_H 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Constants.h"
#include"Image.h"

void PrintMenu(); 

IMAGE *Edge(IMAGE *image); 

IMAGE *BlackNWhite(IMAGE *image); 

IMAGE *Shuffle(IMAGE *image); 

IMAGE *Resize(IMAGE *image, int percentage); 

IMAGE *Saturate(IMAGE *image, float percent); 

IMAGE *Rotate(IMAGE *image, int option);

void Auto(void); 
#endif /* DIPs.h */

/* EOF DIPs.h */
