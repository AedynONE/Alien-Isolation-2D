#include "PhysicsPlayground.h"
#include "Utilities.h"
#include "RayCastCallback.h"
#include <Box2d/Dynamics/b2Fixture.h>
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

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

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

		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "HelloWorld.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 60);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.5f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));
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
		std::string fileName = "XenomorphRoughDraft.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));
	}
	
	//Player entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);

		//Sets up the components
		std::string fileName = "astronautsprite.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 25);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 5.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//Sphere body
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 0.f));
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(70.f));
		
		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//Box body
		std::vector<b2Vec2> points = { b2Vec2(-tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f),b2Vec2(tempSpr.GetWidth() / 2.f,-tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2.f, tempSpr.GetHeight() / 2.f),b2Vec2(-tempSpr.GetWidth() / 2.f,tempSpr.GetHeight() / 2.f) };
		tempPhsBody = PhysicsBody(entity, BodyType::BOX, tempBody, points, vec2(0.f, 0.f), false, OBJECTS, ENEMY | PLAYER | PICKUP | TRIGGER, 0.5f, 3.f);
		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(0.f);
	}
	
	//largeOctogonRoom(0, 0, true, true, true, true);
	//smallOctogonRoom(0, 0);
	//thickCorner(0, 0, 0);
	curvedCorner(0, 0, 270);

	/*
	//Setup trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		
		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 40);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-30.f, -20.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new DestroyTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(ball);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-100.f), float32(10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}
	*/
	
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
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
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

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
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

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
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

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
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

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
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

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
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS, 1.f, 1.f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
}

float tarX , tarY ;

void MoveTo(int alien,float sX,float sY)
{	



	b2Vec2 direction = (b2Vec2(tarX, tarY) - b2Vec2(sX, sY));
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
	direction = b2Vec2(direction.x / distance, direction.y / distance);

	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	ali.GetBody()->SetLinearVelocity(b2Vec2(direction.x * Timer::deltaTime *60, direction.y * Timer::deltaTime * 60));

	
	//std::cout << "\n" << direction.x << ",\t" <<direction.y << "\t";


}
float startPosX, startPosY;


void FindNextPosition(int alien)
{
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());


		if ((int)ali.GetBody()->GetPosition().x == (int)tarX && (int)ali.GetBody()->GetPosition().y == (int)tarY)
		{
			startPosX = ali.GetBody()->GetPosition().x;
			startPosY = ali.GetBody()->GetPosition().y;
			tarX = player.GetBody()->GetPosition().x;
			tarY = player.GetBody()->GetPosition().y;
			

		}

		if ((int)ali.GetBody()->GetLinearVelocity().x == 0 && (int)ali.GetBody()->GetLinearVelocity().y == 0)
		{
			startPosX = ali.GetBody()->GetPosition().x;
			startPosY = ali.GetBody()->GetPosition().y;
			tarX = player.GetBody()->GetPosition().x;
			tarY = player.GetBody()->GetPosition().y;


		}

}
int aCounter = 0;
void Chase(int alien,b2World* m_physicsWorld)
{
	aCounter = 0;
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	float dodgeX = 1;
	float dodgeY = 1;
	RayCastCallback dodgeRay;
	int maxRayIndex = 0;
	b2Vec2 newDirections[16] = { b2Vec2(0,-1),b2Vec2(0,1),b2Vec2(-1,0),b2Vec2(1,0),b2Vec2(1,1),b2Vec2(1,-1),b2Vec2(-1,-1),b2Vec2(-1,1),b2Vec2(0.5f,-1),b2Vec2(0.5f,1),b2Vec2(-1,0.5f),b2Vec2(1,0.5f),b2Vec2(0.5f,0.5f),b2Vec2(0.5f,-0.5f),b2Vec2(-0.5f,-0.5f),b2Vec2(-0.5f,0.5f) };

	for (int i = 0; i < 16; i++)
	{
		m_physicsWorld->RayCast(&dodgeRay, ali.GetBody()->GetWorldPoint(b2Vec2(0, 0)), ali.GetBody()->GetWorldPoint(b2Vec2(newDirections[i].x * 10000, newDirections[i].y * 10000)));
		b2Vec2 direction = (b2Vec2(dodgeRay.m_point.x, dodgeRay.m_point.y) - b2Vec2(ali.GetBody()->GetPosition().x, ali.GetBody()->GetPosition().y));
		float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
		if (dodgeRay.m_fixture) {
			if (dodgeRay.m_fixture->GetBody() != player.GetBody())
				{
					if (distance < 25)
					{
						dodgeX = -1;
						dodgeY = -1;


					}		
			}
		}
		
	}
	RayCastCallback toPlayer;
	m_physicsWorld->RayCast(&toPlayer, ali.GetBody()->GetWorldPoint(b2Vec2(0, 0)), player.GetBody()->GetWorldPoint(b2Vec2(0, 0)));

	startPosX = ali.GetBody()->GetPosition().x;
	startPosY = ali.GetBody()->GetPosition().y;
	tarX = player.GetBody()->GetPosition().x ;
	tarY = player.GetBody()->GetPosition().y ;
}

