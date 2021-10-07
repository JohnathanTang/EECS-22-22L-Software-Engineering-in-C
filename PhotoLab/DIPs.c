//DIPs.c 

#include "DIPs.h"
#include "Constants.h"
#include "FileIO.h"
#include "Advanced.h"
#include "Image.h"

void PrintMenu() {
    printf("\n----------------------------\n");
    printf(" 1:  Load a PPM image\n");
    printf(" 2:  Save an image in PPM and JPEG format\n");
    printf(" 3:  Change a color image to Black & White\n");
    printf(" 4:  Sketch the edge of an image\n");
    printf(" 5:  Shuffle an image\n");
    printf(" 6:  Adjust brightness of an image\n");
    printf(" 7:  Reflect image horizontally\n");
    printf(" 8:  Hue Rotate image\n");
    printf(" 9:  Resize image\n");  
    printf("10:  Saturate image\n");
    printf("11:  Rotate image\n");   
    printf("12:  Test all functions\n");
    printf("13:  Exit\n");
}

/* reverse image color */
IMAGE *BlackNWhite(IMAGE *image)
{   
 	int             x, y, tmp;
	int HEIGHT = ImageHeight(image); 
	int WIDTH = ImageWidth(image); 
    for (y = 0; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
        {
            tmp = (GetPixelR(image,x,y) + GetPixelG(image,x,y) + GetPixelB(image,x,y)) / 3; 
            SetPixelR(image,x,y,tmp); 
	    SetPixelG(image,x,y,tmp);
	    SetPixelB(image,x,y,tmp);  
        }
    }
return image;
}

/* edge detection */
IMAGE *Edge(IMAGE *image)
{   
    int             x, y, m, n, a, b;
    int HEIGHT = ImageHeight(image); 
    int WIDTH = ImageWidth(image); 
    unsigned char   tmpR[WIDTH][HEIGHT];
    unsigned char   tmpG[WIDTH][HEIGHT];
    unsigned char   tmpB[WIDTH][HEIGHT];
    for (y = 0; y < HEIGHT; y++){
        for (x = 0; x < WIDTH; x++) {
            tmpR[x][y] = GetPixelR(image,x,y); 
            tmpG[x][y] = GetPixelG(image,x,y); 
            tmpB[x][y] = GetPixelB(image,x,y); 
        }
    }
    int sumR = 0;   /* sum of the intensity differences with neighbors */
    int sumG = 0;
    int sumB = 0;
    for (y = 1; y < HEIGHT - 1; y++){
        for (x = 1; x < WIDTH - 1; x++){
            for (n = -1; n <= 1; n++){
                for (m = -1; m <= 1; m++) {
                    a = (x + m >= WIDTH) ? WIDTH - 1 : (x + m < 0) ? 0 : x + m;
                    b = (y + n >= HEIGHT) ? HEIGHT - 1 : (y + n < 0) ? 0 : y + n;
                    sumR += (tmpR[x][y] - tmpR[a][b]);
                    sumG += (tmpG[x][y] - tmpG[a][b]);
                    sumB += (tmpB[x][y] - tmpB[a][b]);

                }
            }
            SetPixelR(image,x,y,(sumR > MAX_PIXEL) ? MAX_PIXEL: (sumR < 0) ? 0: sumR);
            SetPixelG(image,x,y,(sumG > MAX_PIXEL) ? MAX_PIXEL: (sumG < 0) ? 0: sumG);
            SetPixelB(image,x,y,(sumB > MAX_PIXEL) ? MAX_PIXEL: (sumB < 0) ? 0: sumB);
            sumR = sumG = sumB = 0;
        }
    }
    /* set all four borders to 0 */
    for (y = 0; y < HEIGHT; y++) {
        SetPixelR(image,0,y,0); 
        SetPixelG(image,0,y,0); 
        SetPixelB(image,0,y,0); 
        SetPixelR(image,WIDTH-1,y,0);
        SetPixelG(image,WIDTH-1,y,0); 
        SetPixelB(image,WIDTH-1,y,0); 
    }
    for (x = 0; x < WIDTH; x++) {
	SetPixelR(image,x,0,0); 
	SetPixelG(image,x,0,0); 
	SetPixelB(image,x,0,0); 
	SetPixelR(image,x,HEIGHT-1,0); 
	SetPixelG(image,x,HEIGHT-1,0); 
	SetPixelB(image,x,HEIGHT-1,0);
    }
return image;
}

