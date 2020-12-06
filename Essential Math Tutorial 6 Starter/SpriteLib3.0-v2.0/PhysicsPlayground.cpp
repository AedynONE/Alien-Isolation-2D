#include "PhysicsPlayground.h"
#include "Utilities.h"
#include "RayCastCallback.h"
#include <Box2d/Dynamics/b2Fixture.h>
#pragma comment(lib, "winmm.lib")
using namespace std;
#include <random>

int triggerPlacer = 2;

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

		vec4 temp = vec4(-90.f, 90.f, -90.f, 90.f);
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
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));
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
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));


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
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, OBJECTS, 0 , 0.f, 0.f);

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
		std::string fileName = "layer1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 4096.f, 4096.f);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0, 0, 0.f));
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
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 20.f));
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
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));
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
		redterminal = entity;
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
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));


		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 12.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(2.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//Sphere body
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 8.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30);
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
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, ENEMY, PLAYER | OBJECTS , 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(0.f);
		
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
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(250.f, -30.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 10.f;
		float shrinkY = 20.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(250.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//largeOctogonRoom(0, 0, true, true, true, true);
	//smallOctogonRoom(0, 0);

	//Sounds
	PlaySound(TEXT("assets/sounds/09_Alone.wav"), NULL, SND_FILENAME | SND_ASYNC);


	
	
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
	ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
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
	int ohfuck = 18;
	smallOctogonCorner(xPos + ohfuck, yPos + ohfuck, 0);
	smallOctogonCorner(xPos + ohfuck, yPos + -ohfuck, 270);
	smallOctogonCorner(xPos + -ohfuck, yPos + ohfuck, 90);
	smallOctogonCorner(xPos + -ohfuck, yPos + -ohfuck, 180);
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
	ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
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
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 128, 180);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xPos, yPos, 2.f));
	ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
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
	int help = 128;
	if (north == true) {
		thickStraightOctogon(xPos + 0, yPos + help, 270);
	}
	if (east == true) {
		thickStraightOctogon(xPos + help, yPos + 0, 180);
	}
	if (south == true) {
		thickStraightOctogon(xPos + 0, yPos + -help +20, 90);
	}
	if (west == true) {
		thickStraightOctogon(xPos + -help, yPos + 0, 0);
	}
	
	thickDiagonalOctogon(xPos + help, yPos + help, 180);
	thickDiagonalOctogon(xPos + help, yPos + -help, 90);
	thickDiagonalOctogon(xPos + -help, yPos + -help, 0);
	thickDiagonalOctogon(xPos + -help, yPos + help, 270);
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

//IMPORTANT VARIABLES
const float AlienSpeed = 6000.f;
const float AlienRetention = 3.f;



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



float tarX , tarY ;

void MoveTo(int alien)
{	
	//This function is always called every frame, and moves the alien in the direction of the target set by other functions
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	b2Vec2 direction = (b2Vec2(tarX, tarY) - ali.GetBody()->GetPosition());
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
	direction = b2Vec2(direction.x / distance, direction.y / distance);

	ali.GetBody()->SetLinearVelocity(b2Vec2(direction.x * Timer::deltaTime *AlienSpeed, direction.y * Timer::deltaTime * AlienSpeed));
	
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
bool Search(int alien,int rayMarker, b2World* m_physicsWorld)
{

	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	auto& rayM = ECS::GetComponent<Transform>(rayMarker);


	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	b2Vec2 direction = (b2Vec2(tarX, tarY) - ali.GetBody()->GetPosition());
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
	direction = b2Vec2(direction.x / distance, direction.y / distance);

	RayCastCallback vCone, vCone2, vCone3;
	m_physicsWorld->RayCast(&vCone,b2Vec2(direction.x * 100,direction.y * 100 ), ali.GetBody()->GetWorldPoint(b2Vec2(0, 0))); //Middle



	rayM.SetPosition(vCone.m_point.x, vCone.m_point.y, 5);



	if (vCone.m_fixture)
	{


		if (vCone.m_fixture->GetBody() == player.GetBody())
		{

			return true;

		}
		else
		{
			return false;

		}
	}
}


void Chase(int alien,b2World* m_physicsWorld)
{
	//This Function is simply used make the alien move in the direction of the player
	newWay = rand() % 2;
	stuckCounter = 0;
	aCounter = 0;
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());


	
	RayCastCallback toPlayer;
	m_physicsWorld->RayCast(&toPlayer, ali.GetBody()->GetWorldPoint(b2Vec2(0, 0)), player.GetBody()->GetWorldPoint(b2Vec2(0, 0)));


	tarX = player.GetBody()->GetPosition().x ;
	tarY = player.GetBody()->GetPosition().y ;


}