void Dodge(int alien,b2World* m_physicsWorld)
{
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	RayCastCallback dodgeRay;
	int maxRayIndex = 0;
	float dodgeX = 1;
	float dodgeY = 1;
	float adjustX = 0;
	float adjustY = 0;

	float maxDist = -10000;
	//int ranDir = rand() % 16;
	b2Vec2 newDirections[16] = {b2Vec2(0,-1),b2Vec2(0,1),b2Vec2(-1,0),b2Vec2(1,0),b2Vec2(1,1),b2Vec2(1,-1),b2Vec2(-1,-1),b2Vec2(-1,1),b2Vec2(0.5f,-1),b2Vec2(0.5f,1),b2Vec2(-1,0.5f),b2Vec2(1,0.5f),b2Vec2(0.5f,0.5f),b2Vec2(0.5f,-0.5f),b2Vec2(-0.5f,-0.5f),b2Vec2(-0.5f,0.5f) };
	RayCastCallback targetRay;


		for (int i = 0; i < 16; i++)
		{
			m_physicsWorld->RayCast(&dodgeRay, ali.GetBody()->GetWorldPoint(b2Vec2(0, 0)), ali.GetBody()->GetWorldPoint(b2Vec2(newDirections[i].x * 10000, newDirections[i].y * 10000)));
			b2Vec2 direction = (b2Vec2(dodgeRay.m_point.x, dodgeRay.m_point.y) - b2Vec2(ali.GetBody()->GetPosition().x, ali.GetBody()->GetPosition().y));
			float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

			b2Vec2 direction2 = (b2Vec2(player.GetBody()->GetPosition().x, player.GetBody()->GetPosition().y) - b2Vec2(dodgeRay.m_point.x, dodgeRay.m_point.y));
			float distance2 = sqrt(direction2.x * direction2.x + direction2.y * direction2.y);

			float distance3 = distance - distance2;
			if (distance > maxDist)
			{
				maxDist = distance;
				maxRayIndex = i;
			}


			if (dodgeRay.m_fixture) {
				if (dodgeRay.m_fixture->GetBody() != player.GetBody())
				{
					if (distance < 25)
					{
						dodgeX = -1;
						dodgeY = -1;
					}
				}
			}
		}


			

		RayCastCallback toPlayer;
		m_physicsWorld->RayCast(&targetRay, ali.GetBody()->GetWorldPoint(b2Vec2(0, 0)), ali.GetBody()->GetWorldPoint(b2Vec2(newDirections[maxRayIndex].x * 10000, newDirections[maxRayIndex].y * 10000)));
		m_physicsWorld->RayCast(&toPlayer, ali.GetBody()->GetWorldPoint(b2Vec2(0, 0)), player.GetBody()->GetWorldPoint(b2Vec2(0, 0)));

		b2Vec2 direction4 = (b2Vec2(toPlayer.m_point.x, toPlayer.m_point.y) - b2Vec2(ali.GetBody()->GetPosition().x, ali.GetBody()->GetPosition().y));
		float distance4 = sqrt(direction4.x * direction4.x + direction4.y * direction4.y);

		startPosX = ali.GetBody()->GetPosition().x;
		startPosY = ali.GetBody()->GetPosition().y;
		tarX = player.GetBody()->GetPosition().x + targetRay.m_point.x + (toPlayer.m_normal.y * -5000);
		tarY = player.GetBody()->GetPosition().y + targetRay.m_point.y + (toPlayer.m_normal.x * 5000);





}


