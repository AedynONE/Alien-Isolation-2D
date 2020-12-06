#include "Player.h"

Player::Player()
{
}

Player::Player(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
					AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body);
}

void Player::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
							AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	//Store references to the components
	m_sprite = sprite;
	m_animController = controller;
	m_transform = transform;
	m_hasPhysics = hasPhys;
	if (hasPhys)
	{
		m_physBody = body;
	}

	//Initialize UVs
	m_animController->InitUVs(fileName);

	//Loads the texture and sets width and height
	m_sprite->LoadSprite(fileName, width, height, true, m_animController);
	m_animController->SetVAO(m_sprite->GetVAO());
	m_animController->SetTextureSize(m_sprite->GetTextureSize());

	//Loads in the animations json file
	nlohmann::json animations = File::LoadJSON(animationJSON);

	//Walk Animations\\

	//WalkRight
	m_animController->AddAnimation(animations["WalkRight"].get<Animation>());
	//WalkLeft
	m_animController->AddAnimation(animations["WalkLeft"].get<Animation>());

	//IDLE ANIMATIONS\\
	
	//Idle Left
	m_animController->AddAnimation(animations["IdleLeft"].get<Animation>());
	//Idle Right
	m_animController->AddAnimation(animations["IdleRight"].get<Animation>());

	//Set Default Animation
	m_animController->SetActiveAnim(IDLERIGHT);
}

void Player::Update()
{
	if (!m_locked)
	{
		MovementUpdate();
	}

	AnimationUpdate();
}

void Player::MovementUpdate()
{
	
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	if ((player.GetBody()->GetLinearVelocity().x > 2 || player.GetBody()->GetLinearVelocity().x < -2) || (player.GetBody()->GetLinearVelocity().y > 2 || player.GetBody()->GetLinearVelocity().y < -2))  {
		m_moving = true;
	}
	else { m_moving = false; }

	//m_moving = &PhysicsPlayground::isMoving;

	//m_moving = false;
	if (Input::GetKey(Key::W) || Input::GetKey(Key::S))
	{
		//m_moving = true;
	}
	if (Input::GetKey(Key::A))
	{
		m_facing = LEFT;
		//m_moving = true;
	}
	if (Input::GetKey(Key::D))
	{
		m_facing = RIGHT;
		//m_moving = true;
	}
}

void Player::AnimationUpdate()
{
	int activeAnimation = 0;

	if (m_moving)
	{
		//Puts it into the WALK category
		activeAnimation = WALK;
	}
	else
	{
		activeAnimation = IDLE;
	}

	SetActiveAnimation(activeAnimation + (int)m_facing);
}

void Player::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}
