/********DO NOT MODIFY THIS SECTION************
 * 
 * EECS 22 : Advanced C Programming
 * Fall 2018, Prof. Quoc-Viet Dang
 * 
 * HW1  : ConnectFour game
 * Template file : ConnectFour.c
 * 
 * Created on September 26, 2018, 12:06 AM by Ali Nickparsa <anickpar@uci.edu>
 */
/**********************************************
 
// Enter your name, last name and UCInet ID below://
 * 
 * *****************************
 * First Name :Johnathan  
 * Last Name:Tang
 * UCInet ID:63061294
 *
 */

#include <stdio.h>

/*Declare your function definitions in the section below*/
/*Mandatory Options : 1-DisplayMenu 2- RegularGame 3-AnvilGame 4-CheckConnect 5-PrintBoard*/
/*You may add other functions if needed*/
/*********************************************************/
int menu(void);
void printboard(void);
void printcolorboard(void);
void regularGame(void);
void anvilGame(void);
void regularColor(void);
void anvilColor(void); 
int checkVertical(void);
int checkHorizontal(void);
int checkDiagLeft(void);
int checkDiagRight(void);
/*********************************************************/

/*Declare your global variables below (if any)*/
/**************************************************/
char board[7][8];


/**************************************************/

/*Colors (you can add or remove from this list*/
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


/*Main function*/

int main(int argc, char** argv)
{
    int loop = 1;
    do{ 
    for (int row = 0; row < 7; row++)//Initialize elements in 2D array
    {
        for(int col = 0; col < 8; col++)
        {
            board [row][col] = ' ';
        }
    }
    int gametype = menu();//Open type of game based on user input
    if(gametype == 1)
    {
        regularGame();
    }
    else if (gametype == 2)
    {
        anvilGame();
    }
    else if (gametype == 3)
    {
	regularColor();
    }
    else if (gametype == 4)
    {
	anvilColor();
    }
    printf("\n(1)- Yes\n");//Prompt user if they would like to re-run the program
    printf("(2)- No\n"); 
    printf("Would you like to play again (1/2): ");
    scanf("%d",&loop);
    }while(loop == 1);
    
    return 0;  
    
}


int menu()//Menu function with options, will return integer corresponding to the game user chooses to play
{
    int gametype = 0, choice = 0;
    printf("\tWelcome to Connect 4 Game : EECS 22 HW1\n");
    printf("Please select the type of game you want to play :\n");
    printf("1- Regular\n");
    printf("2- ANVIL Variation\n");
    printf("3- Regular "); 
    printf("%sC","\x1B[31m");
    printf("%so","\x1B[32m");
    printf("%sl","\x1B[33m");
    printf("%so","\x1B[34m");
    printf("%sr\n","\x1B[35m");
    printf("%s",KNRM);
    printf("4- ANVIL ");
    printf("%sC","\x1B[31m");
    printf("%so","\x1B[32m");
    printf("%sl","\x1B[33m");
    printf("%so","\x1B[34m");
    printf("%sr","\x1B[35m");
    printf("%s",KNRM);
    printf("\nEnter 1, 2, 3, or 4: ");
    scanf("%d",&choice);
    if(choice == 1)
    {
        printf("Regular mode selected\n\n");
        gametype = 1;
    }
    else if(choice == 2)
    {
        printf("ANVIL mode selected\n");
        printf("To use ANVIL, press '8' and then the column you want to place it in\n");
        gametype = 2;
    }
    else if (choice == 3)
    {
        printf("Normal ");
	printf("%sC","\x1B[31m");
	printf("%so","\x1B[32m");
	printf("%sl","\x1B[33m");
	printf("%so","\x1B[34m");
	printf("%sr","\x1B[35m");
	printf("%s mode selected",KNRM);
        gametype = 3;
    }
    else if (choice == 4)
    {
        printf("ANVIL ");
	printf("%sC","\x1B[31m");
	printf("%so","\x1B[32m");
	printf("%sl","\x1B[33m");
	printf("%so","\x1B[34m");
	printf("%sr","\x1B[35m"); 
	printf("%s mode selected","\x1B[37m");
        gametype = 4;
    }
    return gametype;
}