/* Shuffle the image */
IMAGE *Shuffle(IMAGE *image)
{
    int HEIGHT = ImageHeight(image); 
    int WIDTH = ImageWidth(image); 
    int block_cnt = SHUFF_HEIGHT_DIV * SHUFF_WIDTH_DIV;
    int block_width = WIDTH/SHUFF_WIDTH_DIV;
    int block_height = HEIGHT/SHUFF_HEIGHT_DIV;
    int block[SHUFF_WIDTH_DIV][SHUFF_HEIGHT_DIV];
    int i, j;

    srand(time(NULL));
    /* Initialize block markers to not done (-1) */
    for (i = 0; i < SHUFF_WIDTH_DIV; i++) {
        for (j = 0; j< SHUFF_HEIGHT_DIV; j++) {
            block[i][j] = -1;
        }
    }

    while (block_cnt > 0) {
        /* Generate a random pair of blocks */
        int dest_height = rand() % SHUFF_HEIGHT_DIV;
        int dest_width = rand() % SHUFF_WIDTH_DIV;
        int src_height = rand() % SHUFF_HEIGHT_DIV;
        int src_width = rand() % SHUFF_WIDTH_DIV;

        /* Check if these blocks are already swaped, if not swap blocks */
        if ((block[dest_width][dest_height] == -1) && (block[src_width][src_height] == -1)) {
            /* Swap blocks */
            for (i = 0; i < block_height; i++) {
                /* Init src and dest height offset */
                int h_dest = ((dest_height * block_height) + i) % HEIGHT;
                int h_src  = ((src_height * block_height) + i) % HEIGHT;
                for (j = 0; j < block_width; j++) {
                    int temp;
                    /* Init src and dest width offset */
                    int w_src  = ((src_width * block_width) + j) % WIDTH;
                    int w_dest = ((dest_width * block_width) + j) % WIDTH;

                    temp = GetPixelR(image,w_dest,h_dest); 
                    SetPixelR(image,w_dest,h_dest, GetPixelR(image,w_src,h_src)); 
                    SetPixelR(image,w_src,h_src,temp); 

                    temp = GetPixelG(image,w_dest,h_dest); 
                    SetPixelG(image,w_dest,h_dest,GetPixelG(image,w_src,h_src)); 
                    SetPixelG(image,w_src,h_src,temp); 

                    temp = GetPixelB(image,w_dest,h_dest); 
                    SetPixelB(image,w_dest,h_dest,GetPixelB(image,w_src,h_src)); 
                    SetPixelB(image,w_src,h_src,temp); 
                }
            }
            /* Set marker as done */
            block[dest_width][dest_height] = 1;
            block[src_width][src_height] = 1;
            /* Decrease block count */
            block_cnt -= 2; /* Two blocks are swapped */
        }

    }

return image;
}
/* Resize the image based on percentage input*/ 
IMAGE *Resize(IMAGE *image, int percentage)
{
	//Declare width and height and set NewWidth and NewHeight of new image
	int WIDTH = ImageWidth(image); 	
	int HEIGHT = ImageHeight(image); 
	int NEWWIDTH = WIDTH * (percentage / 100.00); 
	int NEWHEIGHT = HEIGHT * (percentage / 100.00); 
	//Create new image 
	IMAGE *NewImage = CreateImage(NEWWIDTH,NEWHEIGHT);
	//If percentage is 100, return the original image
	if (percentage == 100)
	{
		return image; 
	}	
	else
	{
		//For every pixel of the original image, scale each pixel before setting them to the new image
		for (int i = 0; i < NEWWIDTH; i++)
		{
			for (int j = 0; j < NEWHEIGHT; j++)
			{
				SetPixelR(NewImage, i, j, GetPixelR(image,i*(100.00/percentage),j*(100.00/percentage)));
				SetPixelG(NewImage, i, j, GetPixelG(image,i*(100.00/percentage),j*(100.00/percentage))); 
				SetPixelB(NewImage, i, j, GetPixelB(image,i*(100.00/percentage),j*(100.00/percentage)));  
			
			}
		}
	}
	//Delete "image" before returning NewImage, we want # of allocs = # of free        
	DeleteImage(image);
	return NewImage;	
}
//Saturate color of image
IMAGE *Saturate(IMAGE *image, float percent)
{
	int WIDTH = ImageWidth(image); 
	int HEIGHT = ImageHeight(image); 
	for (int i = 0; i < WIDTH; i++) 
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			//Average each RGB pixel 
			int Rt = (GetPixelR(image,i,j) + GetPixelG(image,i,j)+GetPixelB(image,i,j))/3;
			int Gt = Rt; 
			int Bt = Rt;
			
			//Get difference from original pixel from distance from grayscale
			Rt = GetPixelR(image,i,j) - Rt;
			Gt = GetPixelG(image,i,j) - Gt;
			Bt = GetPixelB(image,i,j) - Bt;

			//Add temp RGB to the original RGB pixels while keeping restrictions between [0,255]
			if((GetPixelR(image,i,j) + (Rt * percent/100.00)) > 255)
			{
				SetPixelR(image,i,j,255); 
			}
			else if((GetPixelR(image,i,j) + (Rt * percent/100.00)) < 0)
			{
				SetPixelR(image,i,j,0); 
			}
			else 
			{
				SetPixelR(image,i,j,GetPixelR(image,i,j) + (Rt * percent/100.00)); 
			}
			if((GetPixelG(image,i,j) + (Gt * percent/100.00)) > 255)
			{
				SetPixelG(image,i,j,255);
			}
			else if ((GetPixelG(image,i,j) + (Gt * percent/100.00)) < 0)
			{
				SetPixelG(image,i,j,0); 
			}
			else
			{
				SetPixelG(image,i,j,GetPixelG(image,i,j) + (Gt * percent/100.00));
			}
			if ((GetPixelB(image,i,j) + (Bt * percent/100.00)) > 255)
			{ 
				SetPixelB(image,i,j,255);
			}
			else if ((GetPixelB(image,i,j) + (Bt * percent/100.00)) < 0)
			{
				SetPixelB(image,i,j,0); 
			}
			else 
			{
				SetPixelB(image,i,j,GetPixelB(image,i,j) + (Bt * percent/100.00));
			}

		}
	}
	return image; 
		
}

