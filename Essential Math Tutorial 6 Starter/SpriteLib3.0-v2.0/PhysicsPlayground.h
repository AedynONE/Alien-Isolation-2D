#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"

class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;
	void makeBox(int xSize, int ySize, float xPos, float yPos, float rotation);

	void smallOctogonRoom(int xPos, int yPos);
	void smallOctogonCorner(int xPos, int yPos, float rotation);
	void largeOctogonRoom(int xPos, int yPos, bool north, bool east, bool south, bool west);
	void thickDiagonalOctogon(int xPos, int yPos, float rotation);
	void thickStraightOctogon(int xPos, int yPos, float rotation);
	

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:
	PhysicsPlaygroundListener listener;

	int ball = 0;
	int alien;
	int rayMarker;
	int alienSpr;
	int radar;
	int visionCone;
};
