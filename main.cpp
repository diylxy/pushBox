#include <stdio.h>
#include <stdlib.h>
#define POS_EMPTY 0
#define POS_BLOCK 1
#define POS_NOW 2
#define POS_CUBE 4
#define POS_END 8

char map[12][12];
int now_x, now_y;
bool succ = false;
void gen_map()
{
	succ = false;
	for(int i = 0; i < 12; ++i)	
	{
		for(int j = 0; j < 12; ++j)
		{
			if(i == 0 || i == 11)map[i][j] = 1;
			else if(j == 0 || j == 11)map[i][j] = 1;
			else if(rand() % 5 == 0)map[i][j] = 1;
			else map[i][j] = POS_EMPTY;
		}
	}
	while(1)
	{
		int tmp_x = rand() % 10;
		int tmp_y = rand() % 10;
		if(map[tmp_x + 1][tmp_y + 1] == 0)
		{
			map[tmp_x + 1][tmp_y + 1] = POS_NOW;
			now_x = tmp_x + 1;
			now_y = tmp_y + 1;
			break;
		}
	}
	while(1)
	{
		int tmp_x = rand() % 10;
		int tmp_y = rand() % 10;
		if(map[tmp_x + 1][tmp_y + 1] == 0)
		{
			map[tmp_x + 1][tmp_y + 1] = POS_BLOCK;
			break;
		}
	}
	while(1)
	{
		int tmp_x = rand() % 10;
		int tmp_y = rand() % 10;
		if(map[tmp_x + 1][tmp_y + 1] == 0)
		{
			map[tmp_x + 1][tmp_y + 1] = POS_CUBE;
			break;
		}
	}	
	while(1)
	{
		int tmp_x = rand() % 10;
		int tmp_y = rand() % 10;
		if(map[tmp_x + 1][tmp_y + 1] == 0)
		{
			map[tmp_x + 1][tmp_y + 1] = POS_END;
			break;
		}
	}	
}

void print_map()
{
	for(int i = 0; i < 12; ++i)	
	{
		for(int j = 0; j < 12; ++j)
		{
			switch(map[i][j])
			{
				case POS_EMPTY:
				printf(" ");
				break;
				case POS_NOW:
				printf("n");
				break;
				case POS_BLOCK:
				printf("#");
				break;
				case POS_CUBE:
				printf("O"); 
				break;       
				case POS_END:
				printf("*"); 
				break;       
				default:
				printf("?"); 
				break;
			}
		}
		printf("\n");
	}
}
bool canMove(int delta_x, int delta_y)
{
	int to_x = now_x + delta_x;
	int to_y = now_y + delta_y;
	if(to_x < 0 || to_x >= 12 || to_y < 0 || to_y >= 12)return false;
	if(map[to_x][to_y] & POS_BLOCK)return false;
	if(map[to_x][to_y] & POS_CUBE)
	{
		if(map[to_x + delta_x][to_y + delta_y] & POS_BLOCK)return false;
		return true;
	}
	return true;
}
/**
return true when success
**/
bool move(int delta_x, int delta_y)
{
	int to_x = now_x + delta_x;
	int to_y = now_y + delta_y;
	if(canMove(delta_x, delta_y))
	{
		map[now_x][now_y] &= ~POS_NOW;
		map[to_x][to_y] |= POS_NOW;
		if(map[to_x][to_y] & POS_CUBE)
		{
			if(map[to_x + delta_x][to_y + delta_y] & POS_END)
			{
				map[now_x][now_y] |= POS_NOW;
				map[to_x][to_y] &= ~POS_NOW;
				succ = true;
				return true;
			}
			else
			{				
				map[to_x][to_y] &= ~POS_CUBE;
				map[to_x + delta_x][to_y + delta_y] |= POS_CUBE;
			}
		}
		now_x = to_x;
		now_y = to_y;
	}
	
}
struct roll_back_buffer{
	int delta_x;
	int delta_y;
};
int dfs_now_x = ;
int dfs_now_y;
void rollBack(struct roll_back_buffer rbbuff)
{
			
	
}
int main()
{
	start:
	gen_map();
	while(1)
	{
		system("cls");
		print_map();
		char c = getchar();
		switch(c)
		{
			case 'w':
			move(-1, 0);	
			break;
			case 'a':
			move(0, -1);	
			break;
			case 's':
			move(1, 0);	
			break;
			case 'd':
			move(0, 1);	
			break;
			case 'q':
			printf("Thanks\n");
			return 0;
			break;
			case 'r':
			goto start;
			break;
		}
		if(succ)
		{
			printf("\nYOU WIN!!!\n");
			system("pause");
			goto start;
		}
	}
    return 0;
}
