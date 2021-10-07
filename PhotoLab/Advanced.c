#include "Advanced.h"
#include "Image.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

/* Adjust the brightness of an image */
IMAGE *Brightness(IMAGE *image, int brightness)
{
	int WIDTH = ImageWidth(image); 
	int HEIGHT = ImageHeight(image); 
	for (int i=0; i<WIDTH; i++){
		for (int j=0; j<HEIGHT; j++){
			if (GetPixelR(image,i,j) + brightness > 255){
				SetPixelR(image,i,j,255);
			}else if(GetPixelR(image,i,j) + brightness < 0){
				SetPixelR(image,i,j,0);
			}else{
				SetPixelR(image,i,j,(GetPixelR(image,i,j)+brightness));
			}

			if (GetPixelG(image,i,j) + brightness > 255){
				SetPixelG(image,i,j,255);
			}else if(GetPixelG(image,i,j) + brightness < 0){
				SetPixelG(image,i,j,0); 
			}else{
				SetPixelG(image,i,j,(GetPixelG(image,i,j)+brightness));
			}

			if (GetPixelB(image,i,j) + brightness > 255){
				SetPixelB(image,i,j,255);
			}else if(GetPixelB(image,i,j) + brightness < 0){
				SetPixelB(image,i,j,0); 
			}else{
				SetPixelB(image,i,j,(GetPixelB(image,i,j)+brightness));
			}
		}
	}
return image;
}

/* Mirror an image horizontally */
IMAGE *HMirror(IMAGE *image)
{
	int x, y;
	int HEIGHT = ImageHeight(image);
	int WIDTH = ImageWidth(image); 
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH / 2; x++) {
					SetPixelR(image,WIDTH-1-x,y,GetPixelR(image,x,y)); 
					SetPixelG(image,WIDTH-1-x,y,GetPixelG(image,x,y));
					SetPixelB(image,WIDTH-1-x,y,GetPixelB(image,x,y)); 
		}
	}
return image;
}

void matrix_3_3multiplyVector_3(float vector[3], const float matrix[3][3])
{
	/* Memory for result */
	float result[3] = {0, 0, 0};

	/* Perform multiplication */
	result[0] = matrix[0][0]*vector[0] + matrix[0][1]*vector[1] + matrix[0][2]*vector[2];
	result[1] = matrix[1][0]*vector[0] + matrix[1][1]*vector[1] + matrix[1][2]*vector[2];
	result[2] = matrix[2][0]*vector[0] + matrix[2][1]*vector[1] + matrix[2][2]*vector[2];

	/* Copy result into other stack's memory */
	vector[0] = result[0];
	vector[1] = result[1];
	vector[2] = result[2];
}

IMAGE *HueRotate(IMAGE *image, float angle)
{
	int HEIGHT = ImageHeight(image); 
	int WIDTH = ImageWidth(image); 
	/* Matrix to transform normalized
	// RGB color vector from the RGB
	// color space to the YIQ color space */
	static const float RGBtoYIQMatrix[3][3] = {{0.299,  0.587,  0.114},
								  			   {0.596, -0.274, -0.321},
								  			   {0.211, -0.523,  0.311}};

	/* Matrix to transform normalized
	// YIQ color vector from the YIQ
	// color space to the RGB color space */
	static const float YIQtoRGBMatrix[3][3] = {{1.000,  0.956,  0.621},
								  			   {1.000, -0.272, -0.647},
								  			   {1.000, -1.107,  1.705}};

	const float cs = cos(angle), ss = sin(angle);

	/* Matrix to perform a hue
	// rotation on a normalized
	// YIQ color vector*/
	const float XYPlaneVectorRotateMatrix[3][3] = {{1.000, 0.000, 0.000},
												   {0.000, cs, -ss},
												   {0.000, ss, cs}};

	/* Image variables */
	float temp[3];

	for(unsigned int i = 0; i < WIDTH; i++)
	{
		for(unsigned int j = 0; j < HEIGHT; j++)
		{
			/* Create a normalized RGB color
			// vector fro the current pixel
			// in the image */
			temp[0] = (float)GetPixelR(image,i,j)/255;
			temp[1] = (float)GetPixelG(image,i,j)/255;
			temp[2] = (float)GetPixelB(image,i,j)/255;

			/* Transform the RGB vector
			// to a YIQ vector */
			matrix_3_3multiplyVector_3(temp, RGBtoYIQMatrix);

			/* Perform the DIP */
			matrix_3_3multiplyVector_3(temp, XYPlaneVectorRotateMatrix);

			/* Transform back to RGB */
			matrix_3_3multiplyVector_3(temp, YIQtoRGBMatrix);

			/* Denormalize and store back into the image */
			temp[0] = temp[0] * 255;
			temp[0] = temp[0] < 0 ? 0 : temp[0];
			temp[0] = temp[0] > 255 ? 255 : temp[0];
			temp[1] = temp[1] * 255;
			temp[1] = temp[1] < 0 ? 0 : temp[1];
			temp[1] = temp[1] > 255 ? 255 : temp[1];
			temp[2] = temp[2] * 255;
			temp[2] = temp[2] < 0 ? 0 : temp[2];
			temp[2] = temp[2] > 255 ? 255 : temp[2];

			SetPixelR(image,i,j,(unsigned char)temp[0]);
			SetPixelG(image,i,j,(unsigned char)temp[1]);
			SetPixelB(image,i,j,(unsigned char)temp[2]);
		}
	}
return image;
}

/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