//Rotate image based on option 
IMAGE *Rotate(IMAGE *image, int option)
{
	//Horizontal flip
	if (option == 1)
	{
		int Rt,Gt,Bt;   
		int HEIGHT = ImageHeight(image); 
		int WIDTH = ImageWidth(image);
		for (int i = 0; i < WIDTH/2; i++)
		{
			for (int j = 0; j < HEIGHT; j++)
			{
				//Set temp to right side of the image in reverse order
				Rt = GetPixelR(image,WIDTH - 1 - i,j); 
				Gt = GetPixelG(image,WIDTH - 1 - i,j); 
				Bt = GetPixelB(image,WIDTH - 1 - i,j); 
				
				//Set right side of image to left side of image in reverse order
				SetPixelR(image,WIDTH - 1 - i,j,GetPixelR(image,i,j)); 
				SetPixelG(image,WIDTH - 1 - i,j,GetPixelG(image,i,j)); 
				SetPixelB(image,WIDTH - 1 - i,j,GetPixelB(image,i,j)); 

				//Set left side of image to temp (right side of image) 
				SetPixelR(image,i,j,Rt); 
				SetPixelG(image,i,j,Gt); 
				SetPixelB(image,i,j,Bt); 
			}
		}
	return image;	
	}
      	else if (option == 2)
	{
		int Rt, Gt,Bt;   
		int HEIGHT = ImageHeight(image); 
		int WIDTH = ImageWidth(image);
		for (int i = 0; i < WIDTH; i++)
		{
			for (int j = 0; j < HEIGHT/2; j++)
			{
				//Set temp to bottom half of image in reverse order
				Rt = GetPixelR(image,i,HEIGHT - 1 - j); 
				Gt = GetPixelG(image,i,HEIGHT - 1 - j); 
				Bt = GetPixelB(image,i,HEIGHT - 1 - j); 
				
				//Set bottom half of image to top half in reverse order
				SetPixelR(image,i,HEIGHT - 1 - j,GetPixelR(image,i,j)); 
				SetPixelG(image,i,HEIGHT - 1 - j,GetPixelG(image,i,j)); 
				SetPixelB(image,i,HEIGHT - 1 - j,GetPixelB(image,i,j)); 
				
				//Set temp to top half of image 
				SetPixelR(image,i,j,Rt); 
				SetPixelG(image,i,j,Gt); 
				SetPixelB(image,i,j,Bt); 
			}
		}
		return image;		
	}
	//Rotate clockwise
	else if (option == 3)
	{
		int HEIGHT = ImageHeight(image); 
		int WIDTH = ImageWidth(image); 
		int NEWHEIGHT = WIDTH;
		int NEWWIDTH = HEIGHT; 
	        IMAGE *RotatedImage = CreateImage(NEWWIDTH,NEWHEIGHT);
		for (int i = 0; i < WIDTH; i++)
		{
			for (int j = 0; j < HEIGHT; j++)
			{
				//Set pixels from original image to new image
				//Read rows of original image and set as columns of new image 
				SetPixelR(RotatedImage,NEWWIDTH - j-1 ,i,GetPixelR(image,i,j));
				SetPixelG(RotatedImage,NEWWIDTH - j-1 ,i,GetPixelG(image,i,j));
				SetPixelB(RotatedImage,NEWWIDTH - j-1 ,i,GetPixelB(image,i,j));   
			}
		}
		DeleteImage(image); 
		return RotatedImage; 	
	}
	//Rotate counterclockwise
	else 
	{
		int HEIGHT = ImageHeight(image); 
		int WIDTH = ImageWidth(image); 
		int NEWHEIGHT = WIDTH;
		int NEWWIDTH = HEIGHT; 
	        IMAGE *RotatedImage = CreateImage(NEWWIDTH,NEWHEIGHT);
		for (int i = 0; i < WIDTH; i++)
		{
			for (int j = 0; j < HEIGHT; j++)
			{
				//Set columns of original image to rows of new image 
				//Set rows of original image to columns of new image
				SetPixelR(RotatedImage,j,i,GetPixelR(image,i,j));
				SetPixelG(RotatedImage,j,i,GetPixelG(image,i,j));
				SetPixelB(RotatedImage,j,i,GetPixelB(image,i,j));   
			}
		}
		DeleteImage(image); 
		return RotatedImage; 	
	}

}

