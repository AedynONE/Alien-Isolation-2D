#pragma once

#include "Scene.h"
//#include "Game.cpp"

class AnimationSpritePlayground : public Scene
{
public:
	AnimationSpritePlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;
	//void SwapScene(int newScene);

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;
	void swapScene(int newScene);

protected:

};
