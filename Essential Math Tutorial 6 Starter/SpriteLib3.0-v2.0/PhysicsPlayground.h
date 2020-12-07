#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"

class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;
	int x128(int tileNum);
	void makeBox(int xSize, int ySize, float xPos, float yPos, float rotation);
	void thickCorner1(int xPos, int yPos, float rotation);
	void thickCorner(int xPos, int yPos, float rotation);
	void curvedCorner1(int xPos, int yPos, float rotation);
	void curvedCorner(int xPos, int yPos, float rotation);
	void smallOctogonRoom(int xPos, int yPos);
	void smallOctogonCorner(int xPos, int yPos, float rotation);
	void largeOctogonRoom(int xPos, int yPos, bool north, bool east, bool south, bool west);
	void thickDiagonalOctogon(int xPos, int yPos, float rotation);
	void thickStraightOctogon(int xPos, int yPos, float rotation);
	void thickishWall(int xPos, int yPos, float roation);
	void wall(int xPos, int yPos, float roation);
	void thickWall(int xPos, int yPos, float roation);
	void corridoor(int xPos, int yPos, float roation);
	void thickishDoor(int xPos, int yPos, float roation);
	void thickishDoorEdge(int xPos, int yPos, float roation);
	void thickDoor(int xPos, int yPos, float rotation);
	void thickDoorLeft(int xPos, int yPos, float rotation);
	void thickDoorRight(int xPos, int yPos, float rotation);
	void narrowCorridor(int xPos, int yPos, float rotation);
	void junction(int xPos, int yPos, float rotation);
	void corner(int xPos, int yPos, float rotation);
	void thickJunction(int xPos, int yPos, float rotation);
	void locker(int xPos, int yPos, float rotation);
	void lockerDX(int xPos, int yPos, float rotation);
	void lockerPolygon1(int xPos, int yPos, float rotation);
	void lockerPolygon2(int xPos, int yPos, float rotation);
	void thinThickCorridor(int xPos, int yPos, float rotation);
	void thinThickCorridor1(int xPos, int yPos, float rotation);
	void corridoorThinOpening(int xPos, int yPos, float rotation);
	void corridoorThinOpening1(int xPos, int yPos, float rotation);
	void corridoorThinOpening2(int xPos, int yPos, float rotation);
	void sharpCorner(int xPos, int yPos, float rotation);
	void gapPL(int xPos, int yPos, float rotation);
	void gapPR(int xPos, int yPos, float rotation);
	void mask(int xPos, int yPos, std::string FName);
	void sMask(int xPos, int yPos, std::string FName);
	void Gap(int xPos, int yPos, int rotation);
	void showDeathAni();
	void decoration(std::string FName, int xSize, int ySize, int xPos, int yPos, int rotation);
	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;
	void MouseMotion(SDL_MouseMotionEvent evnt) override;
	
	bool isMoving = false;
protected:
	PhysicsPlaygroundListener listener;

	int sScreen;
	int eScreen;

	bool hideStartScreen = false;
	bool showEndScreen = false;

	int ball = 0;
	int alien;
	int rayMarker;
	int alienSpr;
	int radar;
	int visionCone;
	int thing;
	
	int RCobj;
	int YCobj;
	int GCobj;
	int BCobj;

	bool hasRC = false;
	bool hasYC = false;
	bool hasGC = false;
	bool hasBC = false;

	bool rTA = false;
	bool yTA = false;
	bool gTA = false;
	bool bTA = false;

	bool finalDoorisOpen = false;

	int yd1;
	int yd1s;
	int yd2;
	int yd2s;
	int yd3;

	int gd1;
	int gd1s;
	int gd2;
	int gd2s;

	int bd1;
	int bd1s;
	int bd2;
	int bd2s;
	int bd3;
	int bd3s;
	int bd4;
	int bd5;

	int finDoor;
	int finDoorS;

	int dan1;
	int dan2;
	int dan3;
	int dan4;
	int dan5;
	int dan6;
	int dan7;
	int dan8;
	int dan9;

	bool showingDeathAni = false;

	int startTime;
	double timeDeff = 0;

	int aniClock;
	double aniClockDif = 0;

	int t = 4;

};