void printboard()//Function to print grid of the game
{
    printf("\n    ****Connect Four****\n\n");
    for(int row = 1; row < 7; row++)
    {
        for(int col = 1; col < 8; col++)
        {
            printf("| %c ",board[row][col]);
        }
      
        printf("|\n-----------------------------\n");
    }
    printf("  1   2   3   4   5   6   7\n");
}

void printcolorboard()//Function to print grid of color game modes
{
	printf("\n    ****Connect Four****\n\n");
	for (int row = 1; row < 7; row++)
	{
		for (int col = 1; col < 8; col++)
		{
			printf("|");
			if (board[row][col] == 'X')//Change color of character stored in 2D array while keeping the grid WHITE
			{
				printf("%s %c %s",KRED,board[row][col],KNRM);//Player 1(X) will be RED
			}
			else if (board[row][col] == 'O')
			{
				printf("%s %c %s",KBLU,board[row][col],KNRM);//Player 2(O) will be BLUE
			}
			else
			{
				printf("%s %c %s",KCYN,board[row][col],KNRM);//ANVIL chip will be CYAN
			}
		}
		printf("|\n-----------------------------\n"); 
	}
	printf("  1   2   3   4   5   6   7\n"); 
}

void regularGame()
{
    printboard();
    for(int counter = 1;counter < 42;counter++ )//Count number of turns that pass, once counter hits max number of turns, game is tied
    {
        int player1 = 0, player2 = 0;
        printf("\nPlayer 1 (X):\nEnter number coordinate: ");
        scanf("%d",&player1);
        if (board[1][player1] != ' ')//If the column is full, prompt user to select another column
	{
	    do{
            printf("\nColumn full!");
            printf("\nPlayer 1 (X):\nEnter number coordinate: ");
            scanf("%d",&player1);
            for (int i = 6; i>=0; i--)
            {
                if(board[i][player1] == ' ')//Place player's marker on the first open slot availible from top to bottom of column
                {
                    board[i][player1] = 'X';
                    printboard();
                    break;
                }
            }
	}while(board[1][player1] != ' ');
        }
        else//If column is not full, place player's marker on the first open slot availible from top to bottom of column
        {
            for (int i = 6; i>=0; i--)
            {
                if(board[i][player1] == ' ')
                {
                    board[i][player1] = 'X';
                    printboard();
                    break;
                }
            }
        }
        int v1 = checkVertical();//Check for vertical win condition
        checkVertical();
        if (v1==1)
        {
            printf("Player 1 wins!");
            break;
        }
        int h1 = checkHorizontal();//Check for horizontal win condition
        checkHorizontal();
        if (h1==1)
        {
            printf("Player 1 wins!");
            break;
        }
        int dl1 = checkDiagLeft();//Check for left diagonal win condition
        checkDiagLeft();
        if(dl1 == 1)
        {
            printf("Player 1 wins!");
            break;
        }
        int dr1 = checkDiagRight();//Check for right diagonal win condition
        checkDiagRight();
        if(dr1 == 1)
        {
            printf("Player 1 wins!");
            break;
        }
        printf("\nPlayer 2 (O):\nEnter number coordinate: ");//Repeat exact  algorithm for player 2
        scanf("%d",&player2);
        if (board[1][player2] != ' ')
        {
	    do{
            printf("\nColumn full!");
            printf("\nPlayer 2 (O):\nEnter number coordinate: ");
            scanf("%d",&player2);
            for (int i = 6; i>=0; i--)
            {
                if(board[i][player2] == ' ')
                {
                    board[i][player2] = 'O';
                    printboard();
                    break;
                }
            }
	}while(board[1][player2] != ' ');
        }
        else
        {
            for (int i = 6; i>=0; i--)
            {
                if(board[i][player2] == ' ')
                {
                    board[i][player2] = 'O';
                    printboard();
                    break;
                }
            }
        }
        int v2 = checkVertical();
        checkVertical();
        if(v2 == 1)
        {
            printf("Player 2 wins!");
            break;
        }
        int h2 = checkHorizontal();
        checkHorizontal();
        if(h2 == 1)
        {
            printf("Player 2 wins!");
            break;
        }
        int dl2 = checkDiagLeft();
        checkDiagLeft();
        if(dl2 == 1)
        {
            printf("Player 2 wins!");
            break;
        }
        int dr2 = checkDiagRight();
        checkDiagRight();
        if(dr2 == 1)
        {
            printf("Player 1 wins!");
            break;
        }
    }
}

