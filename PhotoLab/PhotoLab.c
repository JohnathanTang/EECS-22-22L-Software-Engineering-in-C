/*********************************************************************/
/* PhotoLab.c: assignment 2 for EECS 22, Fall 2018                 */
/*                                                                   */
/* modifications: Mihnea Chirila                            */
/* 09/27/17 RD	adjusted for lecture usage                           */
/* 10/1/18 MC	 adjusted for F18                           */
/*********************************************************************/
/* Enter your name, last name and UCInet ID below:
 *
 * *****************************
 * First Name :Johnathan 
 * Last Name:Tang
 * UCInet ID:63061294
 *
 */
#include <assert.h>

#include "FileIO.h"
#include "Constants.h"
#include "DIPs.h"
#include "Advanced.h"
#include "Image.h"
#include "Test.h"

int main()
{
    #ifdef DEBUG
    AutoTest(); 
    #else
    int option;			/* user input option */
    char fname[SLEN];		/* input file name */
    int bright;
    int resize; 
    float hue;  
    float saturate;    
    int rotate;

    IMAGE *image;
    PrintMenu();
    printf("Please make your choice: ");
    scanf("%d", &option);
    int rc = -1;			/* return code of LoadImage() */
    while (option != EXIT) {
        if (option == 1) {
            printf("Please input the file name to load: ");
            scanf("%s", fname);
            image = LoadImage(fname); 
	    rc = SUCCESS;	
        }
        /* menu item 2 - 10 requires image is loaded first */
        else if (option >= 2 && option <= 11) {
            if (rc != SUCCESS)	 {
                printf("No image is read.\n");
            }
            /* now image is loaded */
            else {
                switch(option) {
                    case 2:
                        printf("Please input the file name to save: ");
                        scanf("%s", fname);
                        SaveImage(fname,image);
			DeleteImage(image); 
                        break;
                    case 3:
                        image = BlackNWhite(image); 
                        printf("\"Black & White\" operation is done!\n");
                        break;
                    case 4:
                        image = Edge(image);
                        printf("\"Edge\" operation is done!\n");
                        break;
                    case 5:
                        image = Shuffle(image);
                        printf("\"Shuffle\" operation is done!\n");
                        break;
		    case 6:
			printf("Enter brightness adjustment: "); 
			scanf("%d", &bright);
			image = Brightness(image, bright);  	
			printf("\"Brightness\" operation is done!\n"); 
			break;
		    case 7: 
			image = HMirror(image); 
			printf("\"HMirror\" operation is done!\n"); 
			break;
		    case 8: 
			printf("Enter angle: "); 
			scanf("%f", &hue); 
			image = HueRotate(image,hue); 
			printf("\"Hue Rotation\" operation is done!\n");
			break;
		    case 9:
			printf("Please input the resizing percentage (integer between 1 - 500): ");
			scanf("%d", &resize); 
			image = Resize(image,resize);
			printf("\"Resizing the image\" operation is done!\n");
			break;
		    case 10: 
			printf("Please input saturation percentage(-100 - 100): ");
			scanf("%f",&saturate);
			image = Saturate(image,saturate); 
			printf("\"Saturate\" opeartion is done!\n");
			break;
		    case 11:
			printf("Please input rotate option (1 - 4): "); 
			scanf("%d",&rotate); 
			image = Rotate(image,rotate); 
			printf("\"Rotate\" operation is done!\n");    
		    default:
			break;
                }
            }
        }
        else if (option == 12) {    
	AutoTest(); 
            rc = SUCCESS;	/* set returned code SUCCESS, since image is loaded */
        }
        else {
            printf("Invalid selection!\n");
        }

        /* Process finished, waiting for another input */
        PrintMenu();
        printf("Please make your choice: ");
        scanf("%d", &option);
    }
    printf("You exit the program.\n");
    #endif
    	return 0;
}

