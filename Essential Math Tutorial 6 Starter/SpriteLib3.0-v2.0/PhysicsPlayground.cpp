#include "PhysicsPlayground.h"
#include "Utilities.h"
#include "RayCastCallback.h"
#include <Box2d/Dynamics/b2Fixture.h>
#include "AlienAstar.h";

#pragma comment(lib, "winmm.lib")
using namespace std;
#include <random>

PhysicsPlayground::PhysicsPlayground(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
	
}

void PhysicsPlayground::InitScene(float windowWidth, float windowHeight)
{
	

	//Dynamically allocates the register
	m_sceneReg = new entt::registry;
	m_physicsWorld = new b2World(m_gravity);

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	aniClock = clock();

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;


	{
		auto entity = ECS::CreateEntity();
		sScreen = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "startScreen.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 535, 300);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3((0), (0), 100.f));
	}

	{
		auto entity = ECS::CreateEntity();
		eScreen = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 520, 298);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3((0), (0), 100.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}


	//Death Animation
	{
		{
			auto entity = ECS::CreateEntity();
			dan1 = entity;

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "Death/da1.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 520, 298);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3((0), (0), 100.f));
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}

		{
			auto entity = ECS::CreateEntity();
			dan2 = entity;

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "Death/da2.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 520, 298);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3((0), (0), 100.f));
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}

		{
			auto entity = ECS::CreateEntity();
			dan3 = entity;

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "Death/da3.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 520, 298);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3((0), (0), 100.f));
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}

		{
			auto entity = ECS::CreateEntity();
			dan4 = entity;

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "Death/da4.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 520, 298);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3((0), (0), 100.f));
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}

		{
			auto entity = ECS::CreateEntity();
			dan5 = entity;

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "Death/da5.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 520, 298);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3((0), (0), 100.f));
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}

		{
			auto entity = ECS::CreateEntity();
			dan6 = entity;

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "Death/da6.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 520, 298);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3((0), (0), 100.f));
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}

		{
			auto entity = ECS::CreateEntity();
			dan7 = entity;

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "Death/da7.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 520, 298);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3((0), (0), 100.f));
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}

		{
			auto entity = ECS::CreateEntity();
			dan8 = entity;

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "Death/da8.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 520, 298);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3((0), (0), 100.f));
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}

		{
			auto entity = ECS::CreateEntity();
			dan9 = entity;

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "Death/da9.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 520, 298);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3((0), (0), 100.f));
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}
	}

	decoration("Back facing bed.png", 64, 64, x128(0), -128, 0);
	decoration("bed.png", 64, 64, x128(0), 128, 0);
	decoration("Dead crewmate on a Stacter.png", 60, 60, x128(18), x128(-4) - 32, 0);
	decoration("Kitchen counter.png", 64, 55, x128(13) + 32, x128(-6) + 64, 0);
	decoration("Stacher.png", 60, 60, x128(18), x128(0), 0);
	decoration("Stacher.png", 60, 60, x128(18), x128(0)-30, 0);
	decoration("Stacher.png", 60, 60, x128(16), x128(0), 0);
	decoration("Stacher.png", 60, 60, x128(16), x128(0) - 30, 0);
	decoration("coffeemachine.png", 32, 64, x128(9) - 32, x128(-8) + 32, 0);
	decoration("Couch.png", 64, 64, x128(4) - 60, x128(4) + 64, 0);
	decoration("Couch.png", 64, 64, x128(2)+60, x128(4) + 64, 0);
	decoration("coffeemachine.png", 32, 64, x128(2) - 35, x128(4), 0);
	//decoration("Crate1.png", 40, 40, x128(2) - 35, x128(-5), 0);
	//decoration("Crate1.png", 40, 40, x128(2), x128(-5), 0);
	//decoration("Crate1.png", 40, 40, x128(2) - 35, x128(-5) + 35, 0);
	//decoration("Crate2.png", 40, 40, x128(4) + 15, x128(-3) + 64, 0);
	//decoration("Crate2.png", 40, 40, x128(4) + 35, x128(-3) + 35, 0);
	//decoration("Crate2.png", 40, 40, x128(4), x128(-3) + 35, 0);
	decoration("Egg1.png", 40, 40, x128(2) + 35, x128(8), 0);
	decoration("Egg2.png", 40, 40, x128(1) + 64, x128(6)+35, 0);
	decoration("Egg3.png", 40, 40, x128(4) + 35, x128(8), 0);
	decoration("Table.png", 64, 64, x128(10), x128(-6), 0);
	decoration("Table.png", 64, 64, x128(10), x128(-7), 0);
	decoration("Table.png", 64, 64, x128(10), x128(-8), 0);
	decoration("Table.png", 64, 64, x128(10), x128(-9), 0);

	//Doors
	{
		//Yellow
		{

			{
				int posx = 385;
				int posy = -305;

				{
					auto entity = ECS::CreateEntity();
					yd1s = entity;

					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);

					std::string fileName = "Doors/Yellow Door FC.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3((posx), (posy + 13), 6.f));
				}

				//Creates entity
				auto entity = ECS::CreateEntity();
				yd1 = entity;

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Doors/Yellow Door FC.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 0, 0);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(posx, posy, 6.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.position.Set(float32(posx), float32(posy));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(64), float(7), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 1.f, 1.f);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
				tempPhsBody.SetRotationAngleDeg(0);
			}

			{
				int posx = 384;
				int posy = 205;

				{
					auto entity = ECS::CreateEntity();
					yd2s = entity;

					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);

					std::string fileName = "Doors/Yellow Door FC.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3((posx), (posy + 13), 6.f));
				}

				//Creates entity
				auto entity = ECS::CreateEntity();
				yd2 = entity;

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Doors/Yellow Door FC.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 0, 0);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(posx, posy, 6.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.position.Set(float32(posx), float32(posy));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(64), float(7), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 1.f, 1.f);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
				tempPhsBody.SetRotationAngleDeg(0);
			}

			{
				int posx = -145;
				int posy = 10;

				//Creates entity
				auto entity = ECS::CreateEntity();
				yd3 = entity;

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Doors/Yellow Door S.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 7, 64);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(posx, posy, 6.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.position.Set(float32(posx), float32(posy));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 1.f, 1.f);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
				tempPhsBody.SetRotationAngleDeg(0);
			}
		}

		//Green
		{
			{
				int posx = 1280;
				int posy = -550;

				{
					auto entity = ECS::CreateEntity();
					gd1s = entity;

					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);

					std::string fileName = "Doors/Green Door FC.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3((posx), (posy + 13), 6.f));
				}

				//Creates entity
				auto entity = ECS::CreateEntity();
				gd1 = entity;

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Doors/Yellow Door FC.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 0, 0);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(posx, posy, 6.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.position.Set(float32(posx), float32(posy));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(64), float(7), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 1.f, 1.f);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
				tempPhsBody.SetRotationAngleDeg(0);
			}

			{
				int posx = 2176;
				int posy = 220;

				{
					auto entity = ECS::CreateEntity();
					gd2s = entity;

					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);

					std::string fileName = "Doors/Green Door FC.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3((posx), (posy + 13), 6.f));
				}

				//Creates entity
				auto entity = ECS::CreateEntity();
				gd2 = entity;

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Doors/Yellow Door FC.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 0, 0);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(posx, posy, 6.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.position.Set(float32(posx), float32(posy));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(64), float(7), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 1.f, 1.f);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
				tempPhsBody.SetRotationAngleDeg(0);
			}

		}

		//Blue
		{

			{
				int posx = 1666;
				int posy = -200;

				{
					auto entity = ECS::CreateEntity();
					bd1s = entity;

					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);

					std::string fileName = "Doors/Blue Door FC.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3((posx), (posy + 13), 6.f));
				}

				//Creates entity
				auto entity = ECS::CreateEntity();
				bd1 = entity;

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Doors/Blue Door FC.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 0, 0);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(posx, posy, 6.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.position.Set(float32(posx), float32(posy));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(64), float(7), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 1.f, 1.f);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
				tempPhsBody.SetRotationAngleDeg(0);
			}

			{
				int posx = 2176;
				int posy = 580;

				{
					auto entity = ECS::CreateEntity();
					bd2s = entity;

					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);

					std::string fileName = "Doors/Blue Door FC.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3((posx), (posy + 13), 6.f));
				}

				//Creates entity
				auto entity = ECS::CreateEntity();
				bd2 = entity;

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Doors/Blue Door FC.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 0, 0);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(posx, posy, 6.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.position.Set(float32(posx), float32(posy));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(64), float(7), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 1.f, 1.f);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
				tempPhsBody.SetRotationAngleDeg(0);
			}

			{
				int posx = 895;
				int posy = 465;

				{
					auto entity = ECS::CreateEntity();
					bd3s = entity;

					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);

					std::string fileName = "Doors/Blue Door FC.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3((posx), (posy + 13), 6.f));
				}

				//Creates entity
				auto entity = ECS::CreateEntity();
				bd3 = entity;

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Doors/Blue Door FC.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 0, 0);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(posx, posy, 6.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.position.Set(float32(posx), float32(posy));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(64), float(7), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 1.f, 1.f);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
				tempPhsBody.SetRotationAngleDeg(0);
			}

			{
				int posx = 1610;
				int posy = 1035;

				//Creates entity
				auto entity = ECS::CreateEntity();
				bd4 = entity;

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Doors/Blue Door S.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 7, 64);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(posx, posy, 6.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.position.Set(float32(posx), float32(posy));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 1.f, 1.f);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
				tempPhsBody.SetRotationAngleDeg(0);
			}

			{
				int posx = 1131;
				int posy = 650;

				//Creates entity
				auto entity = ECS::CreateEntity();
				bd5 = entity;

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Doors/Blue Door S.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 7, 64);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(posx, posy, 6.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.position.Set(float32(posx), float32(posy));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 1.f, 1.f);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
				tempPhsBody.SetRotationAngleDeg(0);
			}

		}

		//Final
		{
			{
				int posx = 1280;
				int posy = 1110;

				{
					auto entity = ECS::CreateEntity();
					finDoorS = entity;

					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);

					std::string fileName = "Doors/Final_Door.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 128, 96);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3((posx), (posy + 13), 6.f));
				}

				//Creates entity
				auto entity = ECS::CreateEntity();
				finDoor = entity;

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Doors/Final_Door.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 0, 0);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(posx, posy, 6.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.position.Set(float32(posx), float32(posy));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(128), float(7), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 1.f, 1.f);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
				tempPhsBody.SetRotationAngleDeg(0);
			}
		}
	}



	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		//vec4 temp = vec4(-(aspectRatio *100), (aspectRatio *100), -(aspectRatio *100), (aspectRatio *100));
		vec4 temp = vec4(-(150), (150), -(150), (150));
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		//ECS::GetComponent<Camera>(entity).SetFOV(vec2(300, 300));
	}



	//Setup raycast marker
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		rayMarker = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "BeachBall.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 5);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 4.f));
	}


	//Setup Vcone
	{


		auto entity = ECS::CreateEntity();
		visionCone = entity;


		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);


		//Sets up the components
		std::string fileName = "Cone.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 800, 800);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 4.f));


		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//Sphere body
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, OBJECTS, 0, 0.f, 0.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(false);
		tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(0.f);
	}


	//Setup background
	{

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);


		//Set up the components
		std::string fileName = "M1_L1A.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 4096.f, 4096.f);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1088, 192, 3.f));
	}

	//Setup background2
	{

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "M1_L2G.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 4096.f, 4096.f);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.8f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1088.f, 192.f, 20.f));
	}
	//Setup Overlay
	{

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "FullMap.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 4096.f, 4096.f);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.05f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1088.f, 192.f, 100.f));
	}

	//Setup Radar
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		radar = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "radar.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 5);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));
	}

	//Setup Alien Sprite
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		alienSpr = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "AlienIdle.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));
	}

	//Player entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Player>(entity);
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up the components
		std::string fileName = "spritesheets/Player.png";
		std::string animations = "Player.json";
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 32, 32, &ECS::GetComponent<Sprite>(entity),
			&ECS::GetComponent<AnimationController>(entity), &ECS::GetComponent<Transform>(entity));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 5.f));


		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 12.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(x128(10)), float32(x128(-7)));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//Sphere body
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 8.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(180.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(0.f);

	}

	//Alien entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		alien = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);

		//Sets up the components
		std::string fileName = "XenomorphRoughDraft.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 40);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 5.f));
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(x128(8),x128(1) );

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//Box body
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, -10.f), false, ENEMY, PLAYER | OBJECTS, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(0.f);
		//tempPhsBody.GetBody()->SetActive(false);

	}
	//Setup Corpse 1
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "corpse1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1660.f, 130.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 10.f;
		float shrinkY = 20.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1660.f), float32(130.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Corpse 2
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "corpse2.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1660.f, 130.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 10.f;
		float shrinkY = 20.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(858.f), float32(701.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Corpse 3
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "corpse3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1660.f, 130.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 10.f;
		float shrinkY = 20.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2281.f), float32(-479.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Corpse 4
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "corpse4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1660.f, 130.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 10.f;
		float shrinkY = 20.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(868.f), float32(-340.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Corpse 5
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "corpse5.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1660.f, 130.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 10.f;
		float shrinkY = 20.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1185.f), float32(1200.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Locker (Shower)
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "lockerfrontfacing.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 128);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1664.f, 735.f, 6.f));
	}
	//Setup Locker (Lounge to Storage Hallway)
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "lockersidefacing.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 128);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(427.f, 38.f, 6.f));
	}
	//Setup Locker (Storage Left)
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "lockersidefacing.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 128);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(299.f, -476.f, 6.f));
	}
	//Setup Locker (Storage Right)
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "lockersidefacing.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 128);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(470.f, -478.f, 6.f));
	}
	//Setup Locker (Green room top)
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "lockerfrontfacing.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 128);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1920.f, 480.f, 6.f));
	}
	//Setup Locker (Green room side)
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "lockersidefacing.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 128);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1878.f, 417.f, 6.f));
	}
	//Setup Locker (Green Room to Medbay Hallway)
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "lockersidefacing.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 128);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(2219.f, 162.f, 6.f));
	}
	//Setup Locker (Isolation)
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "lockersidefacing.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 128);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(2347.f, -351.f, 6.f));
	}
	//Setup Locker (Kitchen)
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "lockerfrontfacing.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 128);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1920.f, -672.f, 6.f));
	}
	//Setup Computer 1
	{		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "phatterminal.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 36);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1663.f, 156.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1661.f), float32(156.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Box 32x32 (freezer)
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "cardboardbox_32x32.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1663.f, 156.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 10.f;
		float shrinkY = 10.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1971.f), float32(-589.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Box 32x16 left of big box (freezer)
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "cardboardbox_32x16.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 16);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1663.f, 156.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 10.f;
		float shrinkY = 10.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1949.f), float32(-589.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Box 32x16 left corner in front of door (freezer)
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "cardboardbox_32x16.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 16);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1663.f, 156.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 10.f;
		float shrinkY = 10.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1738.f), float32(-589.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Box 32x32 top right corner (storage)
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "cardboardbox_32x32.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1654.f, 156.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 10.f;
		float shrinkY = 10.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(552.f), float32(-330.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Box 32x16 left of big box (storage)
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "cardboardbox_32x16.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 16);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1663.f, 156.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 10.f;
		float shrinkY = 10.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(530.f), float32(-334.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Box 32x16 top left corner (storage)
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "cardboardbox_32x16.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 16);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1663.f, 156.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 10.f;
		float shrinkY = 10.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(213.f), float32(-332.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Broom (Closet)
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "broom2.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-254.f, 71.f, 6.f));
	}
	//Corpse head
	{
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "corpse1head.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 18, 16);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1600.f, 120.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(1600.f), float32(120.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetWidth() - shrinkY) / 2.f), vec2(0.f, 0.f), false, OBJECTS, GROUND | ENVIRONMENT | PLAYER | TRIGGER, 0.3f);
		std::vector<b2Vec2> points = { b2Vec2(-tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f),b2Vec2(tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2.f, tempSpr.GetHeight() / 2.f),b2Vec2(-tempSpr.GetWidth() / 2.f,tempSpr.GetHeight() / 2.f) };
		tempPhsBody = PhysicsBody(entity, BodyType::CIRCLE, tempBody, points, vec2(0.f, 0.f), false, OBJECTS, GROUND | ENEMY | PLAYER, 0.5f, 3.f);

		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(0.f);

	}
	//Setup Yellow Terminal
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "yellowterminal.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(256.f, 78.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(256.f), float32(78.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Blue Terminal
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "blueterminal.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(256.f, 78.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(2016.f), float32(542.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Red Terminal
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "redterminal.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(256.f, 78.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(897.f), float32(825.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Green Terminal
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "greenterminal.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(256.f, 78.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1022.f), float32(260.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Yellow Keycard
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		YCobj = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "keycards_yellow.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 16, 16);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(292.f, -124.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(292.f), float32(-124.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Blue Keycard
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		BCobj = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "keycards_blue.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 16, 16);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(380.f, 400.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(380.f), float32(400.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Red Keycard
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		RCobj = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "keycards_red.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 16, 16);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(292.f, -124.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1787.f), float32(598.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Green Keycard
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		GCobj = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "keycards_green.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 16, 16);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(292.f, -124.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1620.f), float32(-763.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//largeOctogonRoom(0, 0, true, true, true, true);
	//smallOctogonRoom(0, 0);

	//Sounds
	PlaySound(TEXT("assets/sounds/09_Alone.wav"), NULL, SND_FILENAME | SND_ASYNC);

	/*

	// This is just a list of the tiles
	//largeOctogonRoom(0, 0, true, true, true, true);
	//smallOctogonRoom(0, 0);
	//thickCorner(0, 0, 0);
	//curvedCorner(0, 0, 0);
	//thickDiagonalOctogon(0, 0, 0);
	//thickishWall(0, 0, 0);
	//thickWall(0, 0, 0);
	//makeBox(128, 128, 0, 0, 0);
	//wall(0,0,180);
	//corridoor(0,0,0);
	//thickishDoor(0, 0, 90);
	//narrowCorridor(0, 0, 90);
	//thickDoor(0, 0, 90);
	//junction(0, 0, 0);
	//locker(0, 0, 0);
	//thinThickCorridor(0, 0, 0);
	//corridoorThinOpening(0, 0, 0);
	//sharpCorner(0, 0, 0);


	*/

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

int PhysicsPlayground::x128(int tileNum) {
	return tileNum * 128;
}
void PhysicsPlayground::makeBox(int xSize, int ySize, float xPos, float yPos, float rotation)
{
	//Creates entity
	auto entity = ECS::CreateEntity();
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/solid wall.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, xSize, ySize);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xPos, yPos, 2.f));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.position.Set(float32(xPos), float32(yPos));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 1.f, 100.f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::smallOctogonCorner(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/thick junction corner.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
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
	std::vector<b2Vec2> points = {
		b2Vec2(tempSpr.GetWidth() / 2.f,tempSpr.GetHeight() / 2),
		b2Vec2(0,tempSpr.GetHeight() / 2),
		b2Vec2(tempSpr.GetWidth() / 2.f,0)
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::smallOctogonRoom(int xPos, int yPos) {
	smallOctogonCorner(xPos + 32, yPos + 32, 0);
	smallOctogonCorner(xPos + 32, yPos + -32, 270);
	smallOctogonCorner(xPos + -32, yPos + 32, 90);
	smallOctogonCorner(xPos + -32, yPos + -32, 180);
}
void PhysicsPlayground::thickCorner1(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/thickish corner bottom left.png";
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
	std::vector<b2Vec2> points = {
		b2Vec2(-tempSpr.GetWidth() / 2.f,tempSpr.GetHeight() / 2),
		b2Vec2(-tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 2),
		b2Vec2(tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 2),
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::thickCorner(int xPos, int yPos, float rotation) {
	if (rotation == 0) {
		thickCorner1(xPos, yPos, rotation);
		makeBox(32, 32, xPos - 48, yPos + 48, 0);
		makeBox(32, 32, xPos + 48, yPos - 48, 0);
	}
	if (rotation == 90) {
		thickCorner1(xPos, yPos, rotation);
		makeBox(32, 32, xPos + 48, yPos + 48, 0);
		makeBox(32, 32, xPos - 48, yPos - 48, 0);
	}
	if (rotation == 180) {
		thickCorner1(xPos, yPos, rotation);
		makeBox(32, 32, xPos - 48, yPos + 48, 0);
		makeBox(32, 32, xPos + 48, yPos - 48, 0);
	}
	if (rotation == 270) {
		thickCorner1(xPos, yPos, rotation);
		makeBox(32, 32, xPos + 48, yPos + 48, 0);
		makeBox(32, 32, xPos - 48, yPos - 48, 0);
	}
}
void PhysicsPlayground::curvedCorner1(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/curved corner bottom left.png";
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
	std::vector<b2Vec2> points = {
		b2Vec2(-tempSpr.GetWidth() / 2.f,tempSpr.GetHeight() / 4),
		b2Vec2(-tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 2),
		b2Vec2(tempSpr.GetWidth() / 4.f,-tempSpr.GetHeight() / 2),
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::curvedCorner(int xPos, int yPos, float rotation) {
	if (rotation == 0) {
		curvedCorner1(xPos, yPos, rotation);
		makeBox(11, 50, xPos - 59, yPos + 40, 0);
		makeBox(50, 11, xPos + 40, yPos - 59, 0);
	}
	if (rotation == 90) {
		curvedCorner1(xPos, yPos, rotation);
		makeBox(11, 50, xPos + 59, yPos + 40, 0);
		makeBox(50, 11, xPos - 40, yPos - 59, 0);
	}
	if (rotation == 180) {
		curvedCorner1(xPos, yPos, rotation);
		makeBox(11, 50, xPos + 59, yPos - 40, 0);
		makeBox(50, 11, xPos - 40, yPos + 59, 0);
	}
	if (rotation == 270) {
		curvedCorner1(xPos, yPos, rotation);
		makeBox(11, 50, xPos - 59, yPos - 40, 0);
		makeBox(50, 11, xPos + 40, yPos + 59, 0);
	}
}

void PhysicsPlayground::thickDiagonalOctogon(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();
	thing = entity;
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/thick corner bottom left.png";
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
	std::vector<b2Vec2> points = {
		b2Vec2(-tempSpr.GetWidth() / 2.f,tempSpr.GetHeight() / 2),
		b2Vec2(-tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 2),
		b2Vec2(tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 2),
		b2Vec2(tempSpr.GetWidth() / 2.f, 0.f),
		b2Vec2(0.f,tempSpr.GetHeight() / 2)
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::thickStraightOctogon(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/thick wall left.png";
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
	std::vector<b2Vec2> points = {
		b2Vec2(-tempSpr.GetWidth() / 2.f,tempSpr.GetHeight() / 2),
		b2Vec2(-tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 2),
		b2Vec2(0,-tempSpr.GetHeight() / 2),
		b2Vec2(0,tempSpr.GetHeight() / 2)
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::largeOctogonRoom(int xPos, int yPos, bool north, bool east, bool south, bool west) {
	if (north == true) {
		thickStraightOctogon(xPos + 0, yPos + 128, 270);
	}
	if (east == true) {
		thickStraightOctogon(xPos + 128, yPos + 0, 180);
	}
	if (south == true) {
		thickStraightOctogon(xPos + 0, yPos + -128, 90);
	}
	if (west == true) {
		thickStraightOctogon(xPos + -128, yPos + 0, 0);
	}
	thickDiagonalOctogon(xPos + 128, yPos + 128, 180);
	thickDiagonalOctogon(xPos + 128, yPos + -128, 90);
	thickDiagonalOctogon(xPos + -128, yPos + -128, 0);
	thickDiagonalOctogon(xPos + -128, yPos + 128, 270);
}
void PhysicsPlayground::thickishWall(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/thickish wall left.png";
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
	std::vector<b2Vec2> points = {
		b2Vec2(-tempSpr.GetWidth() / 2.f,tempSpr.GetHeight() / 2),
		b2Vec2(-tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 2),
		b2Vec2(-tempSpr.GetWidth() / 4.f,-tempSpr.GetHeight() / 2),
		b2Vec2(-tempSpr.GetWidth() / 4.f,tempSpr.GetHeight() / 2),
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::thickWall(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/thick wall left.png";
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
	std::vector<b2Vec2> points = {
		b2Vec2(-tempSpr.GetWidth() / 2.f,tempSpr.GetHeight() / 2),
		b2Vec2(-tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 2),
		b2Vec2(0,-tempSpr.GetHeight() / 2),
		b2Vec2(0, tempSpr.GetHeight() / 2),
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::wall(int xPos, int yPos, float rotation) {
	if (rotation == 0) {
		makeBox(11, 128, xPos - 58.5, yPos, 0);
	}
	if (rotation == 90) {
		makeBox(128, 11, xPos, yPos - 58.5, 0);
	}
	if (rotation == 180) {
		makeBox(11, 128, xPos + 58.5, yPos, 0);
	}
	if (rotation == 270) {
		makeBox(128, 11, xPos, yPos + 58.5, 0);
	}
}
void PhysicsPlayground::corridoor(int xPos, int yPos, float rotation) {
	if (rotation == 0) {
		makeBox(11, 128, xPos - 58.5, yPos, 0);
		makeBox(11, 128, xPos + 58.5, yPos, 0);
	}
	if (rotation == 90) {
		makeBox(128, 11, xPos, yPos - 58.5, 0);
		makeBox(128, 11, xPos, yPos + 58.5, 0);
	}
	if (rotation == 180) {
		makeBox(11, 128, xPos + 58.5, yPos, 0);
		makeBox(11, 128, xPos - 58.5, yPos, 0);
	}
	if (rotation == 270) {
		makeBox(128, 11, xPos, yPos - 58.5, 0);
		makeBox(128, 11, xPos, yPos + 58.5, 0);
	}
}
void PhysicsPlayground::thickishDoor(int xPos, int yPos, float rotation) {
	thickishDoorEdge(xPos, yPos, rotation);
	thickishDoorEdge(xPos, yPos, rotation + 90);
}
void PhysicsPlayground::thickishDoorEdge(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/thickish door down edge.png";
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
	std::vector<b2Vec2> points = {
		b2Vec2(-tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 4),
		b2Vec2(-tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 2),
		b2Vec2(-tempSpr.GetWidth() / 4.f,-tempSpr.GetHeight() / 2),
		b2Vec2(-tempSpr.GetWidth() / 4.f,-tempSpr.GetHeight() / 3),
		b2Vec2(-tempSpr.GetWidth() / 3.f,-tempSpr.GetHeight() / 4),
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::thickDoor(int xPos, int yPos, float rotation) {
	thickDoorLeft(xPos, yPos, rotation);
	thickDoorRight(xPos, yPos, rotation);
}
void PhysicsPlayground::thickDoorLeft(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/thick door down.png";
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
	std::vector<b2Vec2> points = {
		b2Vec2(-tempSpr.GetWidth() / 2.f, 0),
		b2Vec2(-tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 2),
		b2Vec2(-tempSpr.GetWidth() / 4.f,-tempSpr.GetHeight() / 2),
		b2Vec2(-tempSpr.GetWidth() / 4.f,-tempSpr.GetHeight() / 8),
		b2Vec2(-tempSpr.GetWidth() / 3.f, 0)
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::thickDoorRight(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/thick door down.png";
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
	std::vector<b2Vec2> points = {
		b2Vec2(tempSpr.GetWidth() / 2.f, 0),
		b2Vec2(tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 2),
		b2Vec2(tempSpr.GetWidth() / 4.f,-tempSpr.GetHeight() / 2),
		b2Vec2(tempSpr.GetWidth() / 4.f,-tempSpr.GetHeight() / 8),
		b2Vec2(tempSpr.GetWidth() / 3.f, 0)
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::narrowCorridor(int xPos, int yPos, float rotation) {
	if (rotation == 0) {
		makeBox(32, 128, xPos - 48, yPos, 0);
		makeBox(32, 128, xPos + 48, yPos, 0);
	}
	if (rotation == 90) {
		makeBox(128, 32, xPos, yPos - 48, 0);
		makeBox(128, 32, xPos, yPos + 48, 0);
	}
}
void PhysicsPlayground::junction(int xPos, int yPos, float rotation) {
	corner(xPos, yPos, 0);
	corner(xPos, yPos, 90);
	corner(xPos, yPos, 180);
	corner(xPos, yPos, 270);
}
void PhysicsPlayground::corner(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/junction.png";
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
	std::vector<b2Vec2> points = {
		b2Vec2(-tempSpr.GetWidth() / 2,tempSpr.GetHeight() / 2),
		b2Vec2(-tempSpr.GetWidth() / 2,tempSpr.GetHeight() / 2.5),
		b2Vec2(-tempSpr.GetWidth() / 2.5,tempSpr.GetHeight() / 2)
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::locker(int xPos, int yPos, float rotation) {
	if (rotation == 0) {
		makeBox(11, 128, xPos + 58.5, yPos, 0);
		makeBox(11, 128, xPos - 58.5, yPos, 0);
		lockerPolygon1(xPos, yPos, rotation);
		lockerPolygon2(xPos, yPos, rotation);
	}
	if (rotation == 90) {
		makeBox(128, 11, xPos, yPos - 58.5, 0);
		makeBox(128, 11, xPos, yPos + 58.5, 0);
		lockerPolygon1(xPos, yPos, rotation);
		lockerPolygon2(xPos, yPos, rotation);
	}
	if (rotation == 180) {
		makeBox(11, 128, xPos + 58.5, yPos, 0);
		makeBox(11, 128, xPos - 58.5, yPos, 0);
		lockerPolygon1(xPos, yPos, rotation);
		lockerPolygon2(xPos, yPos, rotation);
	}
	if (rotation == 270) {
		makeBox(128, 11, xPos, yPos - 58.5, 0);
		makeBox(128, 11, xPos, yPos + 58.5, 0);
		lockerPolygon1(xPos, yPos, rotation);
		lockerPolygon2(xPos, yPos, rotation);
	}
}
void PhysicsPlayground::lockerDX(int xPos, int yPos, float rotation) {
	if (rotation == 0) {
		makeBox(11, 128, xPos - 58.5, yPos, 0);
		lockerPolygon1(xPos, yPos, rotation);
		lockerPolygon2(xPos, yPos, rotation);
	}
	if (rotation == 90) {
		makeBox(128, 11, xPos, yPos - 58.5, 0);
		lockerPolygon1(xPos, yPos, rotation);
		lockerPolygon2(xPos, yPos, rotation);
	}
	if (rotation == 180) {
		makeBox(11, 128, xPos + 58.5, yPos, 0);
		lockerPolygon1(xPos, yPos, rotation);
		lockerPolygon2(xPos, yPos, rotation);
	}
	if (rotation == 270) {
		makeBox(128, 11, xPos, yPos + 58.5, 0);
		lockerPolygon1(xPos, yPos, rotation);
		lockerPolygon2(xPos, yPos, rotation);
	}
}
void PhysicsPlayground::lockerPolygon1(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/locker left.png";
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
	std::vector<b2Vec2> points = {
		b2Vec2(-tempSpr.GetWidth() / 2.f, tempSpr.GetHeight() / 2),
		b2Vec2(-tempSpr.GetWidth() / 2.f, tempSpr.GetHeight() / 4),
		b2Vec2(-tempSpr.GetWidth() / 4.f, tempSpr.GetHeight() / 4),
		b2Vec2(-tempSpr.GetWidth() / 2.5, tempSpr.GetHeight() / 2)
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::lockerPolygon2(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/locker left.png";
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
	std::vector<b2Vec2> points = {
		b2Vec2(-tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2),
		b2Vec2(-tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 4),
		b2Vec2(-tempSpr.GetWidth() / 4.f, -tempSpr.GetHeight() / 4),
		b2Vec2(-tempSpr.GetWidth() / 2.5, -tempSpr.GetHeight() / 2)
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::thinThickCorridor(int xPos, int yPos, float rotation) {
	if (rotation == 0) {
		thinThickCorridor1(xPos - 32, yPos + 32, rotation);
		thinThickCorridor1(xPos - 32, yPos - 32, rotation + 90);
		makeBox(128, 11, xPos, yPos - 58.5, 0);
		makeBox(128, 11, xPos, yPos + 58.5, 0);
	}
	if (rotation == 90) {
		thinThickCorridor1(xPos - 32, yPos - 32, rotation);
		thinThickCorridor1(xPos + 32, yPos - 32, rotation + 90);
		makeBox(11, 128, xPos - 58.5, yPos, 0);
		makeBox(11, 128, xPos + 58.5, yPos, 0);
	}
	if (rotation == 180) {
		thinThickCorridor1(xPos + 32, yPos - 32, rotation);
		thinThickCorridor1(xPos + 32, yPos + 32, rotation + 90);
		makeBox(128, 11, xPos, yPos - 58.5, 0);
		makeBox(128, 11, xPos, yPos + 58.5, 0);
	}
	if (rotation == 270) {
		thinThickCorridor1(xPos + 32, yPos + 32, rotation);
		thinThickCorridor1(xPos - 32, yPos + 32, rotation + 90);
		makeBox(11, 128, xPos + 58.5, yPos, 0);
		makeBox(11, 128, xPos - 58.5, yPos, 0);
	}
}
void PhysicsPlayground::thinThickCorridor1(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/Random triangle.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
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
	std::vector<b2Vec2> points = {
		b2Vec2(-tempSpr.GetWidth() / 2.f, tempSpr.GetWidth() / 2),
		b2Vec2(-tempSpr.GetWidth() / 2.f,tempSpr.GetHeight() / 4),
		b2Vec2(-tempSpr.GetWidth() / 4.f,tempSpr.GetHeight() / 2),
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::corridoorThinOpening(int xPos, int yPos, float rotation) {
	if (rotation == 0) {
		corridoorThinOpening1(xPos, yPos, rotation);
		corridoorThinOpening2(xPos, yPos, rotation);
		makeBox(128, 11, xPos, yPos - 58.5, 0);
		makeBox(128, 11, xPos, yPos + 58.5, 0);
	}
	if (rotation == 90) {
		corridoorThinOpening1(xPos, yPos, rotation);
		corridoorThinOpening2(xPos, yPos, rotation);
		makeBox(11, 128, xPos - 58.5, yPos, 0);
		makeBox(11, 128, xPos + 58.5, yPos, 0);
	}
	if (rotation == 180) {
		corridoorThinOpening1(xPos, yPos, rotation);
		corridoorThinOpening2(xPos, yPos, rotation);
		makeBox(128, 11, xPos, yPos - 58.5, 0);
		makeBox(128, 11, xPos, yPos + 58.5, 0);
	}
	if (rotation == 270) {
		corridoorThinOpening1(xPos, yPos, rotation);
		corridoorThinOpening2(xPos, yPos, rotation);
		makeBox(11, 128, xPos + 58.5, yPos, 0);
		makeBox(11, 128, xPos - 58.5, yPos, 0);
	}
}
void PhysicsPlayground::corridoorThinOpening1(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/corridoor left.png";
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
	std::vector<b2Vec2> points = {
		b2Vec2(-tempSpr.GetWidth() / 2.f, tempSpr.GetWidth() / 2),
		b2Vec2(-tempSpr.GetWidth() / 2.f,tempSpr.GetHeight() / 4),
		b2Vec2(-tempSpr.GetWidth() / 2.2,tempSpr.GetHeight() / 4),
		b2Vec2(-tempSpr.GetWidth() / 2.5,tempSpr.GetHeight() / 3.5),
		b2Vec2(-tempSpr.GetWidth() / 2.5,tempSpr.GetHeight() / 2),
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::corridoorThinOpening2(int xPos, int yPos, float rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "Isolation 2D/corridoor left.png";
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
	std::vector<b2Vec2> points = {
		b2Vec2(-tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 2),
		b2Vec2(-tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 4),
		b2Vec2(-tempSpr.GetWidth() / 2.2,-tempSpr.GetHeight() / 4),
		b2Vec2(-tempSpr.GetWidth() / 2.5,-tempSpr.GetHeight() / 3.5),
		b2Vec2(-tempSpr.GetWidth() / 2.5,-tempSpr.GetHeight() / 2),
	};
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);

	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}
void PhysicsPlayground::sharpCorner(int xPos, int yPos, float rotation) {
	if (rotation == 0) {
		makeBox(12, 12, xPos - 58.5, yPos - 58.5, 0);
		makeBox(128, 8, xPos, yPos + 60, 0);
		makeBox(8, 128, xPos + 60, yPos, 0);
	}
	else if (rotation == 90) {
		makeBox(8, 8, xPos - 58.5, yPos + 58.5, 0);
		makeBox(128, 8, xPos, yPos - 60, 0);
		makeBox(8, 128, xPos + 60, yPos, 0);
	}
	else if (rotation == 180) {
		makeBox(8, 8, xPos + 58.5, yPos + 58.5, 0);
		makeBox(128, 8, xPos, yPos - 60, 0);
		makeBox(8, 128, xPos - 60, yPos, 0);
	}
	else if (rotation == 270) {
		makeBox(8, 8, xPos + 59, yPos - 59, 0);
		makeBox(128, 8, xPos, yPos + 60, 0);
		makeBox(8, 128, xPos - 60, yPos, 0);
	}

}
void PhysicsPlayground::gapPL(int xPos, int yPos, float rotation) {
	{
		//Creates entity 
		auto entity = ECS::CreateEntity();

		//Add components 
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components 
		std::string fileName = "";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 0, 0);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(xPos, yPos, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.position.Set(float32(xPos - 45), float32(yPos - 45));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//Custom body 
		// The program detects these points clockwise, so top of the triangle, bottom right, bottom left. box 2d is counter clockwise 
		std::vector<b2Vec2> points = {
			b2Vec2(-5,12),
			b2Vec2(-5,0),
			b2Vec2(28,0),
			b2Vec2(20,12)
		};

		tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(rotation);
	}
}
void PhysicsPlayground::gapPR(int xPos, int yPos, float rotation) {
	{
		//Creates entity 
		auto entity = ECS::CreateEntity();

		//Add components 
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components 
		std::string fileName = "";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 0, 0);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(xPos, yPos, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.position.Set(float32(xPos - 45), float32(yPos - 45));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//Custom body 
		// The program detects these points clockwise, so top of the triangle, bottom right, bottom left. box 2d is counter clockwise 
		std::vector<b2Vec2> points = {
			b2Vec2(0,0),
			b2Vec2(28,0),
			b2Vec2(28,12),
			b2Vec2(12,12)
		};

		tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(rotation);
	}
}
void PhysicsPlayground::mask(int xPos, int yPos, std::string FName) {
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = FName;
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 128, 128);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3((xPos), (yPos), 2.f));
	}
}
void PhysicsPlayground::sMask(int xPos, int yPos, std::string FName) {
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = FName;
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3((xPos), (yPos), 2.f));
	}
}
void PhysicsPlayground::Gap(int xPos, int yPos, int rotation) {
	if (rotation == 0) {
		mask(xPos - 35, yPos + 20, "Isolation 2D/gap bottom.png");
		gapPR(xPos + 42, yPos, 0);
		gapPL(xPos - 50, yPos, 0);
	}
	else if (rotation == 180) {
		mask(xPos - 65, yPos - 108, "Isolation 2D/gap top.png");
		gapPR(xPos - 56, yPos, 180);
		gapPL(xPos + 41, yPos, 180);
	}
	else if (rotation == 270) {
		mask(xPos - 65, yPos - 108, "Isolation 2D/gap left.png");
		{
			//Creates entity 
			auto entity = ECS::CreateEntity();

			//Add components 
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components 
			std::string fileName = "";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 0, 0);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3((xPos), (yPos), 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.position.Set(float32(xPos - 130), float32(yPos - 168));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			//Custom body 
			// The program detects these points clockwise, so top of the triangle, bottom right, bottom left. box 2d is counter clockwise 
			std::vector<b2Vec2> points = {
				b2Vec2(0,28),
				b2Vec2(0,0),
				b2Vec2(12,0),
				b2Vec2(12,18)
			};
			tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		}
		{
			//Creates entity 
			auto entity = ECS::CreateEntity();

			//Add components 
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components 
			std::string fileName = "";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 0, 0);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3((xPos), (yPos), 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.position.Set(float32(xPos - 130), float32(yPos - 82));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			//Custom body 
			// The program detects these points clockwise, so top of the triangle, bottom right, bottom left. box 2d is counter clockwise 
			std::vector<b2Vec2> points = {
				b2Vec2(0,32),
				b2Vec2(0,5),
				b2Vec2(12,18),
				b2Vec2(12,32)
			};
			tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		}
	}
	else if (rotation == 90) {
		mask(xPos - 65, yPos - 108, "Isolation 2D/gap right.png");
		{
			//Creates entity 
			auto entity = ECS::CreateEntity();

			//Add components 
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components 
			std::string fileName = "";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 0, 0);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3((xPos), (yPos), 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.position.Set(float32(xPos - 0), float32(yPos - 48));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			//Custom body 
			// The program detects these points clockwise, so top of the triangle, bottom right, bottom left. box 2d is counter clockwise 
			std::vector<b2Vec2> points = {
				b2Vec2(0,28),
				b2Vec2(0,0),
				b2Vec2(12,0),
				b2Vec2(12,18)
			};
			tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
			tempPhsBody.SetRotationAngleDeg(180);
		}
		{
			//Creates entity 
			auto entity = ECS::CreateEntity();

			//Add components 
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components 
			std::string fileName = "";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 0, 0);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3((xPos), (yPos), 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.position.Set(float32(xPos - 0), float32(yPos - 134));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			//Custom body 
			// The program detects these points clockwise, so top of the triangle, bottom right, bottom left. box 2d is counter clockwise 
			std::vector<b2Vec2> points = {
				b2Vec2(0,32),
				b2Vec2(0,5),
				b2Vec2(12,18),
				b2Vec2(12,32)
			};
			tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 100.f);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
			tempPhsBody.SetRotationAngleDeg(180);
		}
	}
}
void PhysicsPlayground::decoration(std::string FName, int xSize, int ySize, int xPos, int yPos, int rotation) {
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = FName;
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, xSize, ySize);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xPos, yPos, 4.f));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.position.Set(float32(xPos), float32(yPos));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 1.f, 1.f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}

float tarX, tarY;
//IMPORTANT VARIABLES
const float AlienSpeed = 3000.f;
const float AlienRetention = 8.f;
bool created = false;
float alienTileTimer = 1.f;
int alienSteps = 0;
void UI(int radar, int alien)
{
	//This function is used to display and position the game's UI.
	auto& cam = ECS::GetComponent<Camera>(MainEntities::MainCamera());
	auto& rad = ECS::GetComponent<Transform>(radar);
	auto& radSpr = ECS::GetComponent<Sprite>(radar);
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);


	//Displays the Radar UI.
	rad.SetPosition(cam.GetPosition().x, cam.GetPosition().y - 60, 100);

	//Calculations For Radar Transparency 
	b2Vec2 direction = (b2Vec2(player.GetBody()->GetPosition().x, player.GetBody()->GetPosition().y) - b2Vec2(ali.GetBody()->GetPosition().x, ali.GetBody()->GetPosition().y));
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
	radSpr.SetTransparency(1 / (distance / 150));


}

void MoveTo(int alien)
{
	//This function is always called every frame, and moves the alien in the direction of the target set by other functions
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	b2Vec2 direction = (b2Vec2(tarX, tarY) - ali.GetBody()->GetPosition());
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
	direction = b2Vec2(direction.x / distance, direction.y / distance);

	ali.GetBody()->SetLinearVelocity(b2Vec2(direction.x * Timer::deltaTime * AlienSpeed, direction.y * Timer::deltaTime * AlienSpeed));

}



void FindNextPosition(int alien)
{
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	if ((int)ali.GetBody()->GetPosition().x == (int)tarX && (int)ali.GetBody()->GetPosition().y == (int)tarY)
	{
		tarX = player.GetBody()->GetPosition().x;
		tarY = player.GetBody()->GetPosition().y;
	}

	if ((int)ali.GetBody()->GetLinearVelocity().x == 0 && (int)ali.GetBody()->GetLinearVelocity().y == 0)
	{
		tarX = player.GetBody()->GetPosition().x;
		tarY = player.GetBody()->GetPosition().y;
	}
}

int aCounter = 0;
int nCounter = 0;
float stuckCounter = 0;
float alienRetentionTimer = 0;
int newWay = 0;
bool found = false;
bool Search(int alien, int rayMarker, b2World* m_physicsWorld)
{

	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	auto& rayM = ECS::GetComponent<Transform>(rayMarker);


	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	RayCastCallback fuck;
	m_physicsWorld->RayCast(&fuck, ali.GetBody()->GetWorldPoint(b2Vec2(0, 0)), player.GetBody()->GetWorldPoint(b2Vec2(0, 0)));



	if (fuck.m_fixture)
	{


		if (fuck.m_fixture->GetBody() == player.GetBody())
		{

			return true;

		}
		else
		{
			return false;

		}
	}
}


void Chase(int alien, b2World* m_physicsWorld,b2Vec2 tar )
{
	//This Function is simply used make the alien move in the direction of the player
	newWay = rand() % 2;
	stuckCounter = 0;
	aCounter = 0;
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	//auto& player = ECS::GetComponent<PhysicsBody>(tEnt);



	RayCastCallback toPlayer;
	m_physicsWorld->RayCast(&toPlayer, ali.GetBody()->GetWorldPoint(b2Vec2(0, 0)),tar);


	tarX = tar.x;
	tarY = tar.y;
}

void Dodge(int alien, b2World* m_physicsWorld,b2Vec2 tar)
{
	//This Function is used to dodge obstacles between the alien and player
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	//auto& player = ECS::GetComponent<PhysicsBody>(tEnt);
	RayCastCallback dodgeRay;
	int maxRayIndex = 0;
	float adjustX = 1;
	float adjustY = 1;

	float maxDist = -10000;

	b2Vec2 newDirections[16] = { b2Vec2(0,-1),b2Vec2(0,1),b2Vec2(-1,0),b2Vec2(1,0),b2Vec2(1,1),b2Vec2(1,-1),b2Vec2(-1,-1),b2Vec2(-1,1),b2Vec2(0.5f,-1),b2Vec2(0.5f,1),b2Vec2(-1,0.5f),b2Vec2(1,0.5f),b2Vec2(0.5f,0.5f),b2Vec2(0.5f,-0.5f),b2Vec2(-0.5f,-0.5f),b2Vec2(-0.5f,0.5f) };
	RayCastCallback targetRay;

	//Shoots Rays in 16 directions away from the alien and finds the longest ray.
	for (int i = 0; i < 16; i++)
	{
		m_physicsWorld->RayCast(&dodgeRay, ali.GetBody()->GetWorldPoint(b2Vec2(0, 0)), ali.GetBody()->GetWorldPoint(b2Vec2(newDirections[i].x * 10000, newDirections[i].y * 10000)));
		b2Vec2 direction = (b2Vec2(dodgeRay.m_point.x, dodgeRay.m_point.y) - b2Vec2(ali.GetBody()->GetPosition().x, ali.GetBody()->GetPosition().y));
		float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

		//b2Vec2 direction2 = (b2Vec2(player.GetBody()->GetPosition().x, player.GetBody()->GetPosition().y) - b2Vec2(dodgeRay.m_point.x, dodgeRay.m_point.y));
		//float distance2 = sqrt(direction2.x * direction2.x + direction2.y * direction2.y);
		//float distance3 = distance - distance2;
		if (distance > maxDist)
		{
			maxDist = distance;
			maxRayIndex = i;
		}


	}

	//This keeps track of how long the alien is stuck in place. (IS CURRENTLY UN-USED)
	if (ali.GetBody()->GetLinearVelocity().x < 5 && ali.GetBody()->GetLinearVelocity().y < 5)
	{
		//cout << "\nSTUCK!!";
		adjustX = 1;
		adjustY = 1;
		stuckCounter += 1 * Timer::deltaTime;
		if (stuckCounter > 0.5)
		{
			//nCounter = 500;
		}
	}
	else
	{

		//adjustX = 0;
		//adjustY = 0;
	}

	//This code gets set off if the alien is stuck in place for too long (CURRENTLY UN-USED)
	if (nCounter >= 1)
	{
		//cout << "Correcting.";
		Chase(alien, m_physicsWorld, tar);
		nCounter -= 1 * Timer::deltaTime;
	}


	else
	{
		RayCastCallback toPlayer;
		m_physicsWorld->RayCast(&targetRay, ali.GetBody()->GetWorldPoint(b2Vec2(0, 0)), ali.GetBody()->GetWorldPoint(b2Vec2(newDirections[maxRayIndex].x * 10000, newDirections[maxRayIndex].y * 10000)));
		m_physicsWorld->RayCast(&toPlayer, ali.GetBody()->GetWorldPoint(b2Vec2(0, 0)), tar);

		b2Vec2 direction4 = (b2Vec2(toPlayer.m_point.x, toPlayer.m_point.y) - b2Vec2(ali.GetBody()->GetPosition().x, ali.GetBody()->GetPosition().y));
		float distance4 = sqrt(direction4.x * direction4.x + direction4.y * direction4.y);

		if (distance4 < 25)
		{
			if (newWay == 0)
			{
				//Clockwise Dodging
				ali.GetBody()->SetAngularVelocity(-200.f * Timer::deltaTime * adjustX);
				tarX = tar.x + targetRay.m_point.x + (toPlayer.m_normal.y * 5000);
				tarY = tar.y + targetRay.m_point.y + (toPlayer.m_normal.x * -5000);
			}
			else
			{
				//Counter Clockwise Dodging
				ali.GetBody()->SetAngularVelocity(200.f * Timer::deltaTime * adjustY);
				tarX = tar.x + targetRay.m_point.x + (toPlayer.m_normal.y * -5000);
				tarY = tar.y + targetRay.m_point.y + (toPlayer.m_normal.x * 5000);

			}
		}
		else
		{
			tarX = tar.x;
			tarY = tar.y;

		}
	}
}


float mousePosX = 0;
float mousePosY = 0;
void ClearObjects(b2World* m_physicsWorld)
{
	b2Body* body = m_physicsWorld->GetBodyList();
	for (int f = 0; f < m_physicsWorld->GetBodyCount(); f++)
	{

		if (body->GetFixtureList()->GetDensity() == 100.f)
		{

			PhysicsBody::m_bodiesToDelete.push_back((int)body->GetUserData());
			//cout << "\n" << (int)body->GetUserData();
			//m_physicsWorld->DestroyBody(m_physicsWorld->GetBodyList());


		}
		body = body->GetNext();
	}
	cout << "\nObjects Cleared.";


}
void ConeMovement(int visionCone)
{
	//FUCK THIS
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& vCone = ECS::GetComponent<Transform>(visionCone);
	auto& vConeB = ECS::GetComponent<PhysicsBody>(visionCone);


	b2Vec2 direction = (b2Vec2(mousePosX, mousePosY));
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
	direction = b2Vec2(direction.x / distance, direction.y / distance);

	vConeB.SetRotationAngleDeg((atan2(direction.y, direction.x) * 180 / 3.14f) + 90);
	vCone.SetPosition(player.GetPosition().x, player.GetPosition().y, 99.f);


}
int pastPosX = 9;
int pastPosY = 9;
int quad = 9;
bool ranonce = false;
void PhysicsPlayground::Update()
{
	{
		auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());
		Scene::AdjustScrollOffset();
		player.Update();

	}
	//cout << "\n"<<alienRetentionTimer;
	auto& rayM = ECS::GetComponent<Transform>(rayMarker);
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	auto& aliT = ECS::GetComponent<Transform>(alien);
	auto& playerT = ECS::GetComponent<Transform>(MainEntities::MainPlayer());
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& rad = ECS::GetComponent<Transform>(radar);
	auto& radSpr = ECS::GetComponent<Sprite>(radar);
	auto& vCone = ECS::GetComponent<Transform>(visionCone);
	auto& vConeB = ECS::GetComponent<PhysicsBody>(visionCone);

	auto& aliene = ECS::GetComponent<PhysicsBody>(alien);

	int alienex = aliene.GetPosition().x;
	int alieney = aliene.GetPosition().y;

	int plx = player.GetPosition().x;
	int ply = player.GetPosition().y;

	std::cout << "Alien Pos: " << alienex << " " << alieney << std::endl;
	//auto& playerObject = ECS::GetComponent<PhysicsBody>(redterminal);
	int xdi = (alienex - plx);
	int ydi = (alieney - ply);
	double newDistance = (pow(xdi, 2) + (pow(ydi, 2)));
	double distance = sqrt(newDistance);


	{
		std::string filename;

		aniClockDif = ((clock() - aniClock) / (double)(CLOCKS_PER_SEC));

		double aniSpeed = 0.2;

		if (aniClockDif < aniSpeed && aniClock > 0) {
			if (ali.GetVelocity().x >= 1 || ali.GetVelocity().y >= 1) {
				filename = "E/alienrunanimframe4.png";
			}
			else if (ali.GetVelocity().x <= -1 || ali.GetVelocity().y <= -1) {
				filename = "E/alienrunanimframe4l.png";
			}
			else {
				filename = "AlienIdle.png";
			}
		}
		else if (aniClockDif < (2 * aniSpeed) && aniClock > (2 * aniSpeed) - aniSpeed) {
			if (ali.GetVelocity().x >= 1 || ali.GetVelocity().y >= 1) {
				filename = "E/alienrunanimframe3.png";
			}
			else if (ali.GetVelocity().x <= -1 || ali.GetVelocity().y <= -1) {
				filename = "E/alienrunanimframe3l.png";
			}
			else {
				filename = "AlienIdle.png";
			}
		}
		else if (aniClockDif < (3 * aniSpeed) && aniClock > (3 * aniSpeed) - aniSpeed) {
			if (ali.GetVelocity().x >= 1 || ali.GetVelocity().y >= 1) {
				filename = "E/alienrunanimframe2.png";
			}
			else if (ali.GetVelocity().x <= -1 || ali.GetVelocity().y <= -1) {
				filename = "E/alienrunanimframe2l.png";
			}
			else {
				filename = "AlienIdle.png";
			}
		}
		else if (aniClockDif < (4 * aniSpeed) && aniClock > (4 * aniSpeed) - aniSpeed) {
			if (ali.GetVelocity().x >= 1 || ali.GetVelocity().y >= 1) {
				filename = "E/alienrunanimframe1.png";
			}
			else if (ali.GetVelocity().x <= -1 || ali.GetVelocity().y <= -1) {
				filename = "E/alienrunanimframe1l.png";
			}
			else {
				filename = "AlienIdle.png";
			}
		}

		if (aniClockDif > (3.8* aniSpeed)) {
			aniClock = clock();
		}

		ECS::GetComponent<Sprite>(alienSpr).LoadSprite(filename, 64, 64);
		//std::cout << "File: " << filename << " Time: " << aniClockDif << std::endl;
	}

	if (distance < 40) {
		showEndScreen = true;
	}
	else {
		std::cout << "Distance: " << distance << std::endl;
	}

	if (hideStartScreen) {
		ECS::GetComponent<Sprite>(sScreen).SetTransparency(0.f);
	}
	else {
		player.SetPosition(b2Vec2(0,0));
		ECS::GetComponent<Sprite>(sScreen).SetTransparency(1.f);
	}
	

	if (showEndScreen) {
		player.SetPosition(b2Vec2(0, 0));
		timeDeff = (clock() - startTime) / (double)(CLOCKS_PER_SEC);
		//ECS::GetComponent<Sprite>(eScreen).SetTransparency(1.f);
		std::cout << "GAME OVER" << std::endl << "INSERT 1 TOKEN TO PLAY AGAIN" << std::endl;
		if (showingDeathAni == false) {
			showingDeathAni = true;
			startTime = clock();
		}

		if (showingDeathAni == true) {
			if (timeDeff < 0.4 && timeDeff > 0.2) {
				ECS::GetComponent<Sprite>(dan1).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(dan2).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan3).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan4).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan5).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan6).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan7).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan8).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan9).SetTransparency(0.f);
			}
			else if (timeDeff < 0.6 && timeDeff > 0.4) {
				ECS::GetComponent<Sprite>(dan1).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan2).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(dan3).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan4).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan5).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan6).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan7).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan8).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan9).SetTransparency(0.f);

			}
			else if (timeDeff < 0.8 && timeDeff > 0.6) {
				ECS::GetComponent<Sprite>(dan1).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan2).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan3).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(dan4).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan5).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan6).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan7).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan8).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan9).SetTransparency(0.f);
			}
			else if (timeDeff < 1.0 && timeDeff > 0.8) {
				ECS::GetComponent<Sprite>(dan1).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan2).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan3).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan4).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(dan5).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan6).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan7).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan8).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan9).SetTransparency(0.f);
			}
			else if (timeDeff < 1.2 && timeDeff > 1.0) {
				ECS::GetComponent<Sprite>(dan1).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan2).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan3).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan4).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan5).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(dan6).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan7).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan8).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan9).SetTransparency(0.f);
			}
			else if (timeDeff < 1.4 && timeDeff > 1.2) {
				ECS::GetComponent<Sprite>(dan1).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan2).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan3).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan4).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan5).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan6).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(dan7).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan8).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan9).SetTransparency(0.f);
			}
			else if (timeDeff < 1.6 && timeDeff > 1.4) {
				ECS::GetComponent<Sprite>(dan1).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan2).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan3).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan4).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan5).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan6).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan7).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(dan8).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan9).SetTransparency(0.f);
			}
			else if (timeDeff < 1.8 && timeDeff > 1.6) {
				ECS::GetComponent<Sprite>(dan1).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan2).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan3).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan4).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan5).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan6).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan7).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan8).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(dan9).SetTransparency(0.f);
			}
			else if (timeDeff < 2.0 && timeDeff > 1.8) {
				ECS::GetComponent<Sprite>(dan1).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan2).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan3).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan4).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan5).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan6).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan7).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan8).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(dan9).SetTransparency(1.f);
			}
		}

	}
	//else {
	//	ECS::GetComponent<Sprite>(sScreen).SetTransparency(0.f);
	//}

	if (Input::GetKey(Key::Space) && hideStartScreen == false) {
		std::cout << "Space is being pressed" << std::endl;
		hideStartScreen = true;
	}

	if (GetKeyState(Key::V) != 0) {
		std::cout << "X: " << player.GetPosition().x << " Y: " << player.GetPosition().y << std::endl;
	}

	if (GetKeyState(Key::E) != 0) {
		//cards
		{
		if (player.GetPosition().x < (1787 + 75) && player.GetPosition().x >(1787 - 75) && player.GetPosition().y < (598 + 75) && player.GetPosition().y >(598 - 75) && hasRC == false) {
			hasRC = true;
			ECS::GetComponent<PhysicsBody>(RCobj).SetPosition(b2Vec2(-10000, -1000));
			ECS::DestroyEntity(RCobj);
		}
		else if (player.GetPosition().x < (292 + 75) && player.GetPosition().x >(292 - 75) && player.GetPosition().y < (-124 + 75) && player.GetPosition().y >(-124 - 75) && hasYC == false) {
			hasYC = true;
			ECS::GetComponent<PhysicsBody>(YCobj).SetPosition(b2Vec2(-10000, -1000));
			ECS::DestroyEntity(YCobj);
		}
		else if (player.GetPosition().x < (1620 + 75) && player.GetPosition().x >(1620 - 75) && player.GetPosition().y < (-763 + 75) && player.GetPosition().y >(-763 - 75) && hasGC == false) {
			hasGC = true;
			ECS::GetComponent<PhysicsBody>(GCobj).SetPosition(b2Vec2(-10000, -1000));
			ECS::DestroyEntity(GCobj);
		}
		else if (player.GetPosition().x < (380 + 75) && player.GetPosition().x >(380 - 75) && player.GetPosition().y < (400 + 75) && player.GetPosition().y >(400 - 75) && hasBC == false) {
			hasBC = true;
			ECS::GetComponent<PhysicsBody>(BCobj).SetPosition(b2Vec2(-10000, -1000));
			ECS::DestroyEntity(BCobj);
		}
		}

		//terminals
		{
			if (player.GetPosition().x < (897 + 75) && player.GetPosition().x >(897 - 75) && player.GetPosition().y < (825 + 75) && player.GetPosition().y >(825 - 75) && hasRC && rTA == false) {
				rTA = true;;
			}
			else if (player.GetPosition().x < (256 + 75) && player.GetPosition().x >(256 - 75) && player.GetPosition().y < (78 + 75) && player.GetPosition().y >(78 - 75) && hasYC && yTA == false) {
				yTA = true;
				ECS::GetComponent<PhysicsBody>(yd1).SetPosition(b2Vec2(-10000, -1000));
				ECS::DestroyEntity(yd1);
				ECS::DestroyEntity(yd1s);
				ECS::GetComponent<PhysicsBody>(yd2).SetPosition(b2Vec2(-10000, -1000));
				ECS::DestroyEntity(yd2);
				ECS::DestroyEntity(yd2s);
				ECS::GetComponent<PhysicsBody>(yd3).SetPosition(b2Vec2(-10000, -1000));
				ECS::DestroyEntity(yd3);
			}
			else if (player.GetPosition().x < (1022 + 75) && player.GetPosition().x >(1022 - 75) && player.GetPosition().y < (260 + 75) && player.GetPosition().y >(260 - 75) && hasGC && gTA == false) {
				gTA = true;
				ECS::GetComponent<PhysicsBody>(gd1).SetPosition(b2Vec2(-10000, -1000));
				ECS::DestroyEntity(gd1);
				ECS::GetComponent<PhysicsBody>(gd2).SetPosition(b2Vec2(-10000, -1000));
				ECS::DestroyEntity(gd2);
			}
			else if (player.GetPosition().x < (2016 + 75) && player.GetPosition().x >(2016 - 75) && player.GetPosition().y < (542 + 75) && player.GetPosition().y >(542 - 75) && hasBC && bTA == false) {
				bTA = true;
				ECS::GetComponent<PhysicsBody>(bd1).SetPosition(b2Vec2(-10000, -1000));
				ECS::DestroyEntity(bd1);
				ECS::DestroyEntity(bd1s);
				ECS::GetComponent<PhysicsBody>(bd2).SetPosition(b2Vec2(-10000, -1000));
				ECS::DestroyEntity(bd2);
				ECS::DestroyEntity(bd2s);
				ECS::GetComponent<PhysicsBody>(bd3).SetPosition(b2Vec2(-10000, -1000));
				ECS::DestroyEntity(bd3);
				ECS::DestroyEntity(bd3s);
				ECS::GetComponent<PhysicsBody>(bd4).SetPosition(b2Vec2(-10000, -1000));
				ECS::DestroyEntity(bd4);
				ECS::GetComponent<PhysicsBody>(bd5).SetPosition(b2Vec2(-10000, -1000));
				ECS::DestroyEntity(bd5);
			}
		}

		if (rTA && yTA && gTA && bTA && finalDoorisOpen == false) {
			finalDoorisOpen = true;
			ECS::GetComponent<PhysicsBody>(finDoor).SetPosition(b2Vec2(-10000, -1000));
			ECS::DestroyEntity(finDoor);
			ECS::DestroyEntity(finDoorS);
		}

	}

	ConeMovement(visionCone);

	//Raycast Pointing Towards Player
	RayCastCallback cb;
	m_physicsWorld->RayCast(&cb, ali.GetBody()->GetWorldPoint(b2Vec2(0, 0)), player.GetBody()->GetWorldPoint(b2Vec2(0, 0)));


	//Raycast Visualization
	//rayM.SetPosition(cb.m_point.x, cb.m_point.y, 5);

	//UI Function
	UI(radar, alien);


	//If player and alien are on the same tile:
	if (found == false)
	{
		if (ranonce == true)
		{
			if (Search(alien, rayMarker, m_physicsWorld) == true)
			{
				found = true;
			}
		}
		if (created == true)
		{
			cout << "\n\nALIEN STEPS:" << alienSteps;
			//cout << "IM HERE!";
			Chase(alien, m_physicsWorld, b2Vec2(x128(getPathCol(alienSteps) - 3), x128(24 -getPathRow(alienSteps) - 11)));
			if (round(((ali.GetBody()->GetPosition().x) / 256) + 3) == getPathCol(alienSteps) && round(24 - ((ali.GetBody()->GetPosition().y / 256) + 11)) == getPathRow(alienSteps))
			{
				//alienTileTimer -= 1.f * Timer::deltaTime;
				//if (alienTileTimer <= 0.f)
				//{
				if (alienSteps > 0)
				{
					alienSteps--;
					//alienTileTimer = 1.f;
					cout << "\nStep Found";
				}
				//}
			}


			cout << "\n" << round(((ali.GetBody()->GetPosition().x) / 128) + 3) << " " << round(24 - ((ali.GetBody()->GetPosition().y / 128) + 11));
			cout << "\nTAREGET" << getPathCol(alienSteps) << " " << getPathRow(alienSteps);

		}
		

		//Patrol()
	}
	if (found == true)
	{
		if (cb.m_fixture)
		{
			if (cb.m_fixture->GetBody() == player.GetBody())
			{
				//Changes Radar Colour
				std::string fileName = "CircleMask.png";
				radSpr.LoadSprite(fileName, 5, 5);

				alienRetentionTimer = AlienRetention;

				//aCounter ensures that the alien stays in dodge mode for a little bit longer after the alien sees the player again
				if (aCounter >= 1)
				{
					Dodge(alien, m_physicsWorld, player.GetPosition());
					aCounter -= 1 * Timer::deltaTime;
				}
				else
				{
					Chase(alien, m_physicsWorld, player.GetPosition());
					//cout << "\nAlien Sees Player";

				}

			}

			else
			{
				//Changes the colour of the radar
				std::string fileName = "radar.png";
				radSpr.LoadSprite(fileName, 5, 5);
				alienRetentionTimer -= 1 * Timer::deltaTime;

				cout << "\n";
				aCounter = 150;

				Dodge(alien, m_physicsWorld, player.GetPosition());

			}
		}
		if (alienRetentionTimer <= 0)
		{
			alienRetentionTimer = 0;
			ali.SetPosition(b2Vec2(x128(round(ali.GetBody()->GetPosition().x / 128)), x128(round(ali.GetBody()->GetPosition().y / 128))));
			found = false;

		}
	}
	MoveTo(alien);



	int pX = (player.GetBody()->GetPosition().x);
	int pY = (player.GetBody()->GetPosition().y);

	int aX = (ali.GetBody()->GetPosition().x);
	int aY = (ali.GetBody()->GetPosition().y);
	int cX = x128(10);
	int cY = x128(1);


	if (round(player.GetBody()->GetPosition().x / 128) != pastPosX || round(player.GetBody()->GetPosition().y / 128) != pastPosY)
	{
		
		int col = round(((player.GetBody()->GetPosition().x) / 128) + 3);
		int row = round(24 - ((player.GetBody()->GetPosition().y / 128) + 11)); //should be 12, is offset to 11 for now
		int Acol = round(((ali.GetBody()->GetPosition().x) / 128) + 3);
		int Arow = round(24 - ((ali.GetBody()->GetPosition().y / 128) + 11)); //should be 12, is offset to 11 for now
		if (round(player.GetBody()->GetPosition().x / 128) != round(ali.GetBody()->GetPosition().x / 128) || round(player.GetBody()->GetPosition().y / 128) != round(ali.GetBody()->GetPosition().y / 128))
		{
			makeGrid(col, row, Acol, Arow);
			created = true;
			////cout << "\n" << col << " " << row;

			////updateGrid(col, row);

			CalculatePath();
			alienSteps = getPathcount() - 1;
			resetPathcount();
		}
		pastPosX = round(player.GetBody()->GetPosition().x / 128);
		pastPosY = round(player.GetBody()->GetPosition().y / 128);
		cout << "\nBODY COUNT!: " << m_physicsWorld->GetBodyCount();

		if (quad != 0)
		{
			if (pX < cX && pY < cY )
			{

				ClearObjects(m_physicsWorld);
				cout << "\nBOT LEFT";
				//Player spawn room
				corridoor(x128(0), x128(0), 90); //BL
				thickDiagonalOctogon(x128(-1), x128(-1), 0); //BL
				thickWall(0, x128(-1), 90); //BL
				thickDiagonalOctogon(x128(1), -x128(1), 90);//BL
				thickWall(0, x128(1), 270);//BL
				thickDiagonalOctogon(x128(1), x128(1), 180);//BL
				thickDiagonalOctogon(-x128(1), x128(1), 270);//BL
				thickDoor(x128(1), 0, 90);//BL
				thickDoor(-x128(1), 0, -90); //BL
				//Alien spawn room
				smallOctogonRoom(-x128(2), 0);//BL
				makeBox(128, 128, -x128(3), 0, 0);//BL
				makeBox(128, 128, -x128(2), -x128(1), 0);//BL
				makeBox(128, 128, -x128(2), x128(1), 0);//BL
				//Portion next to the player spawn room
				smallOctogonRoom(x128(2), 0);//BL
				corridoorThinOpening(x128(2), -x128(1), 270);//BL
				makeBox(128, 128, x128(2), -x128(2), 0);//BL
				makeBox(128, 128, x128(2), x128(1), 0);//BL
				lockerDX(x128(3), 0, 180);//BL
				corridoorThinOpening(x128(3), x128(1), 270);//BL
				Gap(x128(3) + 65, -20, 270);//BL
				makeBox(11, 128, x128(3) + 58.5, -x128(1), 0);//BL
				corridoorThinOpening(x128(3), -x128(2), 90);//BL
					// Small room to the left of the main corridor
				thickWall(x128(8), 0, 90); //bottom left
				makeBox(128, 128, x128(7), 0, 0); //bottom left
				makeBox(128, 128, x128(7), x128(1), 0); //bottom left
				thickWall(x128(9), 0, 0); //bottom left
				thickDoor(x128(9), x128(1), 270); //bottom left
				thickWall(x128(9), -x128(1), 0); //bottom left

				// Room under spawn
				Gap(x128(3) + 65, -x128(2) - 20, 180);				//Bottom Left
				sharpCorner(x128(2), -x128(3), 270);				//bottom left 
				sharpCorner(x128(4), -x128(3), 0);				//bottom left
				locker(x128(2), -x128(4), 180);					//bottom left 
				locker(x128(4), -x128(4), 0);					//bottom left
				sharpCorner(x128(2), -x128(5), 180);				//bottom left
				Gap(x128(3) + 35, -x128(5) - 20, 0);				//bottom left
				Gap(x128(4) + 65, -x128(4) - 20, 90);				//bottom left
				makeBox(128, 11, x128(4), -x128(5) - 58.5, 0);			//bottom left
				// Corridor leading to bottom main corridor			
				corridoorThinOpening(x128(5), -x128(5), 0);			//bottom left
				curvedCorner(x128(6), -x128(5), 90);				//bottom left
				makeBox(128, 128, x128(7), -x128(5), 0);			//bottom left
				wall(x128(6), -x128(4), 0);					//bottom left
				wall(x128(6), -x128(3), 0);					//bottom left
				thickWall(x128(7), -x128(4), 180);				//bottom left
				thickWall(x128(7), -x128(3), 180);				//bottom left
				thickCorner(x128(6), -x128(2), 270);				//bottom left
				thinThickCorridor1(x128(6) + 32, -x128(2) - 32, 180);		//bottom left
				thickishWall(x128(7), -x128(2), 90);				//bottom left
				thickishWall(x128(7), -x128(2), 270);				//bottom left
				thickishWall(x128(8), -x128(2), 90);				//bottom left
				thickishWall(x128(8), -x128(2), 270);				//bottom left
				thickishWall(x128(3), -x128(6), 0);				//bottom left
				thickishWall(x128(3), -x128(6), 180);				//bottom left
				thickCorner(x128(3), -x128(7), 0);				//bottom left
				thinThickCorridor1(x128(3) + 32, -x128(7) + 32, 270);		//bottom left
				thickishWall(x128(4), -x128(7), 90);				//bottom left
				thickishWall(x128(4), -x128(7), 270);				//bottom left
				thickishWall(x128(5), -x128(7), 90);				//bottom left
				thickishWall(x128(5), -x128(7), 270);				//bottom left
				thinThickCorridor(x128(6), -x128(7), 0);			//bottom left
				thinThickCorridor(x128(7), -x128(7), 180);			//bottom left
				thickishWall(x128(8), -x128(7), 90);				//bottom left
				thickishWall(x128(8), -x128(7), 270);				//bottom left
				// Bottom Main Corridor
				makeBox(128, 128, x128(8), -x128(8), 0);			//bottom left
				thickishDoor(x128(9), -x128(7), 270);				//bottom left


				Gap(x128(9) + 65, -x128(7) - 20, 270);				//bottom left

				Gap(x128(10) + 35, -x128(9) - 20, 0);				//bottom left
				curvedCorner(x128(9), -x128(9), 0);				//bottom left

				makeBox(128, 128, x128(10), -x128(10), 0);			//bottom left

				wall(x128(9), -x128(6), 0);					//bottom left

				Gap(x128(10) + 65, -x128(5) - 20, 180);				//bottom left

				curvedCorner(x128(9), -x128(5), 270);				//bottom left

				Gap(x128(10) + 65, -x128(4) - 20, 180);				//bottom left
				// Main Corridor
				thickishDoor(x128(10), -x128(4), 0);				//bottom left
				thickCorner(x128(9), -x128(4), 0);				//bottom left

				thickWall(x128(9), -x128(3), 0);				//bottom left

				thickDoor(x128(9), -x128(2), 270);				//bottom left

				quad = 0;

			}
		}

		if (quad != 1)
		{
			if (pX < cX && pY > cY)
			{
				ClearObjects(m_physicsWorld);
				cout << "\nTOP LEFT";
				//Octogon room above Spawn
				Gap(x128(3) + 35, x128(2) - 20, 0);//TL
				curvedCorner(x128(2), x128(2), 0);//TL
				wall(x128(2), x128(3), 0);//TL
				curvedCorner(x128(2), x128(4), 270);//TL
				curvedCorner(x128(4), x128(2), 90);//TL
				curvedCorner(x128(4), x128(4), 180);//TL
				Gap(x128(4) + 65, x128(4) - 20, 90);//TL
				Gap(x128(3) + 65, x128(5) - 20, 180);//TL
				corridoorThinOpening(x128(5), x128(3), 0);//TL
				thickishWall(x128(3), x128(5), 0);//TL
				thickishWall(x128(3), x128(5), 180);//TL
				//Bullet shaped room (left) 
				thickDoor(x128(3), x128(6), 0);//TL
				thickWall(x128(2), x128(6), 90);//TL
				thickWall(x128(4), x128(6), 90);//TL
				thickDiagonalOctogon(x128(1), x128(6), 0);//TL
				thickDiagonalOctogon(x128(1), x128(8), 270);//TL
				thickWall(x128(1), x128(7), 0);//TL
				thickWall(x128(2), x128(8), 270);//TL
				thickWall(x128(3), x128(8), 270);//TL
				thickWall(x128(4), x128(8), 270);//TL
				makeBox(128, 128, x128(5), x128(6), 0);//TL
				makeBox(128, 128, x128(5), x128(8), 0);//TL
				thickishWall(x128(5), x128(7), 90);//TL
				thickishWall(x128(5), x128(7), 270);//TL
				// Corridor leading to upper main corridor (left)
				Gap(x128(8) + 65, x128(9) - 20, 270);//TL
				thickCorner(x128(6), x128(7), 90);//TL
				thinThickCorridor1(x128(6) - 32, x128(7) + 32, 0);//TL
				thickCorner(x128(6), x128(8), 270);//TL
				thinThickCorridor1(x128(6) + 32, x128(8) - 32, 180);//TL
				thickishWall(x128(7), x128(8), 90);//TL
				thickishWall(x128(7), x128(8), 270);//TL
					// Corridor connecting the octogon room to the main corridor
				thinThickCorridor(x128(8), x128(3), 180); //top left
				Gap(x128(7) + 65, x128(4) - 20, 180); //top left
				wall(x128(7), x128(3), 90); //top left
				corridoor(x128(6), x128(3), 90); //top left
				//Octogon room above connect corridors
				Gap(x128(7) + 35, x128(4) - 20, 0); //top left
				curvedCorner(x128(6), x128(4), 0); //top left
				curvedCorner(x128(8), x128(4), 90); //top left
				wall(x128(6), x128(5), 0); //top left
				wall(x128(7), x128(6), 270); //top left
				curvedCorner(x128(6), x128(6), 270); //top left
				curvedCorner(x128(8), x128(6), 180); //top left
				Gap(x128(7) + 35, x128(6) - 20, 0); //top left
				Gap(x128(6) + 65, x128(6) - 20, 90); //top left
				Gap(x128(8) + 65, x128(6) - 20, 270); //top left
				Gap(x128(7) + 65, x128(5) - 20, 180); //top left
				makeBox(128, 128, x128(7), x128(2), 0); //top left
				thickWall(x128(8), x128(2), 270); //top left
				sharpCorner(x128(9), x128(9), 0); //top left
				sharpCorner(x128(8), x128(9), 270); //top left
				thickWall(x128(9), x128(4), 0); //top left
				thickWall(x128(9), x128(6), 0); //top left
				thickDoor(x128(9), x128(3), 270); //top left
				thickDoor(x128(9), x128(5), 270); //top left
				thickWall(x128(9), x128(2), 0); //top left
				quad = 1;
			}
		}

		if (quad != 2)
		{
			if (pX > cX && pY > cY)
			{
				ClearObjects(m_physicsWorld);
				cout << "\nTOP RIGHT";
				// Corridor leading to upper main corridor (right)
				makeBox(128, 128, x128(15), x128(8), 0);//TR
				Gap(x128(12) + 65, x128(9) - 20, 90);//TR
				thickishWall(x128(13), x128(8), 90);//TR
				thickishWall(x128(13), x128(8), 270);//TR
				thickCorner(x128(14), x128(8), 180);//TR
				thinThickCorridor1(x128(14) - 32, x128(8) - 32, 90);//TR
				thickCorner(x128(14), x128(7), 0);//TR
					// Small room to the right of the main corridor
				wall(x128(12), x128(5), 270); //top right
				sharpCorner(x128(14), x128(5), 0); //top right
				sharpCorner(x128(14), x128(4), 90); //top right
				wall(x128(12), x128(4), 90); //top right
				locker(x128(13), x128(5), 270); //top right
				locker(x128(13), x128(4), 90); //top right
				thickWall(x128(11), x128(6), 180); //top right
				curvedCorner(x128(8), x128(7), 0); //top left
				curvedCorner(x128(12), x128(7), 90); //top right
				sharpCorner(x128(12), x128(9), 0); //top right
				sharpCorner(x128(11), x128(9), 270); //top right
				thickDoor(x128(11), x128(5), 90); //top right
				thickWall(x128(11), x128(2), 180); //top right
				thickWall(x128(11), x128(3), 180); //top right
				thickWall(x128(11), x128(4), 180); //top right
				thickishDoor(x128(17), x128(2), 0);//TR
				thickishWall(x128(16), x128(2), 90);//TR
				thickishWall(x128(18), x128(2), 90);//TR
				thickishWall(x128(16), x128(4), 270);//TR
				thickishWall(x128(18), x128(4), 270);//TR
				thickishWall(x128(19), x128(3), 180);//TR
				thickCorner(x128(19), x128(2), 90);//TR
				thickCorner(x128(19), x128(4), 180);//TR
				makeBox(128, 128, x128(15), x128(2), 0);//TR
				makeBox(128, 128, x128(15), x128(4), 0);//TR
				lockerDX(x128(15), x128(3), 0);//TR
				thickishDoor(x128(17), x128(4), 180);//TR
				thickishWall(x128(17), x128(5), 0);//TR
				thickishWall(x128(17), x128(5), 180);//TR
				// Right Bullet Room
				thickDoor(x128(17), x128(6), 0);//TR
				thickWall(x128(16), x128(6), 90);//TR
				thickWall(x128(18), x128(6), 90);//TR
				thickWall(x128(19), x128(7), 180);//TR
				thickDiagonalOctogon(x128(19), x128(6), 90);//TR
				thickDiagonalOctogon(x128(19), x128(8), 180);//TR
				thickWall(x128(16), x128(8), 270);//TR
				thickWall(x128(17), x128(8), 270);//TR
				thickWall(x128(18), x128(8), 270);//TR
				makeBox(128, 128, x128(15), x128(6), 0);//TR
					// End Room
				largeOctogonRoom(x128(10), x128(11), true, true, false, true);//TR
				corridoor(x128(10), x128(9), 0);//TR

				thinThickCorridor1(x128(15) - 96, x128(7) + 32, 270); //Aedyn	//Top Right
				thickishWall(x128(15), x128(7), 90);				//Top Right
				thickishWall(x128(15), x128(7), 270);				//Top Right
				quad = 2;

			}
		}
		if (quad != 3)
		{
			if (pX > cX && pY < cY)
			{
				ClearObjects(m_physicsWorld);
				cout << "\nBOT RIGHT";
				//Corridor + Rooms at the bottom right of the map
				thinThickCorridor(x128(12), -x128(1), 0); //bottom right
				corridoor(x128(14), -x128(1), 90); //bottom right
				corridoorThinOpening(x128(15), -x128(1), 180); //bottom right
				thickCorner(x128(12), x128(1), 270); //bottom right
				thickCorner(x128(12), 0, 0); //bottom right
				thickCorner(x128(14), x128(1), 180); //bottom right
				thickCorner(x128(14), 0, 90); //bottom right
				thickishWall(x128(13), x128(1), 270); //bottom right
				thickishWall(x128(13), -x128(3), 90); //bottom right
				thickishDoor(x128(13), x128(0), 0); //bottom right
				thickCorner(x128(12), -x128(2), 270); //bottom right
				thickCorner(x128(12), -x128(3), 0); //bottom right
				thickCorner(x128(14), -x128(2), 180); //bottom right
				thickCorner(x128(14), -x128(3), 90); //bottom right
				thickWall(x128(11), x128(1), 180); //bottom right
				thickDoor(x128(11), -x128(1), 90); //bottom right
					//STEPHEN
				thickishDoor(x128(13), -x128(2), 180); //BR
				// Rightmost room
				makeBox(128, 128, x128(15), x128(0), 0);//BR
				makeBox(128, x128(3), x128(15), -x128(3), 0);//BR
				thickWall(x128(16), -x128(4), 90);//BR
				corridoorThinOpening(x128(17), 0, 90);//BR
				thickWall(x128(16), x128(0), 270);//BR
				thickWall(x128(18), x128(0), 270);//BR
				makeBox(128, x128(3), x128(19), -x128(1), 0);//BR
				thickDoor(x128(17), -x128(4), 0);//BR
				corridoor(x128(18), -x128(4), 0);//BR
				Gap(x128(18) + 35, -x128(2) - 20, 0);//BR
				locker(x128(18), -x128(3), 180);//BR
				thickishWall(x128(16), -x128(3), 180);//BR
				makeBox(128, 128, x128(18), -x128(5), 0);//BR
				makeBox(128, 128, x128(16), -x128(5), 0);//BR
				// Room below the right bullet room
				locker(x128(17), x128(1), 180);//BR
					// Room to the right of the bottom main shaft
				thickishWall(x128(17), -x128(5), 0);//BR
				thickishWall(x128(17), -x128(5), 180);//BR
				thickishWall(x128(17), -x128(6), 0);//BR
				thickishWall(x128(17), -x128(6), 180);//BR
				thickCorner(x128(17), -x128(7), 90);//BR
				thinThickCorridor1(x128(17) - 32, -x128(7) + 32, 0);//BR
				thickDoor(x128(16), -x128(7), 90);//BR
				thickWall(x128(16), -x128(6), 180);//BR
				thickDiagonalOctogon(x128(16), -x128(8), 90);//BR
				thickishWall(x128(15), -x128(8), 90);//BR
				thickWall(x128(14), -x128(8), 90);//BR
				locker(x128(15), -x128(6), 270);//BR
				thickDoor(x128(13), -x128(6), 270);//BR
				thickDoor(x128(13), -x128(7), 270);//BR
				thickishWall(x128(12), -x128(7), 90);//BR
				thickishWall(x128(12), -x128(7), 270);//BR
				makeBox(128, 128, x128(12), -x128(6), 0);//BR
				thickDiagonalOctogon(x128(13), -x128(8), 0);//BR
				makeBox(128, 128, x128(13), -x128(5), 0);//BR
				wall(x128(14), -x128(5), 270);//BR
				corridoor(x128(15), -x128(5), 90);//BR
				Gap(x128(14) + 35, -x128(5) - 20, 0);//BR
				thickishDoor(x128(11), -x128(7), 90);				//bottom right
				Gap(x128(11) + 65, -x128(7) - 20, 90);				//bottom right
				curvedCorner(x128(11), -x128(9), 90);				//bottom right
				makeBox(128, 128, x128(12), -x128(8), 0);			//bottom right
				wall(x128(11), -x128(6), 180);					//bottom right
				wall(x128(10), -x128(5), 90);					//bottom right
				curvedCorner(x128(11), -x128(5), 180);				//bottom right
				thickCorner(x128(11), -x128(4), 90);				//bottom right
				thickWall(x128(11), -x128(3), 180);				//bottom right
				thickWall(x128(11), -x128(2), 180);				//bottom right
				thickWall(x128(11), 0, 180);					//bottom right

				quad = 3;

			}
		}
		else
		{

			cout << "\nHey";

		}

		ranonce = true;

	}
	if (showEndScreen == false && hideStartScreen == true) {
		MoveTo(alien);
	}
	
}