void anvilGame()
{
    printboard();
    int anvilCounter1 = 0, anvilCounter2 = 0;//Since we want to limit each player to use their ANVIl chip once per game, use counter variable
    int anvil1 = 0, anvil2 = 0;
    for(int counter = 0;counter < 42;counter++ )
    {
        int player1 = 0, player2 = 0;
        printf("\nPlayer 1 (X):\nEnter number coordinate: ");
        scanf("%d",&player1);
        if(player1 == 8 && anvilCounter1 == 0)//Player will only be prompted to use their ANVIL chip if they select '8' AND if their ANVIL counter is still 0
        {
            printf("ANVIL selected, now select column: ");
            scanf("%d",&anvil1);
            for(int i = 1; i < 8; i++)
            {
                if(board[i][anvil1] != 'A')//Clear every chip in the column the player chooses and place ANVIL chip on the bottom
                {
                    board[i][anvil1] = ' ';
                }
            }
            if(board[6][anvil1] == 'A')//If there is already an ANVIL chip in the same column player chooses, place ANVIL right above the first one
            {
                board[5][anvil1] = 'A';
                anvilCounter1++;
                printboard();
            }
            else
            {
                board[6][anvil1] = 'A';
                anvilCounter1++;
                printboard();
            }
        }
        else if (player1 == 8 && anvilCounter1 == 1)//IF ANVIL counter has already been used once, user will be prompted so
        {
            printf("ANVIL already used!");
            printf("\nPlayer 1 (X):\nEnter number coordinate: ");
            scanf("%d",&player1);
            for (int i = 6; i>=0; i--)
            {
                if(board[i][player1] == ' ')
                {
                    board[i][player1] = 'X';
                    printboard();
                    break;
                }
            }
        }
        else//If user chooses not to use ANVIL chip, game proceeds as a normal game
        {
            if (board[1][player1] != ' ')
            {
		do{
                printf("\nColumn full!");
                printf("\nPlayer 1 (X):\nEnter number coordinate: ");
                scanf("%d",&player1);
                for (int i = 6; i>=0; i--)
                {
                    if(board[i][player1] == ' ')
                    {
                        board[i][player1] = 'X';
                        printboard();
                        break;
                    }
                }
	    }while(board[1][player1] != ' ');
            }
            else
            {
                for (int i = 6; i>=0; i--)
                {
                    if(board[i][player1] == ' ')
                    {
                        board[i][player1] = 'X';
                        printboard();
                        break;
                    }
                }
            }
        }//Check win conditions
        int v1 = checkVertical();
        checkVertical();
        if (v1==1)
        {
            printf("Player 1 wins!");
            break;
        }
        int h1 = checkHorizontal();
        checkHorizontal();
        if (h1==1)
        {
            printf("Player 1 wins!");
            break;
        }
        int dl1 = checkDiagLeft();
        checkDiagLeft();
        if(dl1 == 1)
        {
            printf("Player 1 wins!");
            break;
        }
        int dr1 = checkDiagRight();
        checkDiagRight();
        if(dr1 == 1)
        {
            printf("Player 1 wins!");
            break;
        }
	//Same algorithm for player 2 
        printf("\nPlayer 2 (O):\nEnter number coordinate: ");
        scanf("%d",&player2);
        if(player2 == 8 && anvilCounter2 == 0)
        {
            printf("ANVIL selected, now select column: ");
            scanf("%d",&anvil2);
            for(int i = 1; i < 8; i++)
            {
                if(board[i][anvil2] != 'A')
                {
                    board[i][anvil2] = ' ';
                }
            }
            if(board[6][anvil2] == 'A')
            {
                board[5][anvil2] = 'A';
                anvilCounter2++;
                printboard();
            }
            else
            {
                board[6][anvil2] = 'A';
                anvilCounter2++;
                printboard();
            }
        }
        else if (player2 == 8 && anvilCounter2 == 1)
        {
            printf("ANVIL already used!");
            printf("\nPlayer 2 (O):\nEnter number coordinate: ");
            scanf("%d",&player2);
            for (int i = 6; i>=0; i--)
            {
                if(board[i][player2] == ' ')
                {
                    board[i][player2] = 'O';
                    printboard();
                    break;
                }
            }
        }
        else
        {
            if (board[1][player2] != ' ')
            {
	    	do{
                printf("\nColumn full!");
                printf("\nPlayer 2 (O):\nEnter number coordinate: ");
                scanf("%d",&player2);
                for (int i = 6; i>=0; i--)
                {
                    if(board[i][player2] == ' ')
                    {
                        board[i][player2] = 'O';
                        printboard();
                        break;
                    }
                }
	    }while(board[1][player2] != ' ');
            }
            else
            {
                for (int i = 6; i>=0; i--)
                {
                    if(board[i][player2] == ' ')
                    {
                        board[i][player2] = 'O';
                        printboard();
                        break;
                    }
                }
            }
        }
        int v2 = checkVertical();
        checkVertical();
        if(v2 == 1)
        {
            printf("Player 2 wins!");
            break;
        }
        int h2 = checkHorizontal();
        checkHorizontal();
        if(h2 == 1)
        {
            printf("Player 2 wins!");
            break;
        }
        int dl2 = checkDiagLeft();
        checkDiagLeft();
        if(dl2 == 1)
        {
            printf("Player 2 wins!");
            break;
        }
        int dr2 = checkDiagRight();
        checkDiagRight();
        if(dr2 == 1)
        {
            printf("Player 1 wins!");
            break;
        }
    }
}

