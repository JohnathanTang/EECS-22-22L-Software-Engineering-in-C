#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "AI.h"
#include "Move.h"
#include "IsValid.h"

int *FirstAI(int board[8][8],int turn)
{
	static int result[5];//what should return
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;//y x y x
	int KingPosi_x,KingPosi_y;
	int x,y;
//        Coordinate initial = get_set_coordinate(positions[0],positions[1]);
//        Coordinate final = get_set_coordinate(positions[2],positions[3]);

	if(turn%2==1)
	{
		
		//following clears the result array
		for(i=0; i<5; i++){
				result[i] = 0;
		}
		
		for(x=0;x<8;x++)
		{
			for(y=0;y<8;y++)
			{
				if(board[y][x]==5)
				{
					KingPosi_x=x;
					KingPosi_y=y;
					
				}
			}
		}
		srand(time(0));
		while(1)
		{
			while(1)
			{
				i=rand()%8;
				j=rand()%8;
				if(board[i][j] == 7 || board[i][j] == 8 || board[i][j] == 9 || board[i][j] == 10 ||board[i][j] == 11 ||board[i][j] == 12)
				{
					break;
				}
				
			}
			if(KingPosi_x<j+1 && KingPosi_y>i-1)
			{
				for(k = KingPosi_x; k <j+1; k++)
				{
					for(l = KingPosi_y; l>i-1; l--)
					{
						result[0] = i;
						result[1] = j;
						result[2] = k;
						result[3] = l;
						result[4] = -1;
						if(IsValid(turn, board,result) == 1)
						{
								return result;
						}
					}
				}
			}
			else if(KingPosi_x>j && KingPosi_y>i-1)
			{
				for(k = KingPosi_x; k >j-1; k--)
				{
					for(l = KingPosi_y; l>i-1; l--)
					{
						result[0] = i;
						result[1] = j;
						result[2] = k;
						result[3] = l;
						result[4] = -1;
						if(IsValid(turn, board,result) == 1)
						{
								return result;
						}
					}
				}
			}
			else if(KingPosi_x<j+1 && KingPosi_y<i+1)
			{
				for(k = KingPosi_x; k <j+1; k++)
				{
					for(l = KingPosi_y; l<i+1; l++)
					{
						result[0] = i;
						result[1] = j;
						result[2] = k;
						result[3] = l;
						result[4] = -1;
						if(IsValid(turn, board,result) == 1)
						{
								return result;
						}
					}
				}
			}
			else
			{
				for(k = KingPosi_x; k >j-1; k--)
				{
					for(l = KingPosi_y; l<i+1; l++)
					{
						result[0] = i;
						result[1] = j;
						result[2] = k;
						result[3] = l;
						result[4] = -1;
						if(IsValid(turn, board,result) == 1)
						{
								return result;
						}
					}
				}
			}
			
			
		}
	}
	else
	{
		for(i=0; i<5; i++){
				result[i] = 0;
		}
		for(x=0;x<8;x++)
		{
			for(y=0;y<8;y++)
			{
				if(board[y][x]==11)
				{
					KingPosi_x=x;
					KingPosi_y=y;
					
				}
			}
		}
		srand(time(0));
		while(1)
		{
			while(1)
			{
				i=rand()%8;
				j=rand()%8;
				if(board[i][j] == 1 || board[i][j] == 2 || board[i][j] == 3 || board[i][j] == 4 ||board[i][j] == 5 ||board[i][j] == 6)
				{
					break;
				}
				
			}
			if(KingPosi_x<j+1 && KingPosi_y>i-1)
			{
				for(k = KingPosi_x; k <j+1; k++)
				{
					for(l = KingPosi_y; l>i-1; l--)
					{
						result[0] = i;
						result[1] = j;
						result[2] = k;
						result[3] = l;
						result[4] = -1;
						if(IsValid(turn, board,result) == 1)
						{
								return result;
						}
					}
				}
			}
			else if(KingPosi_x>j && KingPosi_y>i-1)
			{
				for(k = KingPosi_x; k >j-1; k--)
				{
					for(l = KingPosi_y; l>i-1; l--)
					{
						result[0] = i;
						result[1] = j;
						result[2] = k;
						result[3] = l;
						result[4] = -1;
						if(IsValid(turn, board,result) == 1)
						{
								return result;
						}
					}
				}
			}
			else if(KingPosi_x<j+1 && KingPosi_y<i+1)
			{
				for(k = KingPosi_x; k <j+1; k++)
				{
					for(l = KingPosi_y; l<i+1; l++)
					{
						result[0] = i;
						result[1] = j;
						result[2] = k;
						result[3] = l;
						result[4] = -1;
						if(IsValid(turn, board,result) == 1)
						{
								return result;
						}
					}
				}
			}
			else
			{
				for(k = KingPosi_x; k >j-1; k--)
				{
					for(l = KingPosi_y; l<i+1; l++)
					{
						result[0] = i;
						result[1] = j;
						result[2] = k;
						result[3] = l;
						result[4] = -1;
						if(IsValid(turn, board,result) == 1)
						{
								return result;
						}
					}
				}
			}
			
			
		}
	}
}