void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& playerT = ECS::GetComponent<Transform>(MainEntities::MainPlayer());
	auto& playerSpr = ECS::GetComponent<Sprite>(MainEntities::MainPlayer());
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	auto& aliSpr = ECS::GetComponent<Transform>(alienSpr);
	//auto& Thing = ECS::GetComponent<PhysicsBody>(thing);
	float speed = 200.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	aliSpr.SetPosition(ali.GetBody()->GetPosition().x, ali.GetBody()->GetPosition().y, 4.f);

	//if ((vel.x > 1 || vel.x < -1) && (vel.y > 1 || vel.y < -1)) {
	//	//isMoving = true;
	//} else { isMoving = false; }

	if (Input::GetKey(Key::Shift))
	{
		speed *= 2.f;
	}

	if (Input::GetKey(Key::W))
	{
		vel.y += 10.f * Timer::deltaTime;
		//vel += b2Vec2(0.f, 8.f * Timer::deltaTime);
	}
	if (Input::GetKey(Key::S))
	{
		vel.y += 10.f * -Timer::deltaTime;
		//vel += b2Vec2(0.f, -8.f * Timer::deltaTime);
	}

	if (Input::GetKey(Key::A))
	{
		vel.x += 10.f * -Timer::deltaTime;
		//std::string fileName = "left.png";
		//playerSpr.LoadSprite(fileName, 32, 32);
		//vel += b2Vec2(-8.f * Timer::deltaTime, 0.f);
	}
	if (Input::GetKey(Key::D))
	{
		vel.x += 10.f * Timer::deltaTime;
		//std::string fileName = "right.png";
		//playerSpr.LoadSprite(fileName, 32, 32);
		//vel += b2Vec2(8.f * Timer::deltaTime, 0.f);
	}
	if (Input::GetKeyDown(Key::Q))
	{
		ali.SetPosition(b2Vec2(x128(8), x128(1)));

	}
	player.GetBody()->SetLinearVelocity(speed * vel + b2Vec2(player.GetBody()->GetLinearVelocity().x * 0.9f, player.GetBody()->GetLinearVelocity().y * 0.9f));
	//player.GetBody()->SetLinearVelocity(speed * vel);

	////Change physics body size for circle
	//if (Input::GetKey(Key::O))
	//{
	//	ali.ScaleBody(1.3f * Timer::deltaTime, 0);
	//}
	//else if (Input::GetKey(Key::I))
	//{
	//	ali.ScaleBody(-1.3f * Timer::deltaTime, 0);
	//}
}
bool viewAlien = false;

