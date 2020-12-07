
#include <iostream>
#include <vector>
#include "AlienAstar.h";
using namespace std;
const int gWidth = 24;
char junk;
int endRow = 20,endCol = 19;
int startRow = 4, startCol = 13;

//Use this to make changes to the map. (# = Obstacle)
char mapEditor[24][24] = {
{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '.', '.', '.', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '.', '.', '.', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '.', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '.', '.', '.', '.', '.', '#', '#', '#', '#', '#', '#', '#', '#'},
{'#', '#', '#', '#', '#', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '#', '#'},
{'#', '#', '#', '#', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '#'},
{'#', '#', '#', '#', '#', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '#', '#', '#', '#', '.', '.', '.', '#', '#'},
{'#', '#', '#', '#', '#', '#', '.', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '.', '#', '#', '#'},
{'#', '#', '#', '#', '#', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '#', '#'},
{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '#', '.', '.', '.', '.', '.', '#'},
{'#', '#', '#', '#', '#', '.', '.', '.', '#', '#', '#', '#', '.', '.', '.', '#', '#', '#', '#', '.', '.', '.', '#', '#'},
{'#', '#', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '.', '.', '.', '#', '.', '#', '#', '#', '.', '#', '#', '#'},
{'#', '.', '.', '.', '.', '.', '.', '#', '#', '#', '#', '.', '#', '.', '.', '#', '.', '#', '#', '.', '.', '.', '#', '#'},
{'#', '#', '#', '.', '#', '.', '#', '#', '#', '#', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#'},
{'#', '#', '#', '#', '#', '#', '.', '#', '#', '.', '.', '.', '.', '.', '.', '#', '.', '#', '#', '.', '.', '.', '#', '#'},
{'#', '#', '#', '#', '#', '.', '.', '.', '#', '.', '.', '#', '.', '.', '.', '#', '.', '#', '#', '.', '.', '.', '#', '#'},
{'#', '#', '#', '#', '#', '.', '.', '.', '#', '.', '.', '#', '.', '.', '.', '#', '#', '#', '#', '.', '.', '.', '#', '#'},
{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '#', '#', '.', '.', '#', '.', '#', '#', '#'},
{'#', '#', '#', '#', '#', '#', '.', '#', '#', '#', '#', '#', '.', '.', '.', '#', '.', '.', '.', '.', '.', '#', '#', '#'},
{'#', '#', '#', '#', '#', '#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '#', '#'},
{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '.', '.', '.', '#', '#', '.', '.', '#', '#', '#', '#', '#'},
{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '.', '.', '.', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};

class node
{
public:
	int row,col,width,tRows;
	int nbors[4][2],nborsCount;
	int parentPos[2];
	int hCost,gCost,fCost;
	char type = '.';
	

	//Getters
	bool isClosed()
	{
		if (type == 'X')
		{
			return true;

		}
		else
		{

			return false;

		}

	}

	bool isBlank()
	{
		if (type == '.')
		{
			return true;

		}
		else
		{

			return false;

		}

	}

	bool isOpen()
	{
		if (type == 'O')
		{
			return true;

		}
		else
		{

			return false;

		}

	}

	bool isObstacle()
	{
		if (type == '#')
		{

			return true;
		}
		else
		{

			return false;
		}
	}


	bool isStart()
	{
		if (type == 'A')
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool isEnd()
	{
		if (type == 'B')
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void reset()
	{
		type = '.';
	}

	//Setters
	void makeClosed()
	{

			type = 'X';
		
	}

	void makeOpen()
	{
		type = 'O';
	}

	void makeObstacle()
	{
		type = '#';
	}

	void makeStart()
	{
		type = 'A';
	}

	void makeEnd()
	{
		type = 'B';
	}

	void makePath()
	{
		type = '*';
	}

	void draw(node grid[gWidth][gWidth])
	{
		grid[row][col].type = type;
	}

	void updateNbors(node grid[gWidth][gWidth]) //Finds the neighboring nodes to the nodes being checked.
	{
		nborsCount = 0;
		if (row < tRows - 1 && grid[row + 1][col].isBlank() == true || grid[row + 1][col].isEnd() == true) //DOWN
		{
			nbors[nborsCount][0] = row + 1;
			nbors[nborsCount][1] = col;

			grid[row + 1][col].parentPos[0] = row;
			grid[row + 1][col].parentPos[1] = col;
			nborsCount += 1;
		}

		if (row > 0 && grid[row - 1][col].isBlank() == true || grid[row - 1][col].isEnd() == true) //UP
		{
			nbors[nborsCount][0] = row - 1;
			nbors[nborsCount][1] = col;

			grid[row - 1][col].parentPos[0] = row;
			grid[row - 1][col].parentPos[1] = col;
			nborsCount += 1;

		}

		if (col < tRows - 1 && grid[row][col + 1].isBlank() == true || grid[row][col + 1].isEnd() == true) //RIGHT
		{
			nbors[nborsCount][0] = row;
			nbors[nborsCount][1] = col + 1;

			grid[row][col + 1].parentPos[0] = row;
			grid[row][col + 1].parentPos[1] = col;
			nborsCount += 1;

		}

		if (col > 0 && grid[row][col - 1].isBlank() == true || grid[row][col - 1].isEnd() == true) //LEFT
		{
			nbors[nborsCount][0] = row;
			nbors[nborsCount][1] = col - 1;

			grid[row][col - 1].parentPos[0] = row;
			grid[row][col - 1].parentPos[1] = col;
			nborsCount += 1;

		}

	}


};
//AI Grid
node grid[gWidth][gWidth];

node openSet[576];
int openCount = 0;

node closedSet[576];
int closedCount = 0;

node pathSet[576];
int pathCount = 0;


//Calculates G cost 
int g(node p1) 
{
	int x1 = p1.row, y1 = p1.col;
	int x2 = startRow, y2 = startCol; 
	int gCost = abs(x1 - x2) + abs(y1 - y2);
	return gCost;

}

//Using this function instead of the regular g cost function will make the AI take a simpler, slower path.
int gNull(node p1)
{
	int x2 = p1.row, y2 = p1.col;
	int x1 = endRow, y1 = endCol; 
	int gCost = abs(x1 - x2) + abs(y1 - y2);
	return gCost;

}

//Calculates H cost
int h(node p1)
{
	int x1 = p1.row, y1 = p1.col;
	int x2 = endRow, y2 = endCol;
	int hCost = abs(x1 - x2) + abs(y1 - y2);
	return hCost;
}

//Calculates F cost
int f(node p1)
{
	int fCost = p1.gCost + p1.hCost;
	return fCost;
}

//Generates the AI grid
void makeGrid(int playerX, int playerY, int alienX, int alienY)
{
	for (int i = 0; i < gWidth; i++)
	{
		for (int j = 0; j < gWidth; j++)
		{
			node Node;
			Node.row = i;
			Node.col = j;
			
			Node.tRows = gWidth;
			grid[i][j] = Node;
			grid[i][j].type = mapEditor[i][j];
		}
	}

	grid[alienY][alienX].makeStart();
	closedSet[closedCount] = grid[alienY][alienX];
	startRow = alienY;
	startCol = alienX;


	closedSet[closedCount].makeClosed();
	closedCount++;
	grid[playerY][playerX].makeEnd();
	endRow = playerY;
	endCol = playerX;

}

//Draws grid to screen
void drawGrid()
{
	for (int i = 0; i < gWidth; i++)
	{
		cout << "\n";
		for (int j = 0; j < gWidth; j++)
		{
			cout << " " << grid[i][j].type;
		}
	}



}

int getPathRow(int count)
{

	return pathSet[count].row;

}

int getPathCol(int count)
{

	return pathSet[count].col;

}

int getPathcount()
{
	return pathCount;
}

void resetPathcount()
{
	pathCount = 0;
}

//Compares nodes in the open set, returns the one with the lowest f cost.
node compareOpen()
{
	
	node min;
	int minin = 0;
	min.fCost = 1000;
	for (int i = 0; i < openCount; i++)
	{
		openSet[i].hCost = h(openSet[i]);
		openSet[i].gCost = g(openSet[i]);
		openSet[i].fCost = f(openSet[i]);
		
		
	}

	for (int i = 0; i < openCount; i++)
	{
		if (openSet[i].fCost < min.fCost)
		{
			min = openSet[i];
			minin = i;
		}

	}

	int total = 576 - minin;

	for (minin; minin < total; minin++)
	{
		openSet[minin] = openSet[minin + 1];
		
	}
	openCount--;
	return min;



}

//Sequences the final path by tracing back the parents of the end node
void sequencePath(node end)
{
	node selected = end;

	while (selected.isStart() == false)
	{
		grid[selected.row][selected.col].makePath();
		pathSet[pathCount] = selected;
		pathCount++;

		selected = grid[selected.parentPos[0]][selected.parentPos[1]];
		

	}

}

//Updates the grid to match the map editor
void updateGrid(int playerX,int playerY)
{

	for (int r = 0; r < gWidth; r++)
	{
		for (int c = 0; c < gWidth; c++)
		{
			grid[r][c].type = mapEditor[r][c];
			//grid[r][c].hCost = 0;

		}

	}

	grid[playerY][playerX].makeStart();
	closedSet[closedCount] = grid[playerY][playerX];
	startRow = playerY;
	startCol = playerX;

}

void CalculatePath()
{	



	//Basic Setup


	
		//Algorithm implimentation
		for (int p = 0; p < closedCount; p++)
		{
			closedSet[p].updateNbors(grid);

			for (int g = 0; g < closedSet[p].nborsCount; g++)
			{
				//Checks if the end node is a neighbor / has been found
				if (grid[closedSet[p].nbors[g][0]][closedSet[p].nbors[g][1]].isEnd() == true)
				{
					cout << "\n\nTARGET HAS BEEN FOUND";
					sequencePath(grid[closedSet[p].nbors[g][0]][closedSet[p].nbors[g][1]]);
					closedCount = 0;
					break;

				}

				grid[closedSet[p].nbors[g][0]][closedSet[p].nbors[g][1]].makeOpen();


				//Add to open set
				openSet[openCount] = grid[closedSet[p].nbors[g][0]][closedSet[p].nbors[g][1]];
				openCount++;

			}
			//Add to closed set
			cout << "\nCC:" << closedCount;
			if (closedCount > 0)
			{
				closedSet[closedCount] = compareOpen();
				grid[closedSet[closedCount].row][closedSet[closedCount].col].makeClosed();
			}

			closedCount++;
			cout << "\n";
			
			
		}
		//drawGrid();
		closedCount = 0;
		openCount = 0;
		//pathCount = 0;

		//fill_n(closedSet, 576, 0);
		//fill_n(openSet, 576, 0);



}