void Dodge(int alien,b2World* m_physicsWorld)
{
	//This Function is used to dodge obstacles between the alien and player
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	RayCastCallback dodgeRay;
	int maxRayIndex = 0;
	float adjustX = 1;
	float adjustY = 1;

	float maxDist = -10000;

	b2Vec2 newDirections[16] = {b2Vec2(0,-1),b2Vec2(0,1),b2Vec2(-1,0),b2Vec2(1,0),b2Vec2(1,1),b2Vec2(1,-1),b2Vec2(-1,-1),b2Vec2(-1,1),b2Vec2(0.5f,-1),b2Vec2(0.5f,1),b2Vec2(-1,0.5f),b2Vec2(1,0.5f),b2Vec2(0.5f,0.5f),b2Vec2(0.5f,-0.5f),b2Vec2(-0.5f,-0.5f),b2Vec2(-0.5f,0.5f) };
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
			Chase(alien, m_physicsWorld);
			nCounter -= 1 * Timer::deltaTime;
		}


		else
		{
			RayCastCallback toPlayer;
			m_physicsWorld->RayCast(&targetRay, ali.GetBody()->GetWorldPoint(b2Vec2(0, 0)), ali.GetBody()->GetWorldPoint(b2Vec2(newDirections[maxRayIndex].x * 10000, newDirections[maxRayIndex].y * 10000)));
			m_physicsWorld->RayCast(&toPlayer, ali.GetBody()->GetWorldPoint(b2Vec2(0, 0)), player.GetBody()->GetWorldPoint(b2Vec2(0, 0)));

			b2Vec2 direction4 = (b2Vec2(toPlayer.m_point.x, toPlayer.m_point.y) - b2Vec2(ali.GetBody()->GetPosition().x, ali.GetBody()->GetPosition().y));
			float distance4 = sqrt(direction4.x * direction4.x + direction4.y * direction4.y);

			if (distance4 < 25)
			{
				if (newWay == 0)
				{
					//Clockwise Dodging
					ali.GetBody()->SetAngularVelocity(-200.f * Timer::deltaTime * adjustX);
					tarX = player.GetBody()->GetPosition().x + targetRay.m_point.x + (toPlayer.m_normal.y * 5000);
					tarY = player.GetBody()->GetPosition().y + targetRay.m_point.y + (toPlayer.m_normal.x * -5000);
				}
				else
				{
					//Counter Clockwise Dodging
					ali.GetBody()->SetAngularVelocity(200.f * Timer::deltaTime * adjustY);
					tarX = player.GetBody()->GetPosition().x + targetRay.m_point.x + (toPlayer.m_normal.y * -5000);
					tarY = player.GetBody()->GetPosition().y + targetRay.m_point.y + (toPlayer.m_normal.x * 5000);

				}
			}
			else
			{
				tarX = player.GetBody()->GetPosition().x;
				tarY = player.GetBody()->GetPosition().y;

			}
		}
}


float mousePosX = 0;
float mousePosY = 0;

void ConeMovement(int visionCone)
{
	//FUCK THIS
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& vCone = ECS::GetComponent<Transform>(visionCone);
	auto& vConeB = ECS::GetComponent<PhysicsBody>(visionCone);
	

	b2Vec2 direction = (b2Vec2(mousePosX ,mousePosY ));
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
	direction = b2Vec2(direction.x / distance, direction.y / distance);

	vConeB.SetRotationAngleDeg((atan2(direction.y , direction.x ) * 180 / 3.14f) + 90);
	vCone.SetPosition(player.GetPosition().x, player.GetPosition().y, 99.f);


}


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
	auto& playerObject = ECS::GetComponent<PhysicsBody>(redterminal);



	ConeMovement(visionCone);

	//Raycast Pointing Towards Player
	RayCastCallback cb;
	m_physicsWorld->RayCast(&cb, ali.GetBody()->GetWorldPoint(b2Vec2(0,0)), player.GetBody()->GetWorldPoint(b2Vec2(0, 0)));
	

	//Raycast Visualization
	//rayM.SetPosition(cb.m_point.x, cb.m_point.y, 5);

	//UI Function
	UI(radar, alien);

	
//If player and alien are on the same tile:
	if (found == false)
	{
		if (Search(alien, rayMarker, m_physicsWorld) == true)
		{
			found = true;
		}

		
		//Patrol()
	}
	if (found == true)
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
				Dodge(alien, m_physicsWorld);
				aCounter -= 1 * Timer::deltaTime;
			}
			else
			{
				Chase(alien, m_physicsWorld);
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

			Dodge(alien, m_physicsWorld);

		}

		if (alienRetentionTimer <= 0)
		{
			alienRetentionTimer = 0;
			found = false;
			
		}
	}
	MoveTo(alien);
}

void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& playerT = ECS::GetComponent<Transform>(MainEntities::MainPlayer());
	auto& playerSpr = ECS::GetComponent<Sprite>(MainEntities::MainPlayer());
	auto& ali = ECS::GetComponent<PhysicsBody>(alien);
	auto& aliSpr = ECS::GetComponent<Transform>(alienSpr);
	float speed = 350.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);
	
	aliSpr.SetPosition(ali.GetBody()->GetPosition().x, ali.GetBody()->GetPosition().y, 2.f);

	//if ((vel.x > 1 || vel.x < -1) && (vel.y > 1 || vel.y < -1)) {
	//	//isMoving = true;
	//} else { isMoving = false; }
	
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
		//std::string fileName = "left.png";
		//playerSpr.LoadSprite(fileName, 32, 32);
		//vel += b2Vec2(-8.f * Timer::deltaTime, 0.f);
	}
	if (Input::GetKey(Key::D))
	{
		vel.x += Timer::deltaTime;
		//std::string fileName = "right.png";
		//playerSpr.LoadSprite(fileName, 32, 32);
		//vel += b2Vec2(8.f * Timer::deltaTime, 0.f);
	}
	if (Input::GetKeyDown(Key::E))
	{
		player.SetPosition(b2Vec2(0.f, 30.f));

	}
	player.GetBody()->SetLinearVelocity(speed * vel + b2Vec2(player.GetBody()->GetLinearVelocity().x * 0.9f, player.GetBody()->GetLinearVelocity().y * 0.9f));
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

	
	mousePosX = evnt.x -(ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetWindowSize().x/2);
	mousePosY = (ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetWindowSize().y -evnt.y) - (ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetWindowSize().y /2);



}
