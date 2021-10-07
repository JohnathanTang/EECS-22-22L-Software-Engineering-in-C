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
#include "IsValidCheck.h"

int main() {
    /*local variables */
    int game = 1;
    int board[8][8];
    int curr_move[MOV_SIZE] = {0,0,0,0,0};
    int *positions = NULL; /*points to array of curr_move */
    int turn = 0; /*0 means white and 1 means black */
    int check = 0;     check++; check--;
    int valid = 0;
    int selector = 0;
    int ValidCheck = 0;
    /*Create a New Game */
    selector = Initialize(board, &game, &turn, selector);

    while (game >0) {

	printf("\nThis the the number of turns taken place %d \n\n" , turn);

        check = IsCheck(board, turn);
                if(check==1)
                {
                        printf("You are being checked\n\n");
			ValidCheck = 1; 
                }
                else if(check==2)
                {
                        if (turn%2==0)
                        {
                                printf("Black side wins\n\n");
                                break;
                        }
                        else
                        {
                                printf("White side wins\n\n");
                                break;
                        }
                }
        while(valid == 0) {

            PrintBoard(board);
	    
	    if((selector == 1 && turn % 2 == 1)||(selector == 2 && turn % 2 == 0)){
                positions = FirstAI(turn,board);
                valid = IsValid(turn, board, positions); 
	    }
            else{
                positions = ReadInput(turn, board, curr_move);
		valid = IsValid(turn, board, positions);
            }
	    if(ValidCheck ==1 )
	    {
		int vc = IsValidCheck(turn,board,positions); 
		valid = vc; 
	    }
            if (valid == 0)
            {
                printf("Invalid move, please try again\n\n");
            }
            else if(valid == 2){
                printf("Castling\n\n");
            }
            else if(valid == 3){
                printf("En Passant\n\n");
            }
	    
        }
	  
        /*check = isCheck */
        Move(positions,board);
        /* display increment turns, check flags, prev move update, update logs etc */
        turn++;
        valid = 0;
	ValidCheck = 0;
    }
    return 0;
}