void PhysicsPlayground::Update()
{
	RayCastCallback cb;
	auto& rayM = ECS::GetComponent<Transform>(rayMarker);
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	
	ali.GetBody()->SetAngularVelocity(300.f * Timer::deltaTime);

	m_physicsWorld->RayCast(&cb, ali.GetBody()->GetWorldPoint(b2Vec2(0,0)), player.GetBody()->GetWorldPoint(b2Vec2(0, 0)));

	//Raycast Visualization
	rayM.SetPosition(cb.m_point.x, cb.m_point.y, 5);

	//FindNextPosition(alien);

	

//If player and alien are on the same tile:
	if (cb.m_fixture->GetBody() == player.GetBody())
	{
		if (aCounter >= 1)
		{
			Dodge(alien, m_physicsWorld);
			aCounter -= 1 *Timer::deltaTime;
		}
		else
		{
			Chase(alien, m_physicsWorld);
			cout << "\nAlien Sees Player";

		}

	}

	else
	{

		cout << "\n";

		aCounter = 150;


		
		Dodge(alien, m_physicsWorld);


	}


	MoveTo(alien,startPosX,startPosY);
}

void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	auto& aliSpr = ECS::GetComponent<Transform>(alienSpr);
	float speed = 75.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);
	
	aliSpr.SetPosition(ali.GetBody()->GetPosition().x, ali.GetBody()->GetPosition().y, 2.f);

	if (Input::GetKey(Key::Shift))
	{
		speed *= 2.f;
	}

	if (Input::GetKey(Key::W))
	{
		vel.y += Timer::deltaTime;
		//vel += b2Vec2(0.f, 8.f * Timer::deltaTime);
	}
	if (Input::GetKey(Key::S))
	{
		vel.y += -Timer::deltaTime;
		//vel += b2Vec2(0.f, -8.f * Timer::deltaTime);
	}

	if (Input::GetKey(Key::A))
	{
		vel.x += -Timer::deltaTime;
		//vel += b2Vec2(-8.f * Timer::deltaTime, 0.f);
	}
	if (Input::GetKey(Key::D))
	{
		vel.x += Timer::deltaTime;
		//vel += b2Vec2(8.f * Timer::deltaTime, 0.f);
	}

	player.GetBody()->SetLinearVelocity(speed * vel + b2Vec2(player.GetBody()->GetLinearVelocity().x * 0.98f, player.GetBody()->GetLinearVelocity().y * 0.98f));
	//player.GetBody()->SetLinearVelocity(speed * vel);

	//Change physics body size for circle
	if (Input::GetKey(Key::O))
	{
		ali.ScaleBody(1.3f * Timer::deltaTime, 0);
	}
	else if (Input::GetKey(Key::I))
	{
		ali.ScaleBody(-1.3f * Timer::deltaTime, 0);
	}
}


void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	if (canJump.m_canJump)
	{
		if (Input::GetKeyDown(Key::Space))
		{
			player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 160000.f), true);
			canJump.m_canJump = false;
		}
	}
}

void PhysicsPlayground::KeyboardUp()
{
	

}
