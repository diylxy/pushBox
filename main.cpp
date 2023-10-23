#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	return false;
}

//////////////////////////////////////////////////////////

struct roll_back_buffer{
	int delta_x;
	int delta_y;
};
struct roll_back_buffer dfs_rbbuffer[1000000];

int dfs_now_x;
int dfs_now_y;
char dfs_map[12][12];
bool dfs_succ = false;
bool flag = false;
int dfs_move_table_x[4] = {0, 0, 1, -1};
int dfs_move_table_y[4] = {1, -1, 0, 0};

bool dfs_canMove(int delta_x, int delta_y)
{
	int to_x = now_x + delta_x;
	int to_y = now_y + delta_y;
	if(to_x < 0 || to_x >= 12 || to_y < 0 || to_y >= 12)return false;
	if(dfs_map[to_x][to_y] & POS_BLOCK)return false;
	if(dfs_map[to_x][to_y] & POS_CUBE)
	{
		if(dfs_map[to_x + delta_x][to_y + delta_y] & POS_BLOCK)return false;
		return true;
	}
	return true;
}
/**
return true when success
**/
bool dfs_move(int delta_x, int delta_y)
{
	int to_x = now_x + delta_x;
	int to_y = now_y + delta_y;
	if(canMove(delta_x, delta_y))
	{
		dfs_map[now_x][now_y] &= ~POS_NOW;
		dfs_map[to_x][to_y] |= POS_NOW;
		if(dfs_map[to_x][to_y] & POS_CUBE)
		{
			if(dfs_map[to_x + delta_x][to_y + delta_y] & POS_END)
			{
				dfs_map[now_x][now_y] |= POS_NOW;
				dfs_map[to_x][to_y] &= ~POS_NOW;
				dfs_succ = true;
				return true;
			}
			else
			{				
				dfs_map[to_x][to_y] &= ~POS_CUBE;
				dfs_map[to_x + delta_x][to_y + delta_y] |= POS_CUBE;
			}
		}
		now_x = to_x;
		now_y = to_y;
	}
	return false;
}
void rollBack(struct roll_back_buffer *rbbuff)
{
	int prev_x = dfs_now_x - rbbuff->delta_x;
	int prev_y = dfs_now_y - rbbuff->delta_y;
	if(dfs_map[dfs_now_x + rbbuff->delta_x][dfs_now_y + rbbuff->delta_y] & POS_CUBE)
	{
		//here is a cube
		dfs_map[dfs_now_x + rbbuff->delta_x][dfs_now_y + rbbuff->delta_y] &= ~POS_CUBE;
		dfs_map[dfs_now_x][dfs_now_y] |= POS_CUBE;
	}
	dfs_map[dfs_now_x][dfs_now_y] &= ~POS_NOW;
	dfs_map[prev_x][prev_y] |= POS_NOW;
	dfs_now_x = prev_x;
	dfs_now_y = prev_y;
}
void init_dfs()
{
	memcpy(dfs_map, map, sizeof(map));
	dfs_now_x = now_x;
	dfs_now_y = now_y;
	dfs_succ = false;
	flag = false;
	memset(dfs_rbbuffer, 0, sizeof(dfs_rbbuffer));
}
//first, search cube route
bool succ_phase1;

int p1_from_x, p1_from_y, p1_to_x, p1_to_y;
bool findrouteSucc = false;

void findRoute(bool ignoreCube)
{
	int bitmask = POS_CUBE | POS_BLOCK;
	if(ignoreCube)bitmask = POS_BLOCK;
	if(p1_from_x == p1_to_x && p1_from_y == p1_to_y)
	{
		findrouteSucc = true;
		return;		
	}
	for(int i = 0; i < 4; ++i)
	{
		int tmp_x = p1_from_x  + dfs_move_table_x[i];
		int tmp_y = p1_from_y  + dfs_move_table_y[i];
		if(dfs_map[tmp_x][tmp_y] != bitmask)
		{
			p1_from_x = tmp_x;
			p1_from_y = tmp_y;
			findRoute(ignoreCube);
			p1_from_x -= dfs_move_table_x[i];
			p1_from_y -= dfs_move_table_y[i];
		}
		if(findrouteSucc == true)return;
	}
}
void init_findroute(int from_x, int from_y, int to_x, int to_y)
{
	p1_from_x = from_x;
	p1_from_y = from_y;
	p1_to_x = to_x;
	p1_to_y = to_y;
	findrouteSucc = false;
}
int pos_cube_x;
int pos_cube_y;
int pos_end_x;
int pos_end_y;
void init_pos()
{
	for(int i = 0; i < 12; ++i)	
	{
		for(int j = 0; j < 12; ++j)
		{
			switch(dfs_map[i][j])
			{
				case POS_CUBE:
				pos_cube_x = i;
				pos_cube_y = j;
				break;
				case POS_END:
				pos_end_x = i;
				pos_end_y = j;
				break;
				default:
				break;
			}
		}
	}
}
void hasRoute()
{
	init_dfs();
	init_pos();
	init_findroute(pos_cube_x, pos_cube_y, pos_end_x, pos_end_y);
	findRoute(false);
	if(findrouteSucc)
		printf("SUCC\n");
}
void dfs(int now_buffer_id)
{
	for(int i = 0; i < 4; ++i)
	{
		if(dfs_canMove(dfs_move_table_x[i], dfs_move_table_y[i]))
		{
			if(flag)return;
			if(dfs_succ == true)
			{
				printf("SUCC");
				flag = true;
				return;
			}
			dfs_move(dfs_move_table_x[i], dfs_move_table_y[i]);
			dfs_rbbuffer[now_buffer_id].delta_x = dfs_move_table_x[i];
			dfs_rbbuffer[now_buffer_id].delta_y = dfs_move_table_y[i];
			dfs(now_buffer_id + 1);
			rollBack(&dfs_rbbuffer[now_buffer_id]);
		}
	}
}
int main()
{
	for(int i = 0; i < 10; ++i)
	{
		printf("Trying %d\n", i);
		hasRoute();
	}
		system("pause");
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