void PhysicsPlayground::KeyboardDown()
{
	auto& aliT = ECS::GetComponent<Transform>(alien);
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	//auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	auto& playerT = ECS::GetComponent<Transform>(MainEntities::MainPlayer());

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	/*if (canJump.m_canJump)
	{
		if (Input::GetKeyDown(Key::Space))
		{
			//player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 160000.f), true);
			canJump.m_canJump = false;
		}
	}*/



	if (Input::GetKeyDown(Key::Y))
	{
		
		if (viewAlien == false)
		{
			viewAlien = true;
		}
		else
		{
			viewAlien = false;

		}

	}

	if (Input::GetKeyDown(Key::H)) {
		hasRC = true;
		hasYC = true;
		hasGC = true;
		hasBC = true;
	}

	if (viewAlien == false)
	{
		ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&playerT);
		ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&playerT);
	}
	else
	{
		ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&aliT);
		ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&aliT);

	}

}

void PhysicsPlayground::KeyboardUp()
{


}

void PhysicsPlayground::MouseMotion(SDL_MouseMotionEvent evnt)
{

	//cout << "\n" << evnt.x << " "<< evnt.y;


	mousePosX = evnt.x - (ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetWindowSize().x / 2);
	mousePosY = (ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetWindowSize().y - evnt.y) - (ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetWindowSize().y / 2);



}