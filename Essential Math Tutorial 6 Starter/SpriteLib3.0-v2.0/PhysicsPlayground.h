#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"

class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;
	void makeBox(int xSize, int ySize, float xPos, float yPos, float rotation);
	void makePolygon(int xPos, int yPos, float rotation) {
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 128, 128);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(xPos, yPos, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.position.Set(float32(xPos), float32(yPos));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//Custom body
		// The program detects these points clockwise, so top of the triangle, bottom right, bottom left. box 2d is counter clockwise
		std::vector<b2Vec2> points = { b2Vec2(tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 2), b2Vec2(0.f,tempSpr.GetHeight() / 2.f),  b2Vec2(-tempSpr.GetWidth() / 2, -tempSpr.GetHeight() / 2.f) };
		tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(rotation);
	}

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:
	PhysicsPlaygroundListener listener;

	int ball = 0;
};