void regularColor()//Same function as regulargame except we're printing to the colorboard rather the normal one
{
    printboard();
    for(int counter = 1;counter < 42;counter++ )
    {
        int player1 = 0, player2 = 0;
        printf("\nPlayer 1 (X):\nEnter number coordinate: ");
        scanf("%d",&player1);
        if (board[1][player1] != ' ')
        {
	    do{
            printf("\nColumn full!");
            printf("\nPlayer 1 (X):\nEnter number coordinate: ");
            scanf("%d",&player1);
            for (int i = 6; i>=0; i--)
            { 
                if(board[i][player1] == ' ')
                {
		    board[i][player1] = 'X';
                    printcolorboard();
                    break;
                }
            }
        }while(board[1][player1] != ' ');
	}
        else
        {
            for (int i = 6; i>=0; i--)
            {
                if(board[i][player1] == ' ')
                {
                    board[i][player1] = 'X';
                    printcolorboard();
                    break;
                }
            }
        }
        int v1 = checkVertical();
        checkVertical();
        if (v1==1)
        {
            printf("Player 1 wins!");
            break;
        }
        int h1 = checkHorizontal();
        checkHorizontal();
        if (h1==1)
        {
            printf("Player 1 wins!");
            break;
        }
        int dl1 = checkDiagLeft();
        checkDiagLeft();
        if(dl1 == 1)
        {
            printf("Player 1 wins!");
            break;
        }
        int dr1 = checkDiagRight();
        checkDiagRight();
        if(dr1 == 1)
        {
            printf("Player 1 wins!");
            break;
        }
        printf("\nPlayer 2 (O):\nEnter number coordinate: ");
        scanf("%d",&player2);
        if (board[1][player2] != ' ')
        {
	    do{
            printf("\nColumn full!");
            printf("\nPlayer 2 (O):\nEnter number coordinate: ");
            scanf("%d",&player2);
            for (int i = 6; i>=0; i--)
            {
                if(board[i][player2] == ' ')
                {
                    board[i][player2] = 'O';
                    printcolorboard();
                    break;
                }
            }
        }while(board[1][player2] != ' ');
        }
        else
        {
            for (int i = 6; i>=0; i--)
            {
                if(board[i][player2] == ' ')
                {
                    board[i][player2] = 'O';
                    printcolorboard();
                    break;
                }
            }
        }
        int v2 = checkVertical();
        checkVertical();
        if(v2 == 1)
        {
            printf("Player 2 wins!");
            break;
        }
        int h2 = checkHorizontal();
        checkHorizontal();
        if(h2 == 1)
        {
            printf("Player 2 wins!");
            break;
        }
        int dl2 = checkDiagLeft();
        checkDiagLeft();
        if(dl2 == 1)
        {
            printf("Player 2 wins!");
            break;
        }
        int dr2 = checkDiagRight();
        checkDiagRight();
        if(dr2 == 1)
        {
            printf("Player 2 wins!");
            break;
        }
    }
}