/* Test all functions */
/*void Auto(void)
{
    IMAGE *image; 
    char fname[SLEN] = "HSSOE";
    char sname[SLEN];
    
 
    image = LoadImage(fname); 
    BlackNWhite(image);
    strcpy(sname, "bw");
    SaveImage(sname,image); 
    printf("Black & White tested!\n\n");
    DeleteImage(image);

    image = LoadImage(fname);
    Edge(image);
    strcpy(sname, "edge");
    SaveImage(sname, image);
    printf("Edge Detection tested!\n\n");
    DeleteImage(image); 

    image = LoadImage(fname);
    Shuffle(image);
    strcpy(sname, "shuffle");
    SaveImage(sname, image);
    printf("Shuffle tested!\n\n");
    DeleteImage(image);

    image = LoadImage(fname); 
    Brightness(image, 200); 
    strcpy(sname, "brightness"); 
    SaveImage(sname, image); 
    printf("Brightness tested!\n\n");   
    DeleteImage(image);

    image = LoadImage(fname);  
    HMirror(image);  
    strcpy(sname, "hmirror"); 
    SaveImage(sname,image); 
    printf("HMirror tested!\n\n"); 
    DeleteImage(image);   

    image = LoadImage(fname);  
    HueRotate(image, -1.5);  
    strcpy(sname, "hue"); 
    SaveImage(sname, image); 
    printf("Hue Rotate tested!\n\n");
    DeleteImage(image); 
	
    image = LoadImage(fname); 
    Resize(image,170); 
    strcpy(sname,"bigresize");
    SaveImage(sname,image);  
    printf("Big Resize tested!\n\n");
    DeleteImage(image); 

    image = LoadImage(fname); 
    Resize(image,60); 
    strcpy(sname,"smallresize"); 
    SaveImage(sname,image); 
    printf("Small Resize tested!\n\n"); 
    DeleteImage(image); 
	
    image = LoadImage(fname); 
    Saturate(image,90); 
    strcpy(sname,"saturate"); 
    SaveImage(sname,image); 
    printf("Saturate tested!\n\n"); 
    DeleteImage(image);

    image = LoadImage(fname);
    Rotate(image,4);  
    strcpy(sname,"rotate"); 
    SaveImage(sname,image); 
    printf("Rotate tested!\n\n"); 
    DeleteImage(image);   
}*/

/* EOF */
