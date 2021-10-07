/* FileIO.h: header file for input and outputting image files */

#ifndef FILEIO_H
#define FILEIO_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Constants.h"
#include "Image.h"

IMAGE *LoadImage(const char *fname);

int SaveImage(const char *fname, const IMAGE *image);
#endif /* FILEIO_H */ 

/* EOF FileIO.h */