void anvilColor()//Same function as ANVIL function except the function calls to colorboard
{
    printcolorboard();
    int anvilCounter1 = 0, anvilCounter2 = 0;
    int anvil1 = 0, anvil2 = 0;
    for(int counter = 0;counter < 42;counter++ )
    {
        int player1 = 0, player2 = 0;
        printf("\nPlayer 1 (X):\nEnter number coordinate: ");
        scanf("%d",&player1);
        if(player1 == 8 && anvilCounter1 == 0)
        {
            printf("ANVIL selected, now select column: ");
            scanf("%d",&anvil1);
            for(int i = 1; i < 8; i++)
            {
                if(board[i][anvil1] != 'A')
                {
                    board[i][anvil1] = ' ';
                }
            }
            if(board[6][anvil1] == 'A')
            {
                board[5][anvil1] = 'A';
                anvilCounter1++;
                printcolorboard();
            }
            else
            {
                board[6][anvil1] = 'A';
                anvilCounter1++;
                printcolorboard();
            }
        }
        else if (player1 == 8 && anvilCounter1 == 1)
        {
            printf("ANVIL already used!");
            printf("\nPlayer 1 (X):\nEnter number coordinate: ");
            scanf("%d",&player1);
            for (int i = 6; i>=0; i--)
            {
                if(board[i][player1] == ' ')
                {
                    board[i][player1] = 'X';
                    printcolorboard();
                    break;
                }
            }
        }
        else
        {
            if (board[1][player1] != ' ')
            {
	        do{
                printf("\nColumn full!");
                printf("\nPlayer 1 (X):\nEnter number coordinate: ");
                scanf("%d",&player1);
                for (int i = 6; i>=0; i--)
                {
                    if(board[i][player1] == ' ')
                    {
                        board[i][player1] = 'X';
                        printcolorboard();
                        break;
                    }
                }
	    }while(board[1][player1] != ' ');	
            }
            else
            {
                for (int i = 6; i>=0; i--)
                {
                    if(board[i][player1] == ' ')
                    {
                        board[i][player1] = 'X';
                        printcolorboard();
                        break;
                    }
                }
            }
        }
        int v1 = checkVertical();
        checkVertical();
        if (v1==1)
        {
            printf("Player 1 wins!");
            break;
        }
        int h1 = checkHorizontal();
        checkHorizontal();
        if (h1==1)
        {
            printf("Player 1 wins!");
            break;
        }
        int dl1 = checkDiagLeft();
        checkDiagLeft();
        if(dl1 == 1)
        {
            printf("Player 1 wins!");
            break;
        }
        int dr1 = checkDiagRight();
        checkDiagRight();
        if(dr1 == 1)
        {
            printf("Player 1 wins!");
            break;
        }
        printf("\nPlayer 2 (O):\nEnter number coordinate: ");
        scanf("%d",&player2);
        if(player2 == 8 && anvilCounter2 == 0)
        {
            printf("ANVIL selected, now select column: ");
            scanf("%d",&anvil2);
            for(int i = 1; i < 8; i++)
            {
                if(board[i][anvil2] != 'A')
                {
                    board[i][anvil2] = ' ';
                }
            }
            if(board[6][anvil2] == 'A')
            {
                board[5][anvil2] = 'A';
                anvilCounter2++;
                printcolorboard();
            }
            else
            {
                board[6][anvil2] = 'A';
                anvilCounter1++;
                printcolorboard();
            }
        }
        else if (player2 == 8 && anvilCounter2 == 1)
        {
            printf("ANVIL already used!");
            printf("\nPlayer 2 (O):\nEnter number coordinate: ");
            scanf("%d",&player2);
            for (int i = 6; i>=0; i--)
            {
                if(board[i][player2] == ' ')
                {
                    board[i][player2] = 'O';
                    printcolorboard();
                    break;
                }
            }
        }
        else
        {
            if (board[1][player2] != ' ')
            {
	        do{
                printf("\nColumn full!");
                printf("\nPlayer 2 (O):\nEnter number coordinate: ");
                scanf("%d",&player2);
                for (int i = 6; i>=0; i--)
                {
                    if(board[i][player2] == ' ')
                    {
                        board[i][player2] = 'O';
                        printcolorboard();
                        break;
                    }
                }
	    }while(board[1][player2] != ' ');
            }
            else
            {
                for (int i = 6; i>=0; i--)
                {
                    if(board[i][player2] == ' ')
                    {
                        board[i][player2] = 'O';
                        printcolorboard();
                        break;
                    }
                }
            }
        }
        int v2 = checkVertical();
        checkVertical();
        if(v2 == 1)
        {
            printf("Player 2 wins!");
            break;
        }
        int h2 = checkHorizontal();
        checkHorizontal();
        if(h2 == 1)
        {
            printf("Player 2 wins!");
            break;
        }
        int dl2 = checkDiagLeft();
        checkDiagLeft();
        if(dl2 == 1)
        {
            printf("Player 2 wins!");
            break;
        }
        int dr2 = checkDiagRight();
        checkDiagRight();
        if(dr2 == 1)
        {
            printf("Player 2 wins!");
            break;
        }
    }
}

