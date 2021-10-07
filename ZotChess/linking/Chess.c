#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "AI.h"
#include "Initialize.h"
#include "PrintBoard.h"
#include "ReadInput.h"
#include "Move.h"
#include "IsValid.h"
#include "Constants.h"
#include "Check.h"

int main() {
    /*local variables */
    int game = 1;
    int board[8][8];
    int curr_move[MOV_SIZE] = {0,0,0,0,0};
    int *positions = NULL; /*points to array of curr_move */
    int turn = 0; /*0 means white and 1 means black */
    int check = 0;     check++; check--;
    int valid = 0;

    /*Create a New Game */
    Initialize(board, &game, &turn);

    printf("\nThis the the number of turns taken place %d \n\n" , turn);
	
    while (game >0) 
	{
        check = IsCheck(board,turn);
                if(check==1)
                {
                        printf("You are being checked\n\n");
                }
                else if(check==2)
                {
                        if (turn%2==0)
                        {
                                printf("Black side wins");
                                break;
                        }
                        else
                        {
                                printf("White side wins");
                                break;
                        }
                }
        while(valid == 0) 
		{
            PrintBoard(board);
			if(game==2)
			{
				positions = FirstAI(board,turn);
				printf("AI suggestion: from x=%d, y=%d, to x=%d,y=%d\n",positions[0],positions[1],positions[2],positions[3]);
			}
			
			positions = ReadInput(turn, board, curr_move);
			valid =  IsValid(turn,board,positions);
            if (valid == 0)
            {
                printf("Invalid move, please try again\n\n");
            }
            else if(valid == 2)
			{
                printf("Castling\n\n");
            }
            else if(valid == 3)
			{
                printf("En Passant\n\n");
            }

        }
        /*check = isCheck */
        Move(positions,board);
        /* display increment turns, check flags, prev move update, update logs etc */
        turn++;
        valid = 0;
    }


    return 0;
}
