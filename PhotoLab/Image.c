#include "Image.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//Define GetPixel for RGB 
unsigned char GetPixelR(const IMAGE *image, unsigned int x,  unsigned int y)
{
	return image -> R[ x + y * ImageWidth(image)]; 
}

unsigned char GetPixelG(const IMAGE *image, unsigned int x,  unsigned int y)
{
    	return image -> G[ x + y * ImageWidth(image)]; 
}

unsigned char GetPixelB(const IMAGE *image, unsigned int x,  unsigned int y)
{
    	return image -> B[ x + y * ImageWidth(image)]; 
}

//Define SetPixel for RGB 
void SetPixelR(IMAGE *image, unsigned int x,  unsigned int y, unsigned char r)
{      
    	image -> R[ x + y * ImageWidth(image)] = r;
}

void SetPixelG(IMAGE *image, unsigned int x,  unsigned int y, unsigned char g)
{    
    	image -> G[ x + y * ImageWidth(image)] = g;
}

void SetPixelB(IMAGE *image, unsigned int x,  unsigned int y, unsigned char b)
{    
	image -> B[ x + y * ImageWidth(image)] = b;
}

IMAGE *CreateImage(unsigned int Width, unsigned int Height)
{
	//Manually allocate memory for image
	IMAGE *image = malloc(sizeof(IMAGE));
	//Check if image is not empty or NULL
	assert(image!=NULL); 
      	image -> W = Width;
      	image -> H = Height;
      	image -> R =  malloc(Width * Height);
     	image -> G =  malloc(Width * Height);
      	image -> B =  malloc(Width * Height);
	return image;
}

void DeleteImage(IMAGE *image)
{
	//Check if image exists before freeing memory of RGB and the image itself
	assert(image);     
        free(image -> R);
        free(image -> G);
        free(image -> B);
	free(image); 
}

unsigned int ImageWidth(const IMAGE *image)
{   
	return image->W;
}

unsigned int ImageHeight(const IMAGE *image)
{
	return image->H;
}
