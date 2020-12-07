#pragma once


void CalculatePath();
void makeGrid(int playerX, int playerY, int alienX, int alienY);
void updateGrid(int playerX, int playerY);

int getPathRow(int count);
int getPathCol(int count);
int getPathcount();
void resetPathcount();