int checkVertical()
{
    int vertical = 0;
    for (int row = 1;row < 7; row++)
    {
        for(int col = 1; col < 8; col++)
        {
            char player = board[row][col];
            if (player != ' ' && player == board[row+1][col] && player == board[row+2][col] && player == board[row+3][col])
	    //Scan entire grid for 4 consecutive player marks in each column 

            {
                vertical = 1;
            }
        }
    }
    return vertical;//Return value based on whether the function found a consecutive four
}

int checkHorizontal()
{
    int horizontal = 0;
    for (int row = 1; row < 7; row++)
    {
        for(int col = 1; col < 8; col++)
        {
            char player = board[row][col];
            if(player != ' ' && player == board[row][col+1] && player == board[row][col+2] && player == board[row][col+3])
	    //Scan entire grid for 4 consecutive player marks in each row
            {
                horizontal = 1;
            }
        }
    }
    return horizontal;
}

int checkDiagLeft()
{
    int diagL = 0;
    for (int row = 1; row < 7; row++)
    {
        for (int col =1; col < 8; col++)
        {
            char player = board[row][col];
            if(player != ' ' && player == board[row+1][col-1] && player == board[row+2][col-2] && player == board[row+3][col-3])
	    //Scan entire grid for 4 consecutive player marks down each diagonal line oriented to the left
            {
                diagL = 1;
            }
        }
    }
    return diagL;
}

int checkDiagRight()
{
    int diagR = 0;
    for (int row = 1; row < 7; row++)
    {
        for (int col =1; col < 8; col++)
        {
            char player = board[row][col];
            if(player != ' ' && player == board[row+1][col+1] && player == board[row+2][col+2] && player == board[row+3][col+3])
	    //Scan entire grid for 4 consecutive player marks down each diagonal line oriented to the right
            {
                diagR = 1;
            }
        }
    }
    return diagR;
}

/*EOF*